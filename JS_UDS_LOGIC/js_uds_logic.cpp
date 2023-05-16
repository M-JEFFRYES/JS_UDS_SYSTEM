#include "js_uds_logic.h"

JS_UDS_LOGIC::JS_UDS_LOGIC(QObject* parent):QObject(parent)
{
    count = 0;
}

uint JS_UDS_LOGIC::addOne(uint value) noexcept
{
    return value+1;
}
