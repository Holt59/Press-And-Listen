#ifndef GLOBALSHORTCUTEVENTFILTER_H
#define GLOBALSHORTCUTEVENTFILTER_H

#include <QAbstractNativeEventFilter>


class GlobalShortcutEventFilter : public QAbstractNativeEventFilter
{
public:
    explicit GlobalShortcutEventFilter();

protected:
    virtual bool nativeEventFilter (const QByteArray & eventType, void * message, long * result) Q_DECL_OVERRIDE ;

};

#endif // GLOBALSHORTCUTEVENTFILTER_H
