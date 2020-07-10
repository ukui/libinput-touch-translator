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

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QKeySequence>

#include <QMetaEnum>
#include "touch-screen/touch-screen-gesture-interface.h"
#include "touchpad/touchpad-gesture-manager.h"

class TouchScreenGestureInterface;
class QSettings;

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    static SettingsManager *getManager();

    QKeySequence getShortCut(TouchScreenGestureInterface *gesture,
                             TouchScreenGestureInterface::State state,
                             TouchScreenGestureInterface::Direction direction);

    QKeySequence gesShortCut(int fingerCount,
                             TouchpadGestureManager::GestureType type,
                             TouchpadGestureManager::State state,
                             TouchpadGestureManager::Direction direction);

signals:

public slots:
    void setToucScreenShortCut(TouchScreenGestureInterface::GestureType type,
                               TouchScreenGestureInterface::State state,
                               TouchScreenGestureInterface::Direction direction,
                               int fingerCount,
                               QKeySequence shortCut);

    void setTouchPadShortCut (TouchpadGestureManager::GestureType type,
                              TouchpadGestureManager::State state,
                              TouchpadGestureManager::Direction direction,
                              int fingerCount,
                              QKeySequence shortCut);

private:
    explicit SettingsManager(QObject *parent = nullptr);
    QSettings *m_settings;

    QMetaEnum m_touchScreenGestureType;
    QMetaEnum m_touchScreenGestureState;
    QMetaEnum m_touchScreenGestureDirection;

    QMetaEnum m_touchpadGestureType;
    QMetaEnum m_touchpadGestureState;
    QMetaEnum m_touchpadGestureDirection;
};

#endif // SETTINGSMANAGER_H
