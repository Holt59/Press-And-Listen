#ifndef _PLAYERSETTINGSDIALOG_H
#define _PLAYERSETTINGSDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QMap>

#include "PressAndListenPlayer.h"

class PlayerSettingsDialog : public QDialog {

public:
    PlayerSettingsDialog (QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~PlayerSettingsDialog ();

protected:

    virtual void showEvent (QShowEvent *) ;

protected Q_SLOTS:

    void onResetClicked () ;
    void onSaveClicked () ;

private:

    QMap <PlayerInfo::Player, QCheckBox*> m_checkboxes ;

};

#endif