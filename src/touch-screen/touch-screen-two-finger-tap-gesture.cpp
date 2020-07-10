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

#include "touch-screen-two-finger-tap-gesture.h"

TouchScreenTwoFingerTapGesture::TouchScreenTwoFingerTapGesture(QObject *parent) : TouchScreenGestureInterface(parent)
{

}

TouchScreenGestureInterface::State TouchScreenTwoFingerTapGesture::handleInputEvent(libinput_event *event)
{
    switch (libinput_event_get_type(event)) {
    case LIBINPUT_EVENT_TOUCH_DOWN: {
        m_currentFingerCount++;
        if (m_isCancelled)
            return Ignore;

        auto touch_event = libinput_event_get_touch_event(event);

        int current_finger_count = m_currentFingerCount;
        //qDebug()<<"current finger count:"<<current_finger_count;
        int current_slot = libinput_event_touch_get_slot(touch_event);

        double mmx = libinput_event_touch_get_x(touch_event);
        double mmy = libinput_event_touch_get_y(touch_event);

        if (current_finger_count <= 2) {
            m_startPoints[current_slot] = QPointF(mmx, mmy);
        }

        if (current_finger_count == 2) {
            // start the gesture
            m_startTime = libinput_event_touch_get_time(touch_event);
            for (int i = 0; i < 2; i++) {
                m_currentPoints[i] = m_startPoints[i];
            }
            emit gestureBegin(getGestureIndex());
            return Maybe;
        }

        if (current_finger_count > 2) {
            m_isCancelled = true;
            emit gestureCancelled(getGestureIndex());
            return Cancelled;
        }
        break;
    }
    case LIBINPUT_EVENT_TOUCH_MOTION: {
        if (m_isCancelled)
            return Ignore;

        // update position
        auto touch_event = libinput_event_get_touch_event(event);

        int current_slot = libinput_event_touch_get_slot(touch_event);

        double mmx = libinput_event_touch_get_x(touch_event);
        double mmy = libinput_event_touch_get_y(touch_event);

        m_currentPoints[current_slot] = QPointF(mmx, mmy);

        auto delta0 = (m_currentPoints[0] - m_startPoints[0]).manhattanLength();
        auto delta1 = (m_currentPoints[1] - m_startPoints[1]).manhattanLength();
        if (delta0 > 10 || delta1 > 10) {
            m_isCancelled = true;
            gestureCancelled(getGestureIndex());
            return Cancelled;
        }
        break;
    }
    case LIBINPUT_EVENT_TOUCH_UP: {
        m_currentFingerCount--;
        int current_finger_count = m_currentFingerCount;

        if (current_finger_count <= 0) {
            auto currentTime = libinput_event_touch_get_time(libinput_event_get_touch_event(event));
            auto timeDelta = currentTime - m_startTime;
            auto startDistance = (m_startPoints[0] - m_startPoints[1]).manhattanLength();
            if (!m_isCancelled && timeDelta < 300 && startDistance < 50) {
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
        return Ignore;
    }
    case LIBINPUT_EVENT_TOUCH_CANCEL: {
        m_isCancelled = true;
        emit gestureCancelled(getGestureIndex());
        return Cancelled;
        break;
    }
    default:
        break;
    }

    return Ignore;
}

void TouchScreenTwoFingerTapGesture::reset()
{
    m_currentFingerCount = 0;
    m_startTime = 0;
    m_endTime = 0;
    m_isCancelled = false;

    m_startPoints[0] = QPointF(0, 0);
}

TouchScreenGestureInterface::Direction TouchScreenTwoFingerTapGesture::totalDirection()
{
    return None;
}

TouchScreenGestureInterface::Direction TouchScreenTwoFingerTapGesture::lastDirection()
{
    return None;
}

void TouchScreenTwoFingerTapGesture::cancel()
{
    m_isCancelled = true;
}

bool TouchScreenTwoFingerTapGesture::isCancelled()
{
    return m_isCancelled;
}
