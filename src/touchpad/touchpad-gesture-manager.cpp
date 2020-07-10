#include "touchpad-gesture-manager.h"

#include "settings-manager.h"
#include "uinput-helper.h"

#include <QDebug>

static TouchpadGestureManager *instance = nullptr;

TouchpadGestureManager *TouchpadGestureManager::getManager()
{
    if (!instance)
        instance = new TouchpadGestureManager;
    return instance;
}

void TouchpadGestureManager::processEvent(libinput_event *event)
{
    // Fixme:
    auto type = libinput_event_get_type(event);
    libinput_event_gesture *t = libinput_event_get_gesture_event(event);

    switch (type) {
    case LIBINPUT_EVENT_GESTURE_SWIPE_BEGIN: {
        reset();
        m_last_finger = libinput_event_gesture_get_finger_count(t);
        break;
    }
    case LIBINPUT_EVENT_GESTURE_SWIPE_UPDATE: {
        double dx = libinput_event_gesture_get_dx(t);
        double dy = libinput_event_gesture_get_dy(t);
        //double dx_unaccel = libinput_event_gesture_get_dx_unaccelerated(t);
        //double dy_unaccel = libinput_event_gesture_get_dy_unaccelerated(t);

        // total offset
        m_total_dxmm += dx;
        m_total_dymm += dy;
        break;
    }
    case LIBINPUT_EVENT_GESTURE_SWIPE_END: {
        m_is_cancelled = libinput_event_gesture_get_cancelled(t);
        m_last_finger = libinput_event_gesture_get_finger_count(t);
        if (!m_is_cancelled) {
            if (qAbs(m_total_dxmm) > qAbs(m_total_dymm)) {
                if (m_total_dxmm > 20) {
                    // right
                    emit eventTriggered(Swipe, m_last_finger, Finished, Right);
                } else if (m_total_dxmm < -20) {
                    // left
                    emit eventTriggered(Swipe, m_last_finger, Finished, Left);
                } else {
                    //none
                    emit eventTriggered(Swipe, m_last_finger, Finished, None);
                }
            } else {
                if (m_total_dymm > 20) {
                    // down
                    emit eventTriggered(Swipe, m_last_finger, Finished, Down);
                } else if (m_total_dymm < -20) {
                    // up
                    emit eventTriggered(Swipe, m_last_finger, Finished, Up);
                } else {
                    // none
                    emit eventTriggered(Swipe, m_last_finger, Finished, None);
                }
            }
        } else {
            // cancelled
            emit eventTriggered(Swipe, m_last_finger, Cancelled, None);
        }
        reset();
        break;
    }
    case LIBINPUT_EVENT_GESTURE_PINCH_BEGIN: {
        reset();
        m_last_finger = libinput_event_gesture_get_finger_count(t);
        break;
    }
    case LIBINPUT_EVENT_GESTURE_PINCH_UPDATE: {
        m_total_scale = libinput_event_gesture_get_scale(t);
        m_total_angle += libinput_event_gesture_get_angle_delta(t); // useless now
        break;
    }
    case LIBINPUT_EVENT_GESTURE_PINCH_END: {
        m_is_cancelled = libinput_event_gesture_get_cancelled(t);
        m_last_finger = libinput_event_gesture_get_finger_count(t);

        // some zoom out gesture is easy to be recognized as cancelled,
        // so take them into judgement.
        if (!m_is_cancelled || m_total_scale != 0) {
            if (m_total_scale > 1) {
                emit eventTriggered(Pinch, m_last_finger, Finished, ZoomIn);
            } else {
                emit eventTriggered(Pinch, m_last_finger, Finished, ZoomOut);
            }
        } else {
            emit eventTriggered(Pinch, m_last_finger, Cancelled, None);
        }
        reset();
        break;
    }
    default:
        break;
    }

}

void TouchpadGestureManager::reset()
{
    m_last_finger = 0;
    m_is_cancelled = 0;

    m_total_dxmm = 0;
    m_total_dymm = 0;

    m_total_scale = 0;
    m_total_angle = 0;
}

void TouchpadGestureManager::onEventTriggerd(TouchpadGestureManager::GestureType type, int fingerCount, TouchpadGestureManager::State state, TouchpadGestureManager::Direction direction)
{
    qDebug()<<type<<fingerCount<<state<<direction;
    qDebug()<<m_total_dxmm<<m_total_dymm<<m_total_angle<<m_total_scale;

    auto shortcut = SettingsManager::getManager()->gesShortCut(fingerCount, type, state, direction);
    UInputHelper::getInstance()->executeShortCut(shortcut);
}

TouchpadGestureManager::TouchpadGestureManager(QObject *parent) : QObject(parent)
{
    connect(this, &TouchpadGestureManager::eventTriggered, this, &TouchpadGestureManager::onEventTriggerd);
}
