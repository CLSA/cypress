# CLSA Cypress

Instrument integration with CLSA Pine

How to build: 
Windows:
  - Install Qt 5.15.2 using the Online installer (https://www.qt.io/product/development-tools?hsLang=en), make sure to pick LTS 5.15.2 when running the installer 
  - Pull Cypress from Github 
  - Import Cypress into Qt Creator by opening the Cypress.pro file from Qt Creator
  - Download QtUSB (https://github.com/fpoussin/QtUsb/releases/download/v0.7.0/v0.7.0-qt5.15-usb-msvc-dynamic-x64.7z)
  - Extract QtUSB
  - Copy all files from the extracted QtUSB into their respective locations at C:\Qt\5.15.2  (should have matching sub directories)
  - Cypress should now build and run 
  - To run Cypress.exe from the command line, update your path to include C:\Qt\5.15.2\msvc2019_64\bin, this ensures that Windows can locate .dlls at runtime 

Dependencies:
    Qt 	  v5.15.2
    MSVC  2019 64bit
    DCMTK v3.6.7
    POCO  
    CMake v3.24.3
    QtUSB v0.7.0

Download source code from 
https://dicom.offis.de/download/dcmtk/dcmtk367/dcmtk-3.6.7.zip

Download support binaries from 
https://dicom.offis.de/download/dcmtk/dcmtk367/support/dcmtk-3.6.7-win64-support-MD-iconv-msvc-16.9.zip

Extract both folders 

Open CMake GUI and select the extracted DCMTK source code folder and choose a build directory of choice
Press configure, choose MSVC 2019 and platform target x64
Click finish and wait for configuration to complete, there should be no errors in the logs, it's fine if some files are not found

Once the initial configuration is complete there should be variables marked red in the GUI 
Select Grouped and Advanced

Under CMAKE group:
Change all values with /MT and /MTd to /MD and /MDd

Under DCMTK group: 
DCMTK_WITH_ICONV = True
DCMTK_WITH_OPENJPEG = True
DCMTK_WITH_OPENSSL = True
DCMTK_WITH_PNG = True
DCMTK_WITH_THREADS = True
DCMTK_WITH_TIFF = True
DCMTK_WITH_XML = True
DCMTK_WITH_ZLIB = True

Under WITH group:
set paths for each support library, i.e
WITH_LIBXMLINC = C:/Users/Anthony/Documents/DCMTK/dcmtk-3.6.7-win64-support-MD-iconv-msvc-16.9/libxml2-iconv-2.9.13

Tip, use the file browser instead of typing paths, use forward slashes, and no double or single quotes

Press Configure again
Once complete, open your build folder as specified under "Where to build the binaries"
Open the solution file in MS Visual Studio 2019 
Build the project ALL_PROJECTS within VS, this may take a few moments
Build the project INSTALL within VS, this will place the compiled binaries and headers into C:/Program Files/DCMTK, or CMAKE_INSTALL_PREFIX  
Make sure that DCMTK is in your build path in Qt Creator 


How to run from the command line?
The release build of the application won't run / find any of the .dll files unless you run the Qt tool "windeployqt.exe" inside the directory with the Cypress.exe, the tool will build a 
dependency tree and copy the .dll files into the directory. 
 
