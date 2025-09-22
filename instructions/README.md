# Compiling UDSWB application

1. Use QT creator to complie the release version (32-bit)
2. Run "RUN_WDQT_32-BIT.bat" to set the QT environment variables and copy DLLs to the release folder
3. Check the "installer_setup_script_32-bit.iss" Inno Setup script and ensure version of paths are correct
4. Run the setup script to create the installer file
5. Commit and push the new installer

## Developing on MacOS

### 1. Configure the project

/opt/homebrew/opt/qt@5/bin/qmake CONFIG+=sdk_no_version_check src/JS_UDS_UI.pro

### 2. Build the project

make -j$(sysctl -n hw.ncpu)

### 3. Run the application

./JS_UDS_UI.app/Contents/MacOS/JS_UDS_UI
