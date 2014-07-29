#ifndef GLOBALSHORTCUT_H
#define GLOBALSHORTCUT_H

#include <QObject>
#include <QException>


class KeyNotBoundException : public QException {
public:
    KeyNotBoundException () : QException () {}
    virtual ~KeyNotBoundException () throw () {}
};

class GlobalShortcut : public QObject
{
    Q_OBJECT
public:
    GlobalShortcut(QKeySequence hotkey);
    ~GlobalShortcut();

    void setHotKey(QKeySequence hotkey);

    static QHash<QPair<quint32, quint32>, GlobalShortcut*> shortcuts;

signals:
    void activated();

public slots:
    void activate();

private:
    int m_key = 0;
    int m_mods = 0;

    static bool m_isFilterInstalled;

    void registerKey(int nativeKey, int nativeMods);
    void unregisterKey(int nativeKey, int nativeMods);
};

#endif // GLOBALSHORTCUT_H
