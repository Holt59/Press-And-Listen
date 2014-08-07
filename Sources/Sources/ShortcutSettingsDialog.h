#ifndef _SHORTCUT_SETTINGS_DIALOG_H
#define _SHORTCUT_SETTINGS_DIALOG_H

#include <QDialog>

class ShortcutSettingsDialog : public QDialog {
public:
    ShortcutSettingsDialog (QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~ShortcutSettingsDialog ();
};

#endif