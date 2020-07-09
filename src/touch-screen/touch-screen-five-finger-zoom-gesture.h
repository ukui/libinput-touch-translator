#ifndef TOUCHSCREENFIVEFINGERZOOMGESTURE_H
#define TOUCHSCREENFIVEFINGERZOOMGESTURE_H

#include "touch-screen-gesture-interface.h"

#include <QPointF>

class TouchScreenFiveFingerZoomGesture : public TouchScreenGestureInterface
{
public:
    explicit TouchScreenFiveFingerZoomGesture(QObject *parent = nullptr);

    int finger() override {return 5;}

    GestureType type() override {return Zoom;}

    State handleInputEvent(libinput_event *event) override;

    void reset() override;

    Direction totalDirection() override;

    Direction lastDirection() override;

    void cancel() override;

    bool isCancelled() override {return m_is_cancelled;}

private:
    int m_current_finger_count = 0;

    bool m_is_cancelled = false;
    bool m_is_started = false;

    Direction m_last_direction = None;

    QPointF m_start_points[5];
    QPointF m_last_points[5];
    QPointF m_current_points[5];
};

#endif // TOUCHSCREENFIVEFINGERZOOMGESTURE_H
