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

#include "touch-screen-gesture-manager.h"

#include "settings-manager.h"
#include "uinput-helper.h"

#include "touch-screen-two-finger-swipe-gesture.h"

#include <KWindowSystem>

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

int TouchScreenGestureManager::queryGestureIndex(TouchScreenGestureInterface::GestureType type, int fingerCount)
{
    for (auto gesture : m_gestures) {
        if (gesture->type() == type && fingerCount == gesture->finger())
            return m_gestures.indexOf(gesture);
    }

    return -1;
}

void TouchScreenGestureManager::processEvent(libinput_event *event)
{
    for (auto gesture : m_gestures) {
        auto state = gesture->handleInputEvent(event);
        //qDebug()<<gesture->finger()<<state;
    }
}

void TouchScreenGestureManager::forceReset()
{
    for (auto gesture : m_gestures) {
        gesture->reset();
    }
}

void TouchScreenGestureManager::initIgnoreHash()
{
    int twoFingerZoom = queryGestureIndex(TouchScreenGestureInterface::Zoom, 2);
    m_ignoreHash.insert(twoFingerZoom, "google-chrome");
    m_ignoreHash.insert(twoFingerZoom, "browser360-cn");
}

bool TouchScreenGestureManager::shouldEmulate(int gestureIndex, const QString &name)
{
    if (!UInputHelper::isPlatformX11())
        return true;

    qDebug()<<m_ignoreHash.values(gestureIndex);

    QStringList l = m_ignoreHash.values(gestureIndex);
    if (l.isEmpty())
        return true;

    for (QString content : l) {
        if (content.contains(name))
            return false;
        if (content.toLower().contains(name.toLower()))
            return false;
    }

    return true;
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
        if (gesture->finger() == 2) {
            if (UInputHelper::isPlatformX11()) {
                auto window = KWindowSystem::activeWindow();
                auto info = KWindowInfo(window, NET::WMAllProperties, NET::WM2AllProperties);
                qDebug()<<"name"<<info.name()<<"windowClassName"<<info.windowClassName()<<"windowClassClass"<<info.windowClassClass()<<"===";
                if (!shouldEmulate(index, info.windowClassName())) {
                    qDebug()<<"ignore"<<info.windowClassName();
                    return;
                }
            }
            UInputHelper::getInstance()->executeShortCut(gesture->lastDirection() == TouchScreenGestureInterface::ZoomIn? QKeySequence("Ctrl++"): QKeySequence("Ctrl+-"));
        }
    } else {
        if (gesture->finger() == 2) {
            if (gesture->type() == TouchScreenGestureInterface::Swipe) {
                auto twoFingerSwipe = static_cast<TouchScreenTwoFingerSwipeGesture *>(gesture);
                auto offset = twoFingerSwipe->getLastOffset();
                UInputHelper::getInstance()->wheel(offset/10);
            }
        }
    }

    if (gesture->type() == TouchScreenGestureInterface::Swipe || gesture->type() == TouchScreenGestureInterface::Zoom) {
        // cancel drag and tap gesture
        for (auto gesture : m_gestures) {
            if (gesture->type() == TouchScreenGestureInterface::DragAndTap) {
                gesture->cancel();
            }
        }
    }

    if (gesture->type() == TouchScreenGestureInterface::DragAndTap) {
        UInputHelper::getInstance()->clickMouseRightButton();
    }
}

void TouchScreenGestureManager::onGestureCancelled(int index)
{

}

void TouchScreenGestureManager::onGestureFinished(int index)
{
    auto gesture = m_gestures.at(index);
    qDebug()<<gesture->finger()<<"finger"<<gesture->type()<<"finished, total direction:"<<gesture->totalDirection();

    if (gesture->type() == TouchScreenGestureInterface::Tap) {
        if (gesture->finger() == 2)
            UInputHelper::getInstance()->clickMouseRightButton();
    } else {
        auto settingsManager = SettingsManager::getManager();
        auto shortCut = settingsManager->getShortCut(gesture, TouchScreenGestureInterface::Finished, gesture->totalDirection());
        qDebug()<<shortCut;

        UInputHelper::getInstance()->executeShortCut(shortCut);
    }

    // reset all gesture
    for (auto gesture : m_gestures) {
        gesture->reset();
    }
}
