#include "PlayerListWidget.h"

#include <QHeaderView>
#include <QMenu>

PlayerListWidget::PlayerListWidget (QWidget *parent) : QTableWidget (0, 1, parent), m_contextMenuRow(-1) {

    this->horizontalHeader ()->setSectionResizeMode (QHeaderView::ResizeMode::Stretch);
    this->verticalHeader ()->setSectionResizeMode (QHeaderView::ResizeToContents);
    this->horizontalHeader ()->hide ();
    this->verticalHeader ()->hide ();
    this->setSelectionMode (QAbstractItemView::NoSelection) ;

    this->setContextMenuPolicy (Qt::CustomContextMenu);
    createActions () ;

    connect (this, &QTableWidget::customContextMenuRequested, this, &PlayerListWidget::onCustomMenuRequested) ;
}

PlayerListWidget::~PlayerListWidget () {}

void PlayerListWidget::createActions () {

    m_sleepPlayer = new QAction (tr ("P&ause player controls"), this);
    connect (m_sleepPlayer, &QAction::triggered, this, &PlayerListWidget::onUserPlayerSleep);

    m_wakeUpPlayer = new QAction (tr ("R&esume player controls"), this);
    connect (m_wakeUpPlayer, &QAction::triggered, this, &PlayerListWidget::onUserPlayerWakeUp);

    m_closeTabPlayer = new QAction (tr ("C&lose player tab"), this);
    connect (m_closeTabPlayer, &QAction::triggered, this, &PlayerListWidget::onUserPlayerCloseTab);

    m_setAsCurrentPlayer = new QAction (tr ("S&et as current player"), this);
    connect (m_setAsCurrentPlayer, &QAction::triggered, this, &PlayerListWidget::onUserPlayerSetAsCurrent);

    m_focusTabPlayer = new QAction (tr ("Sh&ow player tab"), this);
    connect (m_focusTabPlayer, &QAction::triggered, this, &PlayerListWidget::onUserPlayerFocusTab);

}

void PlayerListWidget::onCustomMenuRequested (QPoint pos) {
    QModelIndex index = this->indexAt (pos);

    m_contextMenuRow = index.row () ;
    PressAndListenPlayer *player = m_players[m_contextMenuRow] ;

    QMenu *menu = new QMenu (this);
    if (player->isEnable ()) {
        menu->addAction (m_setAsCurrentPlayer);
        menu->addAction (m_sleepPlayer);
    }
    else {
        menu->addAction (m_wakeUpPlayer);
    }
    menu->addSeparator ();
    menu->addAction (m_focusTabPlayer);
    menu->addAction (m_closeTabPlayer);
    menu->popup (this->viewport ()->mapToGlobal (pos));
}

int PlayerListWidget::getRowForPlayer (PressAndListenPlayer *player) {
    return m_players.indexOf (player) ;
}

PlayerItemWidget *PlayerListWidget::getCellWidget (int row, int column) {
    return qobject_cast <PlayerItemWidget *> (this->cellWidget (row, column)) ;
}

PlayerItemWidget *PlayerListWidget::getCellWidget (PressAndListenPlayer *player) {
    return getCellWidget (getRowForPlayer (player), 0);
}

void PlayerListWidget::onPlayerEnter (PressAndListenPlayer *player) {
    m_players.push_back (player);
    this->insertRow (this->rowCount());
    this->setCellWidget (this->rowCount () - 1, 0, new PlayerItemWidget(player));
}

void PlayerListWidget::onPlayerLeave (PressAndListenPlayer *player) {
    int pIndex = m_players.indexOf (player) ;
    if (pIndex < 0) {
        return ;
    }
    m_players.removeAt (pIndex);
    this->removeRow (pIndex);
}

void PlayerListWidget::onUserPlayerSleep () {
    if (m_contextMenuRow < 0) {
        return ;
    }
    getCellWidget (m_contextMenuRow, 0)->update () ;
    emit playerSleep (m_players[m_contextMenuRow]);
}

void PlayerListWidget::onUserPlayerWakeUp () {
    if (m_contextMenuRow < 0) {
        return ;
    }
    getCellWidget (m_contextMenuRow, 0)->update () ;
    emit playerWakeUp (m_players[m_contextMenuRow]);
}

void PlayerListWidget::onUserPlayerSetAsCurrent () {
    if (m_contextMenuRow < 0) {
        return ;
    }
    emit playerSetAsCurrent (m_players[m_contextMenuRow]);
}

void PlayerListWidget::onUserPlayerFocusTab () {
    if (m_contextMenuRow < 0) {
        return ;
    }
    emit playerFocusTab (m_players[m_contextMenuRow]);
}

void PlayerListWidget::onUserPlayerCloseTab () {
    if (m_contextMenuRow < 0) {
        return ;
    }
    emit playerCloseTab (m_players[m_contextMenuRow]);
}