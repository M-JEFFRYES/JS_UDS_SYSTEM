# Compiling UDSWB application

1. Use QT creator to complie the release version (32-bit)
2. Run "RUN_WDQT_32-BIT.bat" to set the QT environment variables and copy DLLs to the release folder
3. Check the "installer_setup_script_32-bit.iss" Inno Setup script and ensure version of paths are correct
4. Run the setup script to create the installer file
5. Commit and push the new installer  