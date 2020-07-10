#ifndef TOUCHSCREENFOURFINGERSWIPEGESTURE_H
#define TOUCHSCREENFOURFINGERSWIPEGESTURE_H

#include "touch-screen/touch-screen-gesture-interface.h"

#include <QPointF>

class TouchScreenFourFingerSwipeGesture : public TouchScreenGestureInterface
{
public:
    explicit TouchScreenFourFingerSwipeGesture(QObject *parent = nullptr);

    int finger() override {return 4;}

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

    QPointF m_startPoints[4];
    QPointF m_lastPoints[4];
    QPointF m_currentPoints[4];
};

#endif // TOUCHSCREENFOURFINGERSWIPEGESTURE_H
