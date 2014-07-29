#ifndef GLOBALSHORTCUT_H
#define GLOBALSHORTCUT_H

#include <QObject>
#include <QException>
#include <QKeySequence>

class GlobalShortcut : public QObject {

    Q_OBJECT

public:

    class KeyNotBoundException : public QException {
        QString _what ;
        QKeySequence _key ;
    public:
        KeyNotBoundException (QKeySequence key = QKeySequence (), QString what = QString ()) : QException (), _key (key), _what (what) {}
        virtual ~KeyNotBoundException () throw () {}

        QKeySequence key () const throw () { return _key ; }
        QString info () const throw () { return _what ; }
    };

    GlobalShortcut(QKeySequence hotkey, QObject *parent = 0);
    ~GlobalShortcut();

protected:

    static QHash<QPair<quint32, quint32>, GlobalShortcut*> shortcuts;

    friend class GlobalShortcutEventFilter ;

signals:
    void triggered ();

public slots:
    void trigger ();

private:

    static bool m_isFilterInstalled;

    QKeySequence m_shortcut ;

    int m_key = 0;
    int m_mods = 0;

    void registerKey(int nativeKey, int nativeMods);
    void unregisterKey(int nativeKey, int nativeMods);

};

#endif // GLOBALSHORTCUT_H
