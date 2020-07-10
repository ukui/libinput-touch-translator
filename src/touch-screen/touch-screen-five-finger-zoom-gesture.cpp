#include "touch-screen-five-finger-zoom-gesture.h"

TouchScreenFiveFingerZoomGesture::TouchScreenFiveFingerZoomGesture(QObject *parent) : TouchScreenGestureInterface(parent)
{

}

TouchScreenGestureInterface::State TouchScreenFiveFingerZoomGesture::handleInputEvent(libinput_event *event)
{
    switch (libinput_event_get_type(event)) {
    case LIBINPUT_EVENT_TOUCH_DOWN: {
        m_current_finger_count++;
        if (m_isCancelled)
            return Ignore;
        auto touch_event = libinput_event_get_touch_event(event);

        int current_finger_count = m_current_finger_count;
        //qDebug()<<"current finger count:"<<current_finger_count;
        int current_slot = libinput_event_touch_get_slot(touch_event);

        double mmx = libinput_event_touch_get_x(touch_event);
        double mmy = libinput_event_touch_get_y(touch_event);

        if (current_finger_count <= 5) {
            m_startPoints[current_slot] = QPointF(mmx, mmy);
        }

        if (current_finger_count == 5) {
            // start the gesture
            m_isStarted = true;
            for (int i = 0; i < 5; i++) {
                m_lastPoints[i] = m_startPoints[i];
                m_currentPoints[i] = m_startPoints[i];
            }
            emit gestureBegin(getGestureIndex());
            return Maybe;
        }

        if (current_finger_count > 5) {
            m_isCancelled = true;
            emit gestureCancelled(getGestureIndex());
            return Cancelled;
        }
        break;
    }
    case LIBINPUT_EVENT_TOUCH_MOTION: {
        if (m_isCancelled)
            return Ignore;

        if (!m_isStarted) {
            return Ignore;
        }

        // update position
        auto touch_event = libinput_event_get_touch_event(event);

        int current_slot = libinput_event_touch_get_slot(touch_event);

        double mmx = libinput_event_touch_get_x(touch_event);
        double mmy = libinput_event_touch_get_y(touch_event);

        m_currentPoints[current_slot] = QPointF(mmx, mmy);
        break;
    }
    case LIBINPUT_EVENT_TOUCH_UP: {
        m_current_finger_count--;
        int current_finger_count = m_current_finger_count;

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

        // update gesture

        // count offset
        auto last_distance01 = (m_lastPoints[0] - m_lastPoints[1]).manhattanLength();
        auto last_distance02 = (m_lastPoints[0] - m_lastPoints[2]).manhattanLength();
        auto last_distance03 = (m_lastPoints[0] - m_lastPoints[3]).manhattanLength();
        auto last_distance04 = (m_lastPoints[0] - m_lastPoints[4]).manhattanLength();
        auto current_distance01 = (m_currentPoints[0] - m_currentPoints[1]).manhattanLength();
        auto current_distance02 = (m_currentPoints[0] - m_currentPoints[2]).manhattanLength();
        auto current_distance03 = (m_currentPoints[0] - m_currentPoints[3]).manhattanLength();
        auto current_distance04 = (m_currentPoints[0] - m_currentPoints[4]).manhattanLength();

        auto last_distance = (last_distance01 + last_distance02 + last_distance03 + last_distance04)/4;
        auto current_distance = (current_distance01 + current_distance02 + current_distance03 + current_distance04)/4;
        auto delta = current_distance - last_distance;
        if (qAbs(delta) < 25) {
            return Ignore;
        }

        for (int i = 0; i < 5; i++) {
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

void TouchScreenFiveFingerZoomGesture::reset()
{
    m_isCancelled = false;
    m_isStarted = false;
    m_lastDirection = None;

    for (int i = 0; i < 5; i++) {
        m_startPoints[i] = QPointF();
        m_lastPoints[i] = QPointF();
        m_currentPoints[i] = QPointF();
    }
}

TouchScreenGestureInterface::Direction TouchScreenFiveFingerZoomGesture::totalDirection()
{
    // count offset
    auto start_distance01 = (m_startPoints[0] - m_startPoints[1]).manhattanLength();
    auto start_distance02 = (m_startPoints[0] - m_startPoints[2]).manhattanLength();
    auto start_distance03 = (m_startPoints[0] - m_startPoints[3]).manhattanLength();
    auto start_distance04 = (m_startPoints[0] - m_startPoints[4]).manhattanLength();
    auto current_distance01 = (m_currentPoints[0] - m_currentPoints[1]).manhattanLength();
    auto current_distance02 = (m_currentPoints[0] - m_currentPoints[2]).manhattanLength();
    auto current_distance03 = (m_currentPoints[0] - m_currentPoints[3]).manhattanLength();
    auto current_distance04 = (m_currentPoints[0] - m_currentPoints[4]).manhattanLength();

    auto start_distance = (start_distance01 + start_distance02 + start_distance03 + start_distance04)/4;
    auto current_distance = (current_distance01 + current_distance02 + current_distance03 + current_distance04)/4;
    auto delta = current_distance - start_distance;

    if (delta > 25) {
        return ZoomIn;
    } else if (delta < -25) {
        return ZoomOut;
    }

    return None;
}

TouchScreenGestureInterface::Direction TouchScreenFiveFingerZoomGesture::lastDirection()
{
    return m_lastDirection;
}

void TouchScreenFiveFingerZoomGesture::cancel()
{
    // nothing to do
}
