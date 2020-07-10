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

#include "settings-manager.h"

#include <QSettings>

#include <QFileSystemWatcher>

#include <QDebug>

static SettingsManager *instance = nullptr;

SettingsManager::SettingsManager(QObject *parent) : QObject(parent)
{
    QMetaEnum direction = QMetaEnum::fromType<TouchScreenGestureInterface::Direction>();
    QMetaEnum gestureType = QMetaEnum::fromType<TouchScreenGestureInterface::GestureType>();
    QMetaEnum gestureState = QMetaEnum::fromType<TouchScreenGestureInterface::State>();

    m_touchScreenGestureDirection = direction;
    m_touchScreenGestureState = gestureState;
    m_touchScreenGestureType = gestureType;

    m_touchpadGestureType = QMetaEnum::fromType<TouchpadGestureManager::GestureType>();
    m_touchpadGestureState = QMetaEnum::fromType<TouchpadGestureManager::State>();
    m_touchpadGestureDirection = QMetaEnum::fromType<TouchpadGestureManager::Direction>();

    m_settings = new QSettings(QSettings::SystemScope, "ukui", "gestures", this);
    qDebug()<<m_settings->fileName();

    QFileSystemWatcher *watcher = new QFileSystemWatcher(QStringList()<<m_settings->fileName(), this);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, [=](){
        qDebug()<<"file changed, sync";
        m_settings->sync();
        // we should rewatch the changed file.
        watcher->addPath(m_settings->fileName());
    });

    setToucScreenShortCut(TouchScreenGestureInterface::Swipe, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::Left, 3, QKeySequence("Alt+Left"));
    setToucScreenShortCut(TouchScreenGestureInterface::Swipe, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::Right, 3, QKeySequence("Alt+Right"));
    setToucScreenShortCut(TouchScreenGestureInterface::Swipe, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::Up, 3, QKeySequence("Ctrl+End"));
    setToucScreenShortCut(TouchScreenGestureInterface::Swipe, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::Down, 3, QKeySequence("Ctrl+Home"));
    setToucScreenShortCut(TouchScreenGestureInterface::Swipe, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::Left, 4, QKeySequence("Shift+Alt+Tab"));
    setToucScreenShortCut(TouchScreenGestureInterface::Swipe, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::Right, 4, QKeySequence("Alt+Tab"));
    setToucScreenShortCut(TouchScreenGestureInterface::Swipe, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::Up, 4, QKeySequence("Ctrl+End"));
    setToucScreenShortCut(TouchScreenGestureInterface::Swipe, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::Down, 4, QKeySequence("Ctrl+Home"));
    setToucScreenShortCut(TouchScreenGestureInterface::Swipe, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::Left, 5, QKeySequence("Ctrl+Alt+Left"));
    setToucScreenShortCut(TouchScreenGestureInterface::Swipe, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::Right, 5, QKeySequence("Ctrl+Alt+Right"));
    setToucScreenShortCut(TouchScreenGestureInterface::Swipe, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::Up, 5, QKeySequence("Ctrl+End"));
    setToucScreenShortCut(TouchScreenGestureInterface::Swipe, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::Down, 5, QKeySequence("Ctrl+Home"));

    setToucScreenShortCut(TouchScreenGestureInterface::Zoom, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::ZoomIn, 3, QKeySequence("Ctrl+="));
    setToucScreenShortCut(TouchScreenGestureInterface::Zoom, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::ZoomOut, 3, QKeySequence("Ctrl+-"));
    setToucScreenShortCut(TouchScreenGestureInterface::Zoom, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::ZoomIn, 4, QKeySequence("F11"));
    setToucScreenShortCut(TouchScreenGestureInterface::Zoom, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::ZoomOut, 4, QKeySequence("F11"));
    setToucScreenShortCut(TouchScreenGestureInterface::Zoom, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::ZoomIn, 5, QKeySequence("F11"));
    setToucScreenShortCut(TouchScreenGestureInterface::Zoom, TouchScreenGestureInterface::Finished, TouchScreenGestureInterface::ZoomOut, 5, QKeySequence("F11"));

    setTouchPadShortCut(TouchpadGestureManager::Swipe, TouchpadGestureManager::Finished, TouchpadGestureManager::Left, 3, QKeySequence("Alt+Left"));
    setTouchPadShortCut(TouchpadGestureManager::Swipe, TouchpadGestureManager::Finished, TouchpadGestureManager::Right, 3, QKeySequence("Alt+Right"));
    setTouchPadShortCut(TouchpadGestureManager::Swipe, TouchpadGestureManager::Finished, TouchpadGestureManager::Up, 3, QKeySequence("Ctrl+End"));
    setTouchPadShortCut(TouchpadGestureManager::Swipe, TouchpadGestureManager::Finished, TouchpadGestureManager::Down, 3, QKeySequence("Ctrl+Home"));
    setTouchPadShortCut(TouchpadGestureManager::Swipe, TouchpadGestureManager::Finished, TouchpadGestureManager::Left, 4, QKeySequence("Shift+Alt+Tab"));
    setTouchPadShortCut(TouchpadGestureManager::Swipe, TouchpadGestureManager::Finished, TouchpadGestureManager::Right, 4, QKeySequence("Alt+Tab"));
    setTouchPadShortCut(TouchpadGestureManager::Swipe, TouchpadGestureManager::Finished, TouchpadGestureManager::Up, 4, QKeySequence("Ctrl+Shift+Left"));
    setTouchPadShortCut(TouchpadGestureManager::Swipe, TouchpadGestureManager::Finished, TouchpadGestureManager::Down, 4, QKeySequence("Ctrl+Shift+Right"));

    setTouchPadShortCut(TouchpadGestureManager::Pinch, TouchpadGestureManager::Finished, TouchpadGestureManager::ZoomIn, 3, QKeySequence("Ctrl+="));
    setTouchPadShortCut(TouchpadGestureManager::Pinch, TouchpadGestureManager::Finished, TouchpadGestureManager::ZoomOut, 3, QKeySequence("Ctrl+-"));
    setTouchPadShortCut(TouchpadGestureManager::Pinch, TouchpadGestureManager::Finished, TouchpadGestureManager::ZoomIn, 4, QKeySequence("F11"));
    setTouchPadShortCut(TouchpadGestureManager::Pinch, TouchpadGestureManager::Finished, TouchpadGestureManager::ZoomOut, 4, QKeySequence("F11"));

    m_settings->sync();
}

