#include "settings-manager.h"

#include <QSettings>

static SettingsManager *instance = nullptr;

SettingsManager::SettingsManager(QObject *parent) : QObject(parent)
{
    QMetaEnum direction = QMetaEnum::fromType<TouchScreenGestureInterface::Direction>();
    QMetaEnum gestureType = QMetaEnum::fromType<TouchScreenGestureInterface::GestureType>();
    QMetaEnum gestureState = QMetaEnum::fromType<TouchScreenGestureInterface::State>();

    m_touchScreenGestureDirection = direction;
    m_touchScreenGestureState = gestureState;
    m_touchScreenGestureType = gestureType;

    m_settings = new QSettings("ukui", "libinput-gesture-translator", this);

    m_settings->beginGroup("touch screen");
    if (m_settings->childGroups().isEmpty()) {
        m_settings->beginGroup(gestureType.valueToKey(TouchScreenGestureInterface::GestureType::Swipe));
        m_settings->beginWriteArray(gestureState.valueToKey(TouchScreenGestureInterface::Finished));
        m_settings->setArrayIndex(3);
        auto left = direction.valueToKey(TouchScreenGestureInterface::Direction::Left);
        m_settings->setValue(left, QKeySequence("Alt+Left"));
        auto right = direction.valueToKey(TouchScreenGestureInterface::Direction::Right);
        m_settings->setValue(right, QKeySequence("Alt+Right"));
        m_settings->endArray();
        m_settings->endGroup();
    }
    m_settings->endGroup();
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
}
