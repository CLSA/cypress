# cypress
instrument integration with CLSA Pine

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
