#ifndef TOUCHSCREENTWOFINGERZOOMGESTURE_H
#define TOUCHSCREENTWOFINGERZOOMGESTURE_H

#include "touch-screen-gesture-interface.h"

#include <QPointF>

class TouchScreenTwoFingerZoomGesture : public TouchScreenGestureInterface
{
public:
    explicit TouchScreenTwoFingerZoomGesture(QObject *parent = nullptr);

    int finger() override {return 2;}

    GestureType type() override {return Zoom;}

    State handleInputEvent(libinput_event *event) override;

    void reset() override;

    Direction totalDirection() override;

    Direction lastDirection() override;

    void cancel() override;

    bool isCancelled() override {return m_isCancelled;}

private:
    int m_currentFingerCount = 0;

    bool m_isCancelled = false;
    bool m_isStarted = false;

    Direction m_lastDirection = None;

    QPointF m_startPoints[2];
    QPointF m_lastPoints[2];
    QPointF m_currentPoints[2];
};

#endif // TOUCHSCREENTWOFINGERZOOMGESTURE_H
