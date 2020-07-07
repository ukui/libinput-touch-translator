#ifndef EVENTMONITOR_H
#define EVENTMONITOR_H

#include <QObject>

#include <libinput.h>

class EventMonitor : public QObject
{
    Q_OBJECT
public:
    enum ActionType {
        UNKNOW,
        FINGER_SWIPE,
        FINGER_ZOOM
    };
    Q_ENUM(ActionType)

    explicit EventMonitor(QObject *parent = nullptr);

    void startMonitor();

signals:
    void touchscreenGestureRequest(int fingerCount, ActionType type);
    void touchpadGestureRequest(int fingerCount, ActionType type);

private:
    libinput *m_input;
};

#endif // EVENTMONITOR_H
