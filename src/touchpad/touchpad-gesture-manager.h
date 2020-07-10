#ifndef TOUCHPADGESTUREMANAGER_H
#define TOUCHPADGESTUREMANAGER_H

#include <QObject>

#include <libinput.h>

class TouchpadGestureManager : public QObject
{
    Q_OBJECT
public:
    enum GestureType {
        Swipe,
        Pinch
    };
    Q_ENUM(GestureType)

    enum State {
        Begin,
        Update,
        Cancelled,
        Finished
    };
    Q_ENUM(State)

    enum Direction {
        None,
        Left,
        Right,
        Up,
        Down,
        ZoomIn,
        ZoomOut
    };
    Q_ENUM(Direction)

    static TouchpadGestureManager *getManager();

signals:
    void eventTriggered(GestureType type, int fingerCount, State state, Direction direction);

public slots:
    void processEvent(libinput_event *event);
    void reset();

    void onEventTriggerd(GestureType type, int fingerCount, State state, Direction direction);

private:
    explicit TouchpadGestureManager(QObject *parent = nullptr);

    int m_lastFinger = 0;
    bool m_isCancelled = 0;

    double m_totalDxmm = 0;
    double m_totalDymm = 0;

    double m_totalScale = 0;
    double m_totalAngle = 0;
};

#endif // TOUCHPADGESTUREMANAGER_H
