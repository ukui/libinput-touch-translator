#ifndef TOUCHSCREENGESTUREMANAGER_H
#define TOUCHSCREENGESTUREMANAGER_H

#include <QObject>

#include <libinput.h>

class TouchScreenGestureInterface;

class TouchScreenGestureManager : public QObject
{
    Q_OBJECT
public:
    explicit TouchScreenGestureManager(QObject *parent = nullptr);

    int registerGesuture(TouchScreenGestureInterface *gesture); // return a index of registered gesture.

    void processEvent(libinput_event *event);
signals:

public slots:
    void onGestureBegin(int index);
    void onGestureUpdated(int index);
    void onGestureCancelled(int index);
    void onGestureFinished(int index);

private:
    QList<TouchScreenGestureInterface *> m_gestures;
};

#endif // TOUCHSCREENGESTUREMANAGER_H
