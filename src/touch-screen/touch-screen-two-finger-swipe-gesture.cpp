#include "touch-screen-two-finger-swipe-gesture.h"

//#include <QDebug>

TouchScreenTwoFingerSwipeGesture::TouchScreenTwoFingerSwipeGesture(QObject *parent)
{

}

TouchScreenGestureInterface::State TouchScreenTwoFingerSwipeGesture::handleInputEvent(libinput_event *event)
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
                //qDebug()<<m_startPoints[i]<<m_currentPoints[i];
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
        } else {
            //qDebug()<<"motion"<<m_startPoints[0]<<m_currentPoints[0];
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
        auto last_center_points = (m_lastPoints[0] + m_lastPoints[1])/2;
        auto current_center_points = (m_currentPoints[0] + m_currentPoints[1])/2;
        auto delta = current_center_points - last_center_points;
        auto offset = delta.manhattanLength();

        //qDebug()<<"offset"<<offset;

        if (offset < 20) {
            return Ignore;
        }

        for (int i = 0; i < 2; i++) {
            m_lastPoints[i] = m_currentPoints[i];
        }

        if (qAbs(delta.x()) > qAbs(delta.y())) {
            m_lastDirection = delta.x() > 0? Right: Left;
        } else {
            m_lastDirection = delta.y() > 0? Down: Up;
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

void TouchScreenTwoFingerSwipeGesture::reset()
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

TouchScreenGestureInterface::Direction TouchScreenTwoFingerSwipeGesture::totalDirection()
{
    // count total offset
    auto start_center_points = (m_startPoints[0] + m_startPoints[1])/2;
    auto current_center_points = (m_currentPoints[0] + m_currentPoints[1])/2;
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

TouchScreenGestureInterface::Direction TouchScreenTwoFingerSwipeGesture::lastDirection()
{
    return m_lastDirection;
}

void TouchScreenTwoFingerSwipeGesture::cancel()
{
    m_isCancelled = true;
    emit gestureCancelled(getGestureIndex());
}
