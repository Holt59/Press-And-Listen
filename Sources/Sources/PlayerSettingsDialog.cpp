#include "PlayerSettingsDialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>

#include "PressAndListenSettings.h"

PlayerSettingsDialog::PlayerSettingsDialog (QWidget *parent, Qt::WindowFlags flags) : QDialog (parent, flags) {
    setWindowTitle (tr ("Player Settings")) ;
    setModal (true) ;
    if (parent) {
        setWindowIcon (parent->windowIcon ()) ;
    }
    QVBoxLayout *layout = new QVBoxLayout (this) ;
    for (auto player : PlayerInfo::getPlayers()) {
        m_checkboxes[player] = new QCheckBox (PlayerInfo::toString (player)) ;
        m_checkboxes[player]->setChecked (SETTINGS.isPlayerEnabled (player)) ;
        layout->addWidget (m_checkboxes[player]) ;
    }
    setLayout (layout) ;
    QHBoxLayout *buttonLayout = new QHBoxLayout (this) ;
    QPushButton *resetButton = new QPushButton (tr ("Reset")) ;
    QPushButton *saveButton = new QPushButton (tr ("Save")) ;
    buttonLayout->addWidget (resetButton) ;
    buttonLayout->addWidget (saveButton) ;
    layout->addLayout (buttonLayout) ;

    connect (resetButton, &QPushButton::clicked, this, &PlayerSettingsDialog::onResetClicked) ;
    connect (saveButton, &QPushButton::clicked, this, &PlayerSettingsDialog::onSaveClicked) ;

}


PlayerSettingsDialog::~PlayerSettingsDialog () {}

void PlayerSettingsDialog::onResetClicked () {
    for (auto itnotif = m_checkboxes.begin () ; itnotif != m_checkboxes.end () ; ++itnotif) {
        itnotif.value ()->setChecked (true) ;
    }
}

void PlayerSettingsDialog::onSaveClicked () {
    for (auto itnotif = m_checkboxes.begin () ; itnotif != m_checkboxes.end () ; ++itnotif) {
        SETTINGS.setPlayerEnabled (itnotif.key (), itnotif.value ()->isChecked ()) ;
    }
    SETTINGS.sync () ;
    this->done (QDialog::Accepted) ;
}

void PlayerSettingsDialog::showEvent (QShowEvent *) {
    for (auto itnotif = m_checkboxes.begin () ; itnotif != m_checkboxes.end () ; ++itnotif) {
        itnotif.value ()->setChecked (SETTINGS.isPlayerEnabled (itnotif.key())) ;
    }
}
