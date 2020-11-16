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

#include "touch-screen-one-finger-edge-gesture.h"
#include <QtMath>

#include <QDebug>

TouchScreenOneFingerEdgeGesture::TouchScreenOneFingerEdgeGesture(QObject *parent) : TouchScreenGestureInterface(parent)
{

}

TouchScreenGestureInterface::State TouchScreenOneFingerEdgeGesture::handleInputEvent(libinput_event *event)
{
    // do not handle edge gesture.
    return Ignore;
    switch (libinput_event_get_type(event)) {
    case LIBINPUT_EVENT_TOUCH_DOWN: {
        m_fingerCount++;
        if (m_fingerCount > 1) {
            cancel();
            return Cancelled;
        }

        if (isCancelled())
            return Ignore;

        auto touch_event = libinput_event_get_touch_event(event);
        double nx = libinput_event_touch_get_x_transformed(touch_event, 100);
        double ny = libinput_event_touch_get_y_transformed(touch_event, 100);

        if (nx < 1) {
            m_direction = Left;
        }
        if (nx > 99) {
            m_direction = Right;
        }
        if (ny < 1) {
            m_direction = Up;
        }
        if (ny > 99) {
            m_direction = Down;
        }

        if (m_direction == None) {
            return Ignore;
        }

        double mmx = libinput_event_touch_get_x(touch_event);
        double mmy = libinput_event_touch_get_y(touch_event);

        m_startPoint = QPointF(mmx, mmy);
        m_lastPoint = m_startPoint;
        m_currentPoint = m_startPoint;

        if (isCancelled()) {
            return Ignore;
        }
        break;
    }
    case LIBINPUT_EVENT_TOUCH_MOTION: {
        if (m_direction == None) {
            return Ignore;
        }

        if (!m_isCancelled) {
            auto touch_event = libinput_event_get_touch_event(event);
            double nx = libinput_event_touch_get_x_transformed(touch_event, 100);
            double ny = libinput_event_touch_get_y_transformed(touch_event, 100);

            m_currentPoint = QPointF(nx, ny);
            auto delta = (m_lastPoint - m_currentPoint).manhattanLength();
            if (delta > 20) {
                auto offset = m_currentPoint - m_lastPoint;
                switch (m_direction) {
                case Left: {
                    qDebug()<<m_currentPoint;
                    if (offset.x() > 0) {
                        m_lastPoint = m_currentPoint;
                        gestureUpdate(getGestureIndex());
                        qDebug()<<m_lastPoint;
                        return Update;
                    } else if (offset.x() < -10) {
                        cancel();
                        return Cancelled;
                    }
                    break;
                }
                case Right : {
                    if (offset.x() < 0) {
                        m_lastPoint = m_currentPoint;
                        gestureUpdate(getGestureIndex());
                        return Update;
                    } else if (offset.x() > 10) {
                        cancel();
                        return Cancelled;
                    }
                    break;
                }
                case Up: {
                    if (offset.y() > 0) {
                        m_lastPoint = m_currentPoint;
                        gestureUpdate(getGestureIndex());
                        return Update;
                    } else if (offset.y() < -10) {
                        cancel();
                        return Cancelled;
                    }
                    break;
                }
                case Down: {
                    if (offset.y() < 0) {
                        m_lastPoint = m_currentPoint;
                        gestureUpdate(getGestureIndex());
                        return Update;
                    } else if (offset.y() > 10) {
                        cancel();
                        return Cancelled;
                    }
                    break;
                }
                case None:
                    return Ignore;
                default:
                    break;
                }
            }
        }
        break;
    }
    case LIBINPUT_EVENT_TOUCH_UP: {
        m_fingerCount--;
        if (m_fingerCount == 0) {
            if (!m_isCancelled) {
                if (m_direction == None) {
                    return Ignore;
                }
                if (m_direction == Down) {
                    if (longestDistance() < 80) {
                        reset();
                        return Ignore;
                    }
                }
                gestureFinished(getGestureIndex());
                qDebug()<<"longestDistance"<<longestDistance();
                return Finished;
            } else {
                reset();
            }
        }
        break;
    }
    case LIBINPUT_EVENT_TOUCH_CANCEL: {
        cancel();
        return Cancelled;
    }
    default:
        break;
    }

    return Ignore;
}

TouchScreenGestureInterface::Direction TouchScreenOneFingerEdgeGesture::totalDirection()
{
    if (isCancelled())
        return None;
    return m_direction;
}

TouchScreenGestureInterface::Direction TouchScreenOneFingerEdgeGesture::lastDirection()
{
    if (isCancelled())
        return None;
    return m_direction;
}

void TouchScreenOneFingerEdgeGesture::reset()
{
    m_isCancelled = false;
    m_fingerCount = 0;
    m_direction = None;
    m_startPoint = QPointF();
    m_lastPoint = QPointF();
    m_currentPoint = QPointF();
}

bool TouchScreenOneFingerEdgeGesture::isCancelled()
{
    return m_isCancelled;
}

void TouchScreenOneFingerEdgeGesture::cancel()
{
    m_isCancelled = true;
    m_direction = None;
    m_startPoint = QPointF();
    m_lastPoint = QPointF();
    m_currentPoint = QPointF();
    gestureCancelled(getGestureIndex());
}

int TouchScreenOneFingerEdgeGesture::longestDistance()
{
    switch (m_direction) {
    case Left:
    case Right: {
        return qAbs(m_currentPoint.x() - m_startPoint.x());
    }
    case Up:
    case Down: {
        return qAbs(m_currentPoint.y() - m_startPoint.y());
    }
    default:
        break;
    }

    return -1;
}
