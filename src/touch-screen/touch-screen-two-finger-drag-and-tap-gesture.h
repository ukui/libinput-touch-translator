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

#ifndef TOUCHSCREENTWOFINGERDRAGANDTAPGESTURE_H
#define TOUCHSCREENTWOFINGERDRAGANDTAPGESTURE_H

#include "touch-screen-gesture-interface.h"

#include <QPointF>

class TouchScreenTwoFingerDragAndTapGesture : public TouchScreenGestureInterface
{
public:
    explicit TouchScreenTwoFingerDragAndTapGesture(QObject *parent = nullptr);

    int finger() override {return 2;}

    GestureType type() override {return DragAndTap;}

    State handleInputEvent(libinput_event *event) override;

    void reset() override;

    Direction totalDirection() override {return None;}

    Direction lastDirection() override {return None;}

    void cancel() override;

    bool isCancelled() override;

private:
    int m_currentFingerCount = 0;
    int m_lastSecondFingerPressedTime = 0;

    QPointF m_firstPoint;
    QPointF m_secondPoint;

    bool m_isCancelled = false;
    bool m_isStarted = false;
};

#endif // TOUCHSCREENTWOFINGERDRAGANDTAPGESTURE_H
