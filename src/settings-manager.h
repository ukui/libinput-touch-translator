#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QKeySequence>

#include <QMetaEnum>

#include "touch-screen/touch-screen-gesture-interface.h"

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

signals:

public slots:
    void setShortCut(TouchScreenGestureInterface::GestureType type,
                     TouchScreenGestureInterface::State state,
                     TouchScreenGestureInterface::Direction direction,
                     int fingerCount,
                     QKeySequence shortCut);

private:
    explicit SettingsManager(QObject *parent = nullptr);
    QSettings *m_settings;

    QMetaEnum m_touchScreenGestureType;
    QMetaEnum m_touchScreenGestureState;
    QMetaEnum m_touchScreenGestureDirection;
};

#endif // SETTINGSMANAGER_H
