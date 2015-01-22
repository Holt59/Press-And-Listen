#ifndef _PLAYERITEMWIDGET_H
#define _PLAYERITEMWIDGET_H

#include <QFrame>
#include <QLabel>

#include "PressAndListenPlayer.h"

class PlayerItemWidget : public QFrame {

    Q_OBJECT

public:
    PlayerItemWidget (PressAndListenPlayer *player, QWidget *parent = 0);

public Q_SLOTS:

    void updateInformation () ;

private:

    PressAndListenPlayer *m_player ;
    QLabel *m_text, *m_logo ;
    QFrame *m_active ;

};

#endif