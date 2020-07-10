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

#ifndef EVENTMONITOR_H
#define EVENTMONITOR_H

#include <QObject>

#include <libinput.h>

class TouchScreenGestureManager;

class EventMonitor : public QObject
{
    Q_OBJECT
public:
    enum ActionType {
        UNKNOW,
        FINGER_SWIPE,
        FINGER_ZOOM
    };
    Q_ENUM(ActionType)

    explicit EventMonitor(QObject *parent = nullptr);

public slots:
    void startMonitor();

    void initTouchScreenGestureManager(TouchScreenGestureManager *manager);

signals:
    void touchscreenGestureRequest(int fingerCount, ActionType type);
    void touchpadGestureRequest(int fingerCount, ActionType type);

private:
    libinput *m_input;

    TouchScreenGestureManager *m_touchScreenGestureManager = nullptr;
};

#endif // EVENTMONITOR_H
