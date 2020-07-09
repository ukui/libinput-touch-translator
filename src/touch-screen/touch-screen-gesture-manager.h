#ifndef TOUCHSCREENGESTUREMANAGER_H
#define TOUCHSCREENGESTUREMANAGER_H

#include <QObject>

#include <libinput.h>

class TouchScreenGestureInterface;

class TouchScreenGestureManager : public QObject
{
    friend class TouchScreenGestureInterface;
    Q_OBJECT
public:
    static TouchScreenGestureManager *getManager();

    int queryGestureIndex(TouchScreenGestureInterface *gesture);

    void processEvent(libinput_event *event);
signals:

public slots:
    void onGestureBegin(int index);
    void onGestureUpdated(int index);
    void onGestureCancelled(int index);
    void onGestureFinished(int index);

private:
    int registerGesuture(TouchScreenGestureInterface *gesture); // return a index of registered gesture.

    explicit TouchScreenGestureManager(QObject *parent = nullptr);
    QList<TouchScreenGestureInterface *> m_gestures;
};

#endif // TOUCHSCREENGESTUREMANAGER_H
