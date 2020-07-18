/*
 * Libinput Touch Translator
 *
 * Copyright (C) 2020, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Authors: Yue Lan <lanyue@kylinos.cn>
 *
 */

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
#include "touch-screen/touch-screen-two-finger-swipe-gesture.h"
#include "touch-screen/touch-screen-two-finger-zoom-gesture.h"
#include "touch-screen/touch-screen-two-finger-drag-and-tap-gesture.h"

#include "touchpad/touchpad-gesture-manager.h"

#include "settings-manager.h"
#include "uinput-helper.h"

#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThread t1;

    // init manager
    auto manager = TouchScreenGestureManager::getManager();
    TouchpadGestureManager::getManager();
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
    TouchScreenTwoFingerSwipeGesture *twoFingerSwipe = new TouchScreenTwoFingerSwipeGesture(manager);
    TouchScreenTwoFingerZoomGesture *twoFingerZoom = new TouchScreenTwoFingerZoomGesture(manager);
    TouchScreenTwoFingerDragAndTapGesture *twoFingerDragAndTap = new TouchScreenTwoFingerDragAndTapGesture(manager);

    EventMonitor em;
    em.initTouchScreenGestureManager(manager);
    em.moveToThread(&t1);

    t1.connect(&t1, &QThread::started, &em, &EventMonitor::startMonitor);
    t1.start();

    return a.exec();
}
