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

#ifndef TOUCHSCREENGESTUREMANAGER_H
#define TOUCHSCREENGESTUREMANAGER_H

#include <QObject>

#include <libinput.h>

class TouchScreenGestureInterface;

class TouchScreenGestureManager : public QObject
{
    friend class TouchScreenGestureInterface;
    Q_OBJECT
public:
    static TouchScreenGestureManager *getManager();

    int queryGestureIndex(TouchScreenGestureInterface *gesture);

    void processEvent(libinput_event *event);
signals:

public slots:
    void onGestureBegin(int index);
    void onGestureUpdated(int index);
    void onGestureCancelled(int index);
    void onGestureFinished(int index);

private:
    int registerGesuture(TouchScreenGestureInterface *gesture); // return a index of registered gesture.

    explicit TouchScreenGestureManager(QObject *parent = nullptr);
    QList<TouchScreenGestureInterface *> m_gestures;
};

#endif // TOUCHSCREENGESTUREMANAGER_H
