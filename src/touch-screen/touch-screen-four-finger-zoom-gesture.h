#ifndef TOUCHSCREENFOURFINGERZOOMGESTURE_H
#define TOUCHSCREENFOURFINGERZOOMGESTURE_H

#include "touch-screen-gesture-interface.h"

#include <QPointF>

class TouchScreenFourFingerZoomGesture : public TouchScreenGestureInterface
{
public:
    explicit TouchScreenFourFingerZoomGesture(QObject *parent = nullptr);

    int finger() override {return 4;}

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

    QPointF m_startPoints[4];
    QPointF m_lastPoints[4];
    QPointF m_currentPoints[4];
};

#endif // TOUCHSCREENFOURFINGERZOOMGESTURE_H
