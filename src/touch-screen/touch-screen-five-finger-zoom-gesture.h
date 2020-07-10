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

    bool isCancelled() override {return m_isCancelled;}

private:
    int m_current_finger_count = 0;

    bool m_isCancelled = false;
    bool m_isStarted = false;

    Direction m_lastDirection = None;

    QPointF m_startPoints[5];
    QPointF m_lastPoints[5];
    QPointF m_currentPoints[5];
};

#endif // TOUCHSCREENFIVEFINGERZOOMGESTURE_H
