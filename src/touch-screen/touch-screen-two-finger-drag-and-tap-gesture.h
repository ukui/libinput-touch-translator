#ifndef TOUCHSCREENTWOFINGERDRAGANDTAPGESTURE_H
#define TOUCHSCREENTWOFINGERDRAGANDTAPGESTURE_H

#include "touch-screen-gesture-interface.h"

#include <QPointF>

class TouchScreenTwoFingerDragAndTapGesture : public TouchScreenGestureInterface
{
public:
    explicit TouchScreenTwoFingerDragAndTapGesture(QObject *parent = nullptr);

    int finger() override {return 2;}

    GestureType type() override {return DragAndTap;}

    State handleInputEvent(libinput_event *event) override;

    void reset() override;

    Direction totalDirection() override {return None;}

    Direction lastDirection() override {return None;}

    void cancel() override;

    bool isCancelled() override;

private:
    int m_currentFingerCount = 0;
    int m_lastSecondFingerPressedTime = 0;

    QPointF m_firstPoint;
    QPointF m_secondPoint;

    bool m_isCancelled = false;
    bool m_isStarted = false;
};

#endif // TOUCHSCREENTWOFINGERDRAGANDTAPGESTURE_H
