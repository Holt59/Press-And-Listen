#ifndef _PLAYERITEMWIDGET_H
#define _PLAYERITEMWIDGET_H

#include <QLabel>

#include "PressAndListenPlayer.h"

class PlayerItemWidget : public QLabel {

    Q_OBJECT

public:
    PlayerItemWidget (PressAndListenPlayer *player, QWidget *parent = 0);

public Q_SLOTS:

    void updateInformation () ;

private:

    PressAndListenPlayer *m_player ;

};

#endif