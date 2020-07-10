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

#ifndef TOUCHPADGESTUREMANAGER_H
#define TOUCHPADGESTUREMANAGER_H

#include <QObject>

#include <libinput.h>

class TouchpadGestureManager : public QObject
{
    Q_OBJECT
public:
    enum GestureType {
        Swipe,
        Pinch
    };
    Q_ENUM(GestureType)

    enum State {
        Begin,
        Update,
        Cancelled,
        Finished
    };
    Q_ENUM(State)

    enum Direction {
        None,
        Left,
        Right,
        Up,
        Down,
        ZoomIn,
        ZoomOut
    };
    Q_ENUM(Direction)

    static TouchpadGestureManager *getManager();

signals:
    void eventTriggered(GestureType type, int fingerCount, State state, Direction direction);

public slots:
    void processEvent(libinput_event *event);
    void reset();

    void onEventTriggerd(GestureType type, int fingerCount, State state, Direction direction);

private:
    explicit TouchpadGestureManager(QObject *parent = nullptr);

    int m_lastFinger = 0;
    bool m_isCancelled = 0;

    double m_totalDxmm = 0;
    double m_totalDymm = 0;

    double m_totalScale = 0;
    double m_totalAngle = 0;
};

#endif // TOUCHPADGESTUREMANAGER_H
