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


#ifndef TOUCHSCREENONEFINGEREDGEGESTURE_H
#define TOUCHSCREENONEFINGEREDGEGESTURE_H

#include "touch-screen-gesture-interface.h"

#include <QPointF>

class TouchScreenOneFingerEdgeGesture : public TouchScreenGestureInterface
{
public:
    explicit TouchScreenOneFingerEdgeGesture(QObject *parent = nullptr);

    virtual int finger() {return 1;}

    virtual GestureType type() {return Edge;}

    virtual State handleInputEvent(libinput_event *event);

    virtual Direction totalDirection();

    virtual Direction lastDirection();

    virtual void reset();

    virtual bool isCancelled();
    virtual void cancel();

    int longestDistance();

private:
    int m_fingerCount = 0;

    Direction m_direction = None;
    bool m_isCancelled = false;

    QPointF m_startPoint;
    QPointF m_lastPoint;
    QPointF m_currentPoint;
};

#endif // TOUCHSCREENONEFINGEREDGEGESTURE_H
