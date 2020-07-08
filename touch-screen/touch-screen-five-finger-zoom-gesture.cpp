#include "touch-screen-five-finger-zoom-gesture.h"

TouchScreenFiveFingerZoomGesture::TouchScreenFiveFingerZoomGesture(QObject *parent) : TouchScreenGestureInterface(parent)
{

}

TouchScreenGestureInterface::State TouchScreenFiveFingerZoomGesture::handleInputEvent(libinput_event *event)
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
        auto last_distance01 = (m_last_points[0] - m_last_points[1]).manhattanLength();
        auto last_distance02 = (m_last_points[0] - m_last_points[2]).manhattanLength();
        auto last_distance03 = (m_last_points[0] - m_last_points[3]).manhattanLength();
        auto last_distance04 = (m_last_points[0] - m_last_points[4]).manhattanLength();
        auto current_distance01 = (m_current_points[0] - m_current_points[1]).manhattanLength();
        auto current_distance02 = (m_current_points[0] - m_current_points[2]).manhattanLength();
        auto current_distance03 = (m_current_points[0] - m_current_points[3]).manhattanLength();
        auto current_distance04 = (m_current_points[0] - m_current_points[4]).manhattanLength();

        auto last_distance = (last_distance01 + last_distance02 + last_distance03 + last_distance04)/4;
        auto current_distance = (current_distance01 + current_distance02 + current_distance03 + current_distance04)/4;
        auto delta = current_distance - last_distance;
        if (qAbs(delta) < 20) {
            return Ignore;
        }

        for (int i = 0; i < 5; i++) {
            m_last_points[i] = m_current_points[i];
        }

        if (delta > 0) {
            m_last_direction = ZoomIn;
        } else {
            m_last_direction = ZoomOut;
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

void TouchScreenFiveFingerZoomGesture::reset()
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

TouchScreenGestureInterface::Direction TouchScreenFiveFingerZoomGesture::totalDirection()
{
    // count offset
    auto start_distance01 = (m_start_points[0] - m_start_points[1]).manhattanLength();
    auto start_distance02 = (m_start_points[0] - m_start_points[2]).manhattanLength();
    auto start_distance03 = (m_start_points[0] - m_start_points[3]).manhattanLength();
    auto start_distance04 = (m_start_points[0] - m_start_points[4]).manhattanLength();
    auto current_distance01 = (m_current_points[0] - m_current_points[1]).manhattanLength();
    auto current_distance02 = (m_current_points[0] - m_current_points[2]).manhattanLength();
    auto current_distance03 = (m_current_points[0] - m_current_points[3]).manhattanLength();
    auto current_distance04 = (m_current_points[0] - m_current_points[4]).manhattanLength();

    auto start_distance = (start_distance01 + start_distance02 + start_distance03 + start_distance04)/4;
    auto current_distance = (current_distance01 + current_distance02 + current_distance03 + current_distance04)/4;
    auto delta = current_distance - start_distance;

    if (delta > 20) {
        return ZoomIn;
    } else if (delta < -20) {
        return ZoomOut;
    }

    return None;
}

TouchScreenGestureInterface::Direction TouchScreenFiveFingerZoomGesture::lastDirection()
{
    return m_last_direction;
}

void TouchScreenFiveFingerZoomGesture::cancel()
{
    // nothing to do
}
