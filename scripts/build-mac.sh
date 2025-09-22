BUILD_DIR=./build-mac
mkdir -p "$BUILD_DIR" && cd "$BUILD_DIR"


/opt/homebrew/opt/qt@5/bin/qmake \
  CONFIG+=sdk_no_version_check \
  "OBJECTS_DIR=$PWD/tmp/obj" \
  "MOC_DIR=$PWD/tmp/moc" \
  "RCC_DIR=$PWD/tmp/rcc" \
  "UI_DIR=$PWD/tmp/ui" \
  "DESTDIR=$PWD/out" \
  ../src/JS_UDS_UI.pro