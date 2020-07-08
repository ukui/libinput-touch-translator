#include "touch-screen-gesture-manager.h"

#include "touch-screen-gesture-interface.h"

#include <QDebug>

static TouchScreenGestureManager *instance = nullptr;

TouchScreenGestureManager::TouchScreenGestureManager(QObject *parent) : QObject(parent)
{

}

TouchScreenGestureManager *TouchScreenGestureManager::getManager()
{
    if (!instance)
        instance = new TouchScreenGestureManager;
    return instance;
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

int TouchScreenGestureManager::queryGestureIndex(TouchScreenGestureInterface *gesture)
{
    return m_gestures.indexOf(gesture);
}

void TouchScreenGestureManager::processEvent(libinput_event *event)
{
    for (auto gesture : m_gestures) {
        auto state = gesture->handleInputEvent(event);
        //qDebug()<<gesture->finger()<<state;
    }
}

void TouchScreenGestureManager::onGestureBegin(int index)
{

}

void TouchScreenGestureManager::onGestureUpdated(int index)
{
    auto gesture = m_gestures.at(index);
    qDebug()<<gesture->finger()<<"finger"<<gesture->type()<<"updated, current direction:"<<gesture->lastDirection();

    // cancel swipe gesture if any zoom gesture triggered.
    if (gesture->type() == TouchScreenGestureInterface::Zoom) {
        for (auto gesture : m_gestures) {
            if (gesture->type() == TouchScreenGestureInterface::Swipe) {
                gesture->cancel();
            }
        }
    }
}

void TouchScreenGestureManager::onGestureCancelled(int index)
{

}

void TouchScreenGestureManager::onGestureFinished(int index)
{
    auto gesture = m_gestures.at(index);
    qDebug()<<gesture->finger()<<"finger"<<gesture->type()<<"finished, total direction:"<<gesture->totalDirection();

    // reset all gesture
    for (auto gesture : m_gestures) {
        gesture->reset();
    }
}
