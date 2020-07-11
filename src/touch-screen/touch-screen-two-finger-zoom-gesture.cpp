#include "touch-screen-two-finger-zoom-gesture.h"

TouchScreenTwoFingerZoomGesture::TouchScreenTwoFingerZoomGesture(QObject *parent) : TouchScreenGestureInterface(parent)
{

}

TouchScreenGestureInterface::State TouchScreenTwoFingerZoomGesture::handleInputEvent(libinput_event *event)
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
            m_isStarted = true;
            for (int i = 0; i < 2; i++) {
                m_lastPoints[i] = m_startPoints[i];
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

        if (!m_isStarted) {
            m_startPoints[current_slot] = m_currentPoints[current_slot];
        }
        break;
    }
    case LIBINPUT_EVENT_TOUCH_UP: {
        m_currentFingerCount--;
        //m_isCancelled = true;
        int current_finger_count = m_currentFingerCount;

        if (current_finger_count <= 0) {
            if (!m_isCancelled && m_isStarted && m_lastDirection != None) {
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
        if (m_isCancelled || !m_isStarted)
            return Ignore;

        if (m_currentFingerCount != 2)
            return Ignore;

        // update gesture

        // count offset
        auto last_distance = (m_lastPoints[0] - m_lastPoints[1]).manhattanLength();
        auto current_distance = (m_currentPoints[0] - m_currentPoints[1]).manhattanLength();
        auto delta = current_distance - last_distance;
        if (qAbs(delta) < 15) {
            return Ignore;
        }

        for (int i = 0; i < 2; i++) {
            m_lastPoints[i] = m_currentPoints[i];
        }

        if (delta > 0) {
            m_lastDirection = ZoomIn;
        } else {
            m_lastDirection = ZoomOut;
        }

        emit gestureUpdate(getGestureIndex());

        return Update;

        break;
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

void TouchScreenTwoFingerZoomGesture::reset()
{
    m_isCancelled = false;
    m_isStarted = false;
    m_lastDirection = None;

    for (int i = 0; i < 2; i++) {
        m_startPoints[i] = QPointF();
        m_lastPoints[i] = QPointF();
        m_currentPoints[i] = QPointF();
    }
}

TouchScreenGestureInterface::Direction TouchScreenTwoFingerZoomGesture::totalDirection()
{
    // count offset
    auto start_distance = (m_startPoints[0] - m_startPoints[1]).manhattanLength();
    auto current_distance = (m_currentPoints[0] - m_currentPoints[1]).manhattanLength();
    auto delta = current_distance - start_distance;

    if (delta > 15) {
        return ZoomIn;
    } else if (delta < -15) {
        return ZoomOut;
    }

    return None;
}

TouchScreenGestureInterface::Direction TouchScreenTwoFingerZoomGesture::lastDirection()
{
    return m_lastDirection;
}

void TouchScreenTwoFingerZoomGesture::cancel()
{

}
