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
