TEMPLATE = subdirs

CONFIG  += ordered

SUBDIRS += \
    libcore \
    src \
    tests

libcore.file = libcore/libcore.pro
src.file     = src/JS_UDS_UI.pro
tests.file   = tests/tests.pro

# Build order
src.depends = libcore
tests.depends = libcore
