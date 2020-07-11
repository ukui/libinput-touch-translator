/*
 * Libinput Touch Translator
 *
 * Copyright (C) 2020, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Authors: Yue Lan <lanyue@kylinos.cn>
 *
 */

#include "event-monitor.h"

#include "touch-screen/touch-screen-gesture-manager.h"
#include "touchpad/touchpad-gesture-manager.h"

#include <libudev.h>
#include <linux/uinput.h>
#include <fcntl.h>
#include <unistd.h>

#include <poll.h>

static int finger_count = 0;

static int open_restricted(const char *path, int flags, void *user_data)
{
    int fd = open(path, flags);
    return fd < 0 ? -errno : fd;
}

static void close_restricted(int fd, void *user_data)
{
    close(fd);
}

const static struct libinput_interface interface = {
    .open_restricted = open_restricted,
    .close_restricted = close_restricted,
};

EventMonitor::EventMonitor(QObject *parent) : QObject(parent)
{
    struct libinput *li;
    struct libinput_event *event;

    udev *dev = udev_new();
    li = libinput_udev_create_context(static_cast<const libinput_interface*>(&interface), NULL, dev);
    if (!li)
        return;
    libinput_udev_assign_seat(li, "seat0");
    libinput_dispatch(li);

    bool successed = false;
    while ((event = libinput_get_event(li)) != NULL) {
        auto type = libinput_event_get_type(event);
        switch (type) {
        case LIBINPUT_EVENT_DEVICE_ADDED: {
            libinput_device *dev = libinput_event_get_device(event);
            const char *name = libinput_device_get_name(dev);
            printf("%s added\n", name);
            libinput_device_config_send_events_set_mode(dev, LIBINPUT_CONFIG_SEND_EVENTS_ENABLED);
            if (libinput_device_has_capability(dev, LIBINPUT_DEVICE_CAP_TOUCH)) {
                printf("has cap touch\n");
                libinput_device_ref(dev);
                // config device?
            }
            break;
        }
        default:
            printf("other event %d\n", type);
            break;
        }
        libinput_event_destroy(event);
        libinput_dispatch(li);

        successed = true;
    }

    if (!successed)
        fprintf(stderr, "can not get devices, maybe permission problem.\n");

    m_input = li;
}

void EventMonitor::startMonitor()
{
    struct libinput *li = m_input;
    struct libinput_event *event;

    struct pollfd fds;

    fds.fd = libinput_get_fd(li);
    fds.events = POLLIN;
    fds.revents = 0;
    /* time offset starts with our first received event */
    if (poll(&fds, 1, -1) > -1) {
        struct timespec tp;

        clock_gettime(CLOCK_MONOTONIC, &tp);
        //start_time = tp.tv_sec * 1000 + tp.tv_nsec / 1000000;
        do {
            libinput_dispatch(li);
            while ((event = libinput_get_event(li)) != NULL) {

                // handle the event here
                auto type = libinput_event_get_type(event);
                //printf("loop\n");

                if (type == LIBINPUT_EVENT_TOUCH_DOWN) {
                    finger_count++;
                } else if (type = LIBINPUT_EVENT_TOUCH_UP) {
                    finger_count--;
                }

                switch (type) {
                case LIBINPUT_EVENT_TOUCH_DOWN:
                case LIBINPUT_EVENT_TOUCH_MOTION:
                case LIBINPUT_EVENT_TOUCH_UP:
                case LIBINPUT_EVENT_TOUCH_FRAME:
                case LIBINPUT_EVENT_TOUCH_CANCEL:
                    //printf("touch event %d\n", type);
                    if (m_touchScreenGestureManager) {
                        m_touchScreenGestureManager->processEvent(event);
                    }
                    if (finger_count == 0) {
                        m_touchScreenGestureManager->forceReset();
                    }
                    break;

                case LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN:
                case LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE:
                case LIBINPUT_EVENT_GESTURE_SWIPE_END:
                case LIBINPUT_EVENT_GESTURE_PINCH_BEGIN:
                case LIBINPUT_EVENT_GESTURE_PINCH_UPDATE:
                case LIBINPUT_EVENT_GESTURE_PINCH_END: {
                    TouchpadGestureManager::getManager()->processEvent(event);
                    break;
                }
                default:
                    //printf("other event %d\n", type);
                    break;
                }
                libinput_event_destroy(event);
                libinput_dispatch(li);
            }
        } while (/*!stop && */poll(&fds, 1, -1) > -1);
    }

    libinput_unref(li);
}

void EventMonitor::initTouchScreenGestureManager(TouchScreenGestureManager *manager)
{
    m_touchScreenGestureManager = manager;
}
