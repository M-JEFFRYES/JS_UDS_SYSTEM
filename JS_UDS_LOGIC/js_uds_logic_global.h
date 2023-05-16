#ifndef JS_UDS_LOGIC_GLOBAL_H
#define JS_UDS_LOGIC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(JS_UDS_LOGIC_LIBRARY)
#  define JS_UDS_LOGIC_EXPORT Q_DECL_EXPORT
#else
#  define JS_UDS_LOGIC_EXPORT Q_DECL_IMPORT
#endif

#endif // JS_UDS_LOGIC_GLOBAL_H
