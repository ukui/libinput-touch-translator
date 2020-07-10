#ifndef TOUCHSCREENTHREEFINGERSWIPEGESTURE_H
#define TOUCHSCREENTHREEFINGERSWIPEGESTURE_H

#include "touch-screen-gesture-interface.h"

#include <QPointF>

class TouchScreenThreeFingerSwipeGesture : public TouchScreenGestureInterface
{
public:
    explicit TouchScreenThreeFingerSwipeGesture(QObject *parent = nullptr);

    int finger() override {return 3;}

    GestureType type() override {return Swipe;}

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

    QPointF m_startPoints[3];
    QPointF m_lastPoints[3];
    QPointF m_currentPoints[3];
};

#endif // TOUCHSCREENTHREEFINGERSWIPEGESTURE_H
