#include "settings-manager.h"

#include <QSettings>

#include <QFileSystemWatcher>

#include <QDebug>

static SettingsManager *instance = nullptr;

SettingsManager::SettingsManager(QObject *parent) : QObject(parent)
{
    QMetaEnum direction = QMetaEnum::fromType<TouchScreenGestureInterface::Direction>();
    QMetaEnum gestureType = QMetaEnum::fromType<TouchScreenGestureInterface::GestureType>();
    QMetaEnum gestureState = QMetaEnum::fromType<TouchScreenGestureInterface::State>();

    m_touchScreenGestureDirection = direction;
    m_touchScreenGestureState = gestureState;
    m_touchScreenGestureType = gestureType;

    m_settings = new QSettings(QSettings::SystemScope, "ukui", "gestures", this);
    qDebug()<<m_settings->fileName();

    QFileSystemWatcher *watcher = new QFileSystemWatcher(QStringList()<<m_settings->fileName(), this);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, [=](){
        qDebug()<<"file changed, sync";
        m_settings->sync();
        // we should rewatch the changed file.
        watcher->addPath(m_settings->fileName());
    });

    m_settings->beginGroup("touch screen");
    if (m_settings->childGroups().isEmpty()) {
        // swipe group
        m_settings->beginGroup(gestureType.valueToKey(TouchScreenGestureInterface::GestureType::Swipe));
        m_settings->beginWriteArray(gestureState.valueToKey(TouchScreenGestureInterface::Finished));
        m_settings->setArrayIndex(3);

        auto left = direction.valueToKey(TouchScreenGestureInterface::Direction::Left);
        m_settings->setValue(left, QKeySequence("Alt+Left"));
        auto right = direction.valueToKey(TouchScreenGestureInterface::Direction::Right);
        m_settings->setValue(right, QKeySequence("Alt+Right"));

        m_settings->setArrayIndex(4);
        m_settings->setValue(left, QKeySequence("Alt+Shift+Tab"));
        m_settings->setValue(right, QKeySequence("Alt+Tab"));

        m_settings->setArrayIndex(5);
        m_settings->setValue(left, QKeySequence("Ctrl+Alt+Left"));
        m_settings->setValue(right, QKeySequence("Ctrl+Alt+Right"));

        m_settings->endArray();

        m_settings->endGroup();

        // zoom group
        m_settings->beginGroup(gestureType.valueToKey(TouchScreenGestureInterface::GestureType::Zoom));
        m_settings->beginWriteArray(gestureState.valueToKey(TouchScreenGestureInterface::Finished));

        m_settings->setArrayIndex(4);
        auto zoomIn = direction.valueToKey(TouchScreenGestureInterface::Direction::ZoomIn);
        m_settings->setValue(zoomIn, QKeySequence("F11"));
        auto zoomOut = direction.valueToKey(TouchScreenGestureInterface::Direction::ZoomOut);
        m_settings->setValue(zoomOut, QKeySequence("F11"));

        m_settings->setArrayIndex(5);
        m_settings->setValue(zoomIn, QKeySequence("F11"));
        m_settings->setValue(zoomOut, QKeySequence("F11"));

        m_settings->endArray();

        m_settings->endGroup();
    }
    m_settings->endGroup();
    m_settings->sync();
}

SettingsManager *SettingsManager::getManager()
{
    if (!instance)
        instance = new SettingsManager;
    return instance;
}

QKeySequence SettingsManager::getShortCut(TouchScreenGestureInterface *gesture, TouchScreenGestureInterface::State state, TouchScreenGestureInterface::Direction direction)
{
    m_settings->beginGroup("touch screen");
    m_settings->beginGroup(m_touchScreenGestureType.valueToKey(gesture->type()));
    m_settings->beginReadArray(m_touchScreenGestureState.valueToKey(state));
    m_settings->setArrayIndex(gesture->finger());
    auto shortcut = qvariant_cast<QKeySequence>(m_settings->value(m_touchScreenGestureDirection.valueToKey(direction)));
    m_settings->endArray();
    m_settings->endGroup();
    m_settings->endGroup();
    return shortcut;
}

void SettingsManager::setShortCut(TouchScreenGestureInterface::GestureType type, TouchScreenGestureInterface::State state, TouchScreenGestureInterface::Direction direction, int fingerCount, QKeySequence shortCut)
{
    m_settings->beginGroup("touch screen");
    m_settings->beginGroup(m_touchScreenGestureType.valueToKey(type));
    m_settings->beginWriteArray(m_touchScreenGestureState.valueToKey(state));
    m_settings->setArrayIndex(fingerCount);
    m_settings->setValue(m_touchScreenGestureDirection.valueToKey(direction), shortCut);
    m_settings->endArray();
    m_settings->endGroup();
    m_settings->endGroup();
    m_settings->sync();
}
