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

#ifndef TOUCHSCREENTWOFINGERTAPGESTURE_H
#define TOUCHSCREENTWOFINGERTAPGESTURE_H

#include "touch-screen-gesture-interface.h"

#include <QPointF>

class TouchScreenTwoFingerTapGesture : public TouchScreenGestureInterface
{
public:
    explicit TouchScreenTwoFingerTapGesture(QObject *parent = nullptr);

    int finger() override {return 2;}

    GestureType type() override {return Tap;}

    State handleInputEvent(libinput_event *event) override;

    void reset() override;

    Direction totalDirection() override;

    Direction lastDirection() override;

    void cancel() override;

    bool isCancelled() override;

private:
    int m_currentFingerCount = 0;

    int m_startTime = 0;
    int m_endTime = 0;

    bool m_isCancelled = false;

    QPointF m_startPoints[2];
    QPointF m_currentPoints[2];
};

#endif // TOUCHSCREENTWOFINGERTAPGESTURE_H
