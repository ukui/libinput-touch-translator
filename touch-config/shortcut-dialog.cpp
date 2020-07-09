#include "shortcut-dialog.h"
#include "ui_shortcut-dialog.h"

ShortcutDialog::ShortcutDialog(const QKeySequence &shortcut, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShortcutDialog)
{
    ui->setupUi(this);

    ui->kkeysequencewidget->setKeySequence(shortcut);
}

ShortcutDialog::~ShortcutDialog()
{
    delete ui;
}

QKeySequence ShortcutDialog::getKeySequence()
{
    return ui->kkeysequencewidget->keySequence();
}
