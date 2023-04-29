# Linux build does not have separate 'debug' and 'release' folders when building,
# the entire directory will be either 'debug' or 'release'
linux{
   PRE_TARGETDEPS += ../application/libMainCode.a
   LIBS += -L../MainCode
}

# Add in the correct location for the static library, depending on if we are
# in a debug or release(for Windows)
msvc {
   debug{
       # Ensure that qt will always re-link
       # See: http://stackoverflow.com/questions/1485435/force-relink-when-building-in-qt-creator
       PRE_TARGETDEPS += ../application/debug/MainCode.lib
       LIBS += -L../application/debug/
   }else{
       PRE_TARGETDEPS += ../application/release/MainCode.lib
       LIBS += -L../application/release/
   }
}

win32:!msvc{
   debug{
       # Ensure that qt will always re-link
       # See: http://stackoverflow.com/questions/1485435/force-relink-when-building-in-qt-creator
       PRE_TARGETDEPS += ../application/debug/libMainCode.a
       LIBS += -L../application/debug/
   }else{
       PRE_TARGETDEPS += ../application/release/libMainCode.a
       LIBS += -L../application/release/
   }
}

# MainCode is the main code library that we are going to (statically!) link with.
LIBS += -lapplication

# Add in any other libraries here...
# for example, libraries from pkg-config
#unix: CONFIG += link_pkgconfig
#unix: PKGCONFIG += libfoo