SettingsManager *SettingsManager::getManager()
{
    if (!instance)
        instance = new SettingsManager;
    return instance;
}

QKeySequence SettingsManager::getShortCut(TouchScreenGestureInterface *gesture, TouchScreenGestureInterface::State state, TouchScreenGestureInterface::Direction direction)
{
    m_settings->beginGroup("touch screen");
    m_settings->beginGroup(m_touchScreenGestureType.valueToKey(gesture->type()));
    m_settings->beginReadArray(m_touchScreenGestureState.valueToKey(state));
    m_settings->setArrayIndex(gesture->finger());
    auto shortcut = qvariant_cast<QKeySequence>(m_settings->value(m_touchScreenGestureDirection.valueToKey(direction)));
    m_settings->endArray();
    m_settings->endGroup();
    m_settings->endGroup();
    return shortcut;
}

QKeySequence SettingsManager::gesShortCut(int fingerCount, TouchpadGestureManager::GestureType type, TouchpadGestureManager::State state, TouchpadGestureManager::Direction direction)
{
    m_settings->beginGroup("touchpad");
    m_settings->beginGroup(m_touchpadGestureType.valueToKey(type));

    m_settings->beginReadArray(m_touchpadGestureState.valueToKey(state));
    m_settings->setArrayIndex(fingerCount);
    QKeySequence shortcut = qvariant_cast<QKeySequence>(m_settings->value(m_touchpadGestureDirection.valueToKey(direction)));
    m_settings->endArray();

    m_settings->endGroup();
    m_settings->endGroup();

    return shortcut;
}

void SettingsManager::setToucScreenShortCut(TouchScreenGestureInterface::GestureType type, TouchScreenGestureInterface::State state, TouchScreenGestureInterface::Direction direction, int fingerCount, QKeySequence shortCut)
{
    m_settings->beginGroup("touch screen");
    m_settings->beginGroup(m_touchScreenGestureType.valueToKey(type));
    m_settings->beginWriteArray(m_touchScreenGestureState.valueToKey(state));
    m_settings->setArrayIndex(fingerCount);
    m_settings->setValue(m_touchScreenGestureDirection.valueToKey(direction), shortCut);
    m_settings->endArray();
    m_settings->endGroup();
    m_settings->endGroup();
    //m_settings->sync();
}

void SettingsManager::setTouchPadShortCut(TouchpadGestureManager::GestureType type, TouchpadGestureManager::State state, TouchpadGestureManager::Direction direction, int fingerCount, QKeySequence shortCut)
{
    m_settings->beginGroup("touchpad");
    m_settings->beginGroup(m_touchpadGestureType.valueToKey(type));
    m_settings->beginWriteArray(m_touchpadGestureState.valueToKey(state));
    m_settings->setArrayIndex(fingerCount);
    m_settings->setValue(m_touchpadGestureDirection.valueToKey(direction), shortCut);
    m_settings->endArray();
    m_settings->endGroup();
    m_settings->endGroup();
    //m_settings->sync();
}
