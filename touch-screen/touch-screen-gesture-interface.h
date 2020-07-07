#ifndef TOUCHSCREENGESTUREINTERFACE_H
#define TOUCHSCREENGESTUREINTERFACE_H

#include <QObject>

#include <libinput.h>

class TouchScreenGestureInterface : public QObject
{
    Q_OBJECT
public:
    enum GestureType {
        Unknow,
        Swipe,
        Zoom
    };

    enum State {
        Ignore,
        Maybe,
        Update,
        Cancelled,
        Pending,  // gesture action will real execute when all finger released.
        Finished
    };

    explicit TouchScreenGestureInterface(QObject *parent = nullptr);

    virtual int finger() = 0;

    virtual GestureType type() = 0;

    virtual State handleInputEvent(libinput_event *event) {return Ignore;}

    virtual void reset() {}

signals:
    void gestureBegin(int registedIndex);
    void gestureUpdate(int registedIndex);
    void gestureCancelled(int registedIndex);
    void gestureFinished(int registedIndex);
};

#endif // TOUCHSCREENGESTUREINTERFACE_H
