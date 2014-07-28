#include "NotificationSettingsDialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>

NotificationSettingsDialog::NotificationSettingsDialog (QWidget *parent, Qt::WindowFlags flags) : QDialog (parent, flags) {
    setWindowTitle (tr ("Notification Settings")) ;
    setModal (true) ;
    if (parent) {
        setWindowIcon (parent->windowIcon ()) ;
    }
    QVBoxLayout *layout = new QVBoxLayout (this) ;
    for (auto notif : PressAndListenSettings::getNotificationTypes ()) {
        m_checkboxes[notif] = new QCheckBox (PressAndListenSettings::toUserString (notif)) ;
        m_checkboxes[notif]->setChecked (PressAndListenSettings ().showNotification (notif)) ;
        layout->addWidget (m_checkboxes[notif]) ;
    }
    setLayout (layout) ;
    QHBoxLayout *buttonLayout = new QHBoxLayout (this) ;
    QPushButton *resetButton = new QPushButton (tr ("Reset")) ;
    QPushButton *saveButton = new QPushButton (tr ("Save")) ;
    buttonLayout->addWidget (resetButton) ;
    buttonLayout->addWidget (saveButton) ;
    layout->addLayout (buttonLayout) ;

    connect (resetButton, &QPushButton::clicked, this, &NotificationSettingsDialog::onResetClicked) ;
    connect (saveButton, &QPushButton::clicked, this, &NotificationSettingsDialog::onSaveClicked) ;
    connect (saveButton, &QPushButton::clicked, this, &NotificationSettingsDialog::hide) ;

}


NotificationSettingsDialog::~NotificationSettingsDialog () {}

void NotificationSettingsDialog::onResetClicked () {
    for (auto itnotif = m_checkboxes.begin () ; itnotif != m_checkboxes.end () ; ++itnotif) {
        itnotif.value ()->setChecked (true) ;
    }
}

void NotificationSettingsDialog::onSaveClicked () {
    PressAndListenSettings settings ;
    for (auto itnotif = m_checkboxes.begin () ; itnotif != m_checkboxes.end () ; ++itnotif) {
        settings.setShowNotification (itnotif.key (), itnotif.value ()->isChecked ()) ;
    }
    settings.sync () ;
}

void NotificationSettingsDialog::showEvent (QShowEvent *) {
    for (auto itnotif = m_checkboxes.begin () ; itnotif != m_checkboxes.end () ; ++itnotif) {
        itnotif.value()->setChecked(PressAndListenSettings().showNotification(itnotif.key())) ;
    }
}
