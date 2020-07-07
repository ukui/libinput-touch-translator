#include "touch-screen-gesture-manager.h"

#include "touch-screen-gesture-interface.h"

TouchScreenGestureManager::TouchScreenGestureManager(QObject *parent) : QObject(parent)
{

}

int TouchScreenGestureManager::registerGesuture(TouchScreenGestureInterface *gesture)
{
    m_gestures<<gesture;
    connect(gesture, &TouchScreenGestureInterface::gestureBegin, this, &TouchScreenGestureManager::onGestureBegin);
    connect(gesture, &TouchScreenGestureInterface::gestureUpdate, this, &TouchScreenGestureManager::onGestureUpdated);
    connect(gesture, &TouchScreenGestureInterface::gestureCancelled, this, &TouchScreenGestureManager::onGestureCancelled);
    connect(gesture, &TouchScreenGestureInterface::gestureFinished, this, &TouchScreenGestureManager::onGestureFinished);
    return m_gestures.indexOf(gesture);
}

void TouchScreenGestureManager::processEvent(libinput_event *event)
{
    for (auto gesture : m_gestures) {
        auto state = gesture->handleInputEvent(event);
    }
}

void TouchScreenGestureManager::onGestureBegin(int index)
{

}

void TouchScreenGestureManager::onGestureUpdated(int index)
{

}

void TouchScreenGestureManager::onGestureCancelled(int index)
{

}

void TouchScreenGestureManager::onGestureFinished(int index)
{

}
