#ifndef JS_UDS_LOGIC_H
#define JS_UDS_LOGIC_H

#include "js_uds_logic_global.h"
#include <QObject>
#include <QHash>


class JS_UDS_LOGIC_EXPORT JS_UDS_LOGIC: public QObject
{
    Q_OBJECT

public:
    JS_UDS_LOGIC(QObject* parent=nullptr);

    Q_INVOKABLE uint addOne(uint) noexcept;

private:
    int count;
};

#endif // JS_UDS_LOGIC_H

