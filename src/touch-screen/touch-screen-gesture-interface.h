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

#ifndef TOUCHSCREENGESTUREINTERFACE_H
#define TOUCHSCREENGESTUREINTERFACE_H

#include <QObject>

#include <libinput.h>

class TouchScreenGestureInterface : public QObject
{
    Q_OBJECT
public:
    enum GestureType {
        Unknow,
        Swipe,
        Zoom,
        Tap,
        DragAndTap
    };
    Q_ENUM(GestureType)

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

    enum State {
        Ignore,
        Maybe,
        Update,
        Cancelled,
        Pending,  // gesture action will real execute when all finger released.
        Finished
    };
    Q_ENUM(State)

    explicit TouchScreenGestureInterface(QObject *parent = nullptr);

    virtual int finger() = 0;

    virtual GestureType type() = 0;

    virtual State handleInputEvent(libinput_event *event) {return Ignore;}

    virtual Direction totalDirection() {return None;}

    virtual Direction lastDirection() {return None;}

    virtual void reset() {}

    virtual bool isCancelled() {return false;}
    virtual void cancel() {} // using to cancel some gesture, if it is cancellable.

protected:

    /*!
     * \brief getGestureIndex
     * \return an int value from touch screen gesture manager
     */
    int getGestureIndex();

signals:
    void gestureBegin(int registedIndex);
    void gestureUpdate(int registedIndex);
    void gestureCancelled(int registedIndex);
    void gestureFinished(int registedIndex);
};

#endif // TOUCHSCREENGESTUREINTERFACE_H
