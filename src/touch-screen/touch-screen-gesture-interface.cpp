#include "touch-screen-gesture-interface.h"

#include "touch-screen-gesture-manager.h"

TouchScreenGestureInterface::TouchScreenGestureInterface(QObject *parent) : QObject(parent)
{
    TouchScreenGestureManager::getManager()->registerGesuture(this);
}

int TouchScreenGestureInterface::getGestureIndex()
{
    return TouchScreenGestureManager::getManager()->queryGestureIndex(this);
}
