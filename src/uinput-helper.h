#ifndef UINPUTHELPER_H
#define UINPUTHELPER_H

#include <QObject>

#include <QKeySequence>
#include <QHash>

class UInputHelper : public QObject
{
    Q_OBJECT
public:
    static UInputHelper *getInstance();

signals:

public slots:
    void executeShortCut(const QKeySequence &shortCut);
    void clickMouseRightButton();

protected:
    QList<int> parseShortcut(const QKeySequence &shortCut);

private:
    explicit UInputHelper(QObject *parent = nullptr);

    QHash<QString, int> m_hash;
};

#endif // UINPUTHELPER_H
