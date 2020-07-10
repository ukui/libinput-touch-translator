#ifndef TOUCHSCREENTWOFINGERTAPGESTURE_H
#define TOUCHSCREENTWOFINGERTAPGESTURE_H

#include "touch-screen-gesture-interface.h"

#include <QPointF>

class TouchScreenTwoFingerTapGesture : public TouchScreenGestureInterface
{
public:
    explicit TouchScreenTwoFingerTapGesture(QObject *parent = nullptr);

    int finger() override {return 2;}

    GestureType type() override {return Tap;}

    State handleInputEvent(libinput_event *event) override;

    void reset() override;

    Direction totalDirection() override;

    Direction lastDirection() override;

    void cancel() override;

    bool isCancelled() override;

private:
    int m_currentFingerCount = 0;

    int m_startTime = 0;
    int m_endTime = 0;

    bool m_isCancelled = false;

    QPointF m_startPoints[2];
    QPointF m_currentPoints[2];
};

#endif // TOUCHSCREENTWOFINGERTAPGESTURE_H
