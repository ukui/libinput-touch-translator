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

#include "touch-screen-five-finger-swipe-gesture.h"

TouchScreenFiveFingerSwipeGesture::TouchScreenFiveFingerSwipeGesture(QObject *parent) : TouchScreenGestureInterface(parent)
{

}

TouchScreenGestureInterface::State TouchScreenFiveFingerSwipeGesture::handleInputEvent(libinput_event *event)
{
    switch (libinput_event_get_type(event)) {
    case LIBINPUT_EVENT_TOUCH_DOWN: {
        m_current_finger_count++;
        if (m_is_cancelled)
            return Ignore;
        auto touch_event = libinput_event_get_touch_event(event);

        int current_finger_count = m_current_finger_count;
        //qDebug()<<"current finger count:"<<current_finger_count;
        int current_slot = libinput_event_touch_get_slot(touch_event);

        double mmx = libinput_event_touch_get_x(touch_event);
        double mmy = libinput_event_touch_get_y(touch_event);

        if (current_finger_count <= 5) {
            m_start_points[current_slot] = QPointF(mmx, mmy);
        }

        if (current_finger_count == 5) {
            // start the gesture
            m_is_started = true;
            for (int i = 0; i < 5; i++) {
                m_last_points[i] = m_start_points[i];
                m_current_points[i] = m_start_points[i];
            }
            emit gestureBegin(getGestureIndex());
            return Maybe;
        }

        if (current_finger_count > 5) {
            m_is_cancelled = true;
            emit gestureCancelled(getGestureIndex());
            return Cancelled;
        }
        break;
    }
    case LIBINPUT_EVENT_TOUCH_MOTION: {
        if (m_is_cancelled)
            return Ignore;

        if (!m_is_started) {
            return Ignore;
        }

        // update position
        auto touch_event = libinput_event_get_touch_event(event);

        int current_slot = libinput_event_touch_get_slot(touch_event);

        double mmx = libinput_event_touch_get_x(touch_event);
        double mmy = libinput_event_touch_get_y(touch_event);

        m_current_points[current_slot] = QPointF(mmx, mmy);
        break;
    }
    case LIBINPUT_EVENT_TOUCH_UP: {
        m_current_finger_count--;
        int current_finger_count = m_current_finger_count;

        if (current_finger_count <= 0) {
            if (!m_is_cancelled && m_is_started && m_last_direction != None) {
                emit gestureFinished(getGestureIndex());
                //qDebug()<<"total direction:"<<totalDirection();
                return Finished;
            } else {
                // we have post cancel event yet.
                reset();
                return Ignore;
            }
        }

        break;
    }
    case LIBINPUT_EVENT_TOUCH_FRAME: {
        if (m_is_cancelled || !m_is_started)
            return Ignore;

        // update gesture

        // count offset
        auto last_center_points = (m_last_points[0] + m_last_points[1] + m_last_points[2] + m_last_points[3] + m_last_points[4])/5;
        auto current_center_points = (m_current_points[0] + m_current_points[1] + m_current_points[2] + m_last_points[3] + m_last_points[4])/5;
        auto delta = current_center_points - last_center_points;
        auto offset = delta.manhattanLength();
        if (offset < 20) {
            return Ignore;
        }

        for (int i = 0; i < 5; i++) {
            m_last_points[i] = m_current_points[i];
        }

        if (qAbs(delta.x()) > qAbs(delta.y())) {
            m_last_direction = delta.x() > 0? Right: Left;
        } else {
            m_last_direction = delta.y() > 0? Down: Up;
        }

        emit gestureUpdate(getGestureIndex());

        return Update;

        break;
    }
    case LIBINPUT_EVENT_TOUCH_CANCEL: {
        m_is_cancelled = true;
        emit gestureCancelled(getGestureIndex());
        return Cancelled;
        break;
    }
    default:
        break;
    }

    return Ignore;
}

void TouchScreenFiveFingerSwipeGesture::reset()
{
    m_is_cancelled = false;
    m_is_started = false;
    m_last_direction = None;

    for (int i = 0; i < 5; i++) {
        m_start_points[i] = QPointF();
        m_last_points[i] = QPointF();
        m_current_points[i] = QPointF();
    }
}

TouchScreenGestureInterface::Direction TouchScreenFiveFingerSwipeGesture::totalDirection()
{
    // count total offset
    auto start_center_points = (m_start_points[0] + m_start_points[1] + m_start_points[2] + m_start_points[3] + m_start_points[4])/5;
    auto current_center_points = (m_current_points[0] + m_current_points[1] + m_current_points[2] + m_current_points[3] + m_current_points[4])/5;
    auto delta = current_center_points - start_center_points;
    auto offset = delta.manhattanLength();
    if (offset < 20) {
        return None;
    }

    if (qAbs(delta.x()) > qAbs(delta.y())) {
        return delta.x() > 0? Right: Left;
    } else {
        return delta.y() > 0? Down: Up;
    }
}

TouchScreenGestureInterface::Direction TouchScreenFiveFingerSwipeGesture::lastDirection()
{
    return m_last_direction;
}

void TouchScreenFiveFingerSwipeGesture::cancel()
{
    m_is_cancelled = true;
    emit gestureCancelled(getGestureIndex());
}
