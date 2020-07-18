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

#include "touch-screen-two-finger-drag-and-tap-gesture.h"

TouchScreenTwoFingerDragAndTapGesture::TouchScreenTwoFingerDragAndTapGesture(QObject *parent) : TouchScreenGestureInterface(parent)
{

}

TouchScreenGestureInterface::State TouchScreenTwoFingerDragAndTapGesture::handleInputEvent(libinput_event *event)
{
    switch (libinput_event_get_type(event)) {
    case LIBINPUT_EVENT_TOUCH_DOWN: {
        m_currentFingerCount++;
        if (m_currentFingerCount > 2) {
            m_isCancelled = true;
            return Cancelled;
        }
        switch (m_currentFingerCount) {
        case 1: {
            auto touch_event = libinput_event_get_touch_event(event);
            auto xmm = libinput_event_touch_get_x(touch_event);
            auto ymm = libinput_event_touch_get_y(touch_event);
            m_firstPoint = QPointF(xmm, ymm);
            break;
        }
        case 2: {
            auto touch_event = libinput_event_get_touch_event(event);
            auto xmm = libinput_event_touch_get_x(touch_event);
            auto ymm = libinput_event_touch_get_y(touch_event);
            m_secondPoint = QPointF(xmm, ymm);

            m_lastSecondFingerPressedTime = libinput_event_touch_get_time(touch_event);
            m_isStarted = true;
            return Maybe;
            break;
        }
        default:
            break;
        }
        break;
    }
    case LIBINPUT_EVENT_TOUCH_MOTION: {
        if (m_isCancelled)
            return Ignore;
        auto touch_event = libinput_event_get_touch_event(event);
        auto xmm = libinput_event_touch_get_x(touch_event);
        auto ymm = libinput_event_touch_get_y(touch_event);
        auto current_slot = libinput_event_touch_get_slot(touch_event);
        if (current_slot == 0)
            m_firstPoint = QPointF(xmm, ymm);
        break;
    }
    case LIBINPUT_EVENT_TOUCH_UP: {
        m_currentFingerCount--;
        if (m_isCancelled) {
            return Ignore;
        }
        switch (m_currentFingerCount) {
        case 1: {
            auto touch_event = libinput_event_get_touch_event(event);
            auto timeInterval = libinput_event_touch_get_time(touch_event) - m_lastSecondFingerPressedTime;
            auto distance = (m_secondPoint - m_firstPoint).manhattanLength();
            if (timeInterval < 300 && distance < 50 && !m_isCancelled) {
                emit gestureUpdate(getGestureIndex());
                return Update;
            }
            break;
        }
        case 0: {
            if (m_isCancelled) {
                return Ignore;
            }
            if (m_isStarted) {
                emit gestureFinished(getGestureIndex());
                return Finished;
            }
            break;
        }
        default:
            break;
        }
    }
    default:
        break;
    }

    return Ignore;
}

void TouchScreenTwoFingerDragAndTapGesture::reset()
{
    m_firstPoint = QPointF();
    m_secondPoint = QPointF();

    m_isCancelled = false;
    m_isStarted = false;

    m_lastSecondFingerPressedTime = 0;
}

void TouchScreenTwoFingerDragAndTapGesture::cancel()
{
    m_isCancelled = true;
    emit gestureCancelled(getGestureIndex());
}

bool TouchScreenTwoFingerDragAndTapGesture::isCancelled()
{
    return m_isCancelled;
}
