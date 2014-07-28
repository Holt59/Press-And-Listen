#ifndef _NOTIFICATIONSETTINGSDIALOG_H
#define _NOTIFICATIONSETTINGSDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QMap>

#include "PressAndListenSettings.h"

class NotificationSettingsDialog : public QDialog {

public:
    NotificationSettingsDialog (QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~NotificationSettingsDialog ();

protected:

    virtual void showEvent (QShowEvent *) ;

protected Q_SLOTS:

    void onResetClicked () ;
    void onSaveClicked () ;

private:

    QMap <PressAndListenSettings::NotificationType, QCheckBox*> m_checkboxes ;

};

#endif