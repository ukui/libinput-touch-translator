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

#ifndef UINPUTHELPER_H
#define UINPUTHELPER_H

#include <QObject>

#include <QKeySequence>
#include <QHash>

#include <QPointF>

class UInputHelper : public QObject
{
    Q_OBJECT
public:
    static UInputHelper *getInstance();

signals:

public slots:
    void executeShortCut(const QKeySequence &shortCut);
    void clickMouseRightButton();
    void wheel(QPointF offset);

protected:
    QList<int> parseShortcut(const QKeySequence &shortCut);

private:
    explicit UInputHelper(QObject *parent = nullptr);

    QHash<QString, int> m_hash;
};

#endif // UINPUTHELPER_H
