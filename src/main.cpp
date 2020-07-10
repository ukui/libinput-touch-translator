#include <QCoreApplication>

#include "event-monitor.h"

#include "touch-screen/touch-screen-gesture-manager.h"
#include "touch-screen/touch-screen-three-finger-swipe-gesture.h"
#include "touch-screen/touch-screen-four-finger-swipe-gesture.h"
#include "touch-screen/touch-screen-five-finger-swipe-gesture.h"
#include "touch-screen/touch-screen-three-finger-zoom-gesture.h"
#include "touch-screen/touch-screen-four-finger-zoom-gesture.h"
#include "touch-screen/touch-screen-five-finger-zoom-gesture.h"
#include "touch-screen/touch-screen-two-finger-tap-gesture.h"

#include "settings-manager.h"
#include "uinput-helper.h"

#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThread t1;

    // init manager
    auto manager = TouchScreenGestureManager::getManager();
    SettingsManager::getManager();

    UInputHelper::getInstance();

    // init gesutre and register into gesture manager
    TouchScreenThreeFingerSwipeGesture *threeFingerSwipe = new TouchScreenThreeFingerSwipeGesture(manager);
    TouchScreenFourFingerSwipeGesture *fourFingerSwipe = new TouchScreenFourFingerSwipeGesture(manager);
    TouchScreenFiveFingerSwipeGesture *fiveFingerSwipe = new TouchScreenFiveFingerSwipeGesture(manager);
    TouchScreenThreeFingerZoomGesture *threeFingerZoom = new TouchScreenThreeFingerZoomGesture(manager);
    TouchScreenFourFingerZoomGesture *fourFingerZoom = new TouchScreenFourFingerZoomGesture(manager);
    TouchScreenFiveFingerZoomGesture *fiveFingerZoom = new TouchScreenFiveFingerZoomGesture(manager);
    TouchScreenTwoFingerTapGesture *twoFingerTap = new TouchScreenTwoFingerTapGesture(manager);

    EventMonitor em;
    em.initTouchScreenGestureManager(manager);
    em.moveToThread(&t1);

    t1.connect(&t1, &QThread::started, &em, &EventMonitor::startMonitor);
    t1.start();

    return a.exec();
}
