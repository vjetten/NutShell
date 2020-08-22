#-------------------------------------------------
#
# Project created by QtCreator 2010-07-04T08:04:34
#
#-------------------------------------------------

QT       += core gui widgets
#QTDIR = C:\Qt\Qt5.8.0\5.8\msvc2015
#QTDIR =C:\Qt\Q513\5.13.0\mingw73_64
#QTDIR = C:\Qt\Q513\5.13.0\msvc2015_64
QWTDIR = C:/Qt/qwt-6.1.4
TARGET = nutshell
TEMPLATE = app
CONFIG += exceptions
#QMAKE_CXXFLAGS += -Wall
#CONFIG += static
CONFIG += warn_off
SOURCES += main.cpp\
    nutshellIni.cpp \
        nutshellqt.cpp \
        nutshellExplorer.cpp \
    nutshellAction.cpp \
    nutshelllegend.cpp \
    nutshellmapattribute.cpp \
    nutshellfilenames.cpp \
    nutshelleditor.cpp \
    nutshelllinenumber.cpp \
    nutshellhighligter.cpp \
    nutshellProcess.cpp \
    nutshellhelp.cpp \
    nutshelloptions.cpp \
    findreplaceform.cpp \
    CsfMap.cpp \
    nutshellevent.cpp \
    nutshelltreeview.cpp \
    nutshellCommandwindow.cpp \
    nutshellmapedit.cpp
HEADERS  += nutshellqt.h \
    csfattr.h \
    csfimpl.h \
    csftypes.h \
         csf.h \
    nutshelllegend.h \
    nutshellmapattribute.h \
    nutshelleditor.h \
    nutshellhighlighter.h \
    nutshellhelp.h \
    nutshelloptions.h \
    findreplaceform.h \
    CsfMap.h \
    nutshellmapedit.h
FORMS    += nutshellqt.ui \
    nutshelllegend.ui \
    nutshellmapattribute.ui \
    nutshelloptions.ui \
    findreplaceform.ui \
    mapedit.ui \
    nutshellmapedit.ui
#INCLUDEPATH+=$${QTDIR}/lib
CONFIG(debug, debug|release) {
  # debug build
  win32:win32-msvc2015{
   # compile release with MSVC 2010
    DEFINES += _CRT_SECURE_NO_WARNINGS
    LIBS += -L"debug" -lcsfvcd
    DESTDIR = debug
  } else {
    LIBS += -L"debug" -lcsfd
    DESTDIR = debug
  }
  MOC_DIR = debug/moc
  OBJECTS_DIR= debug/objs
  UI_DIR= debug/ui
  RCC_DIR= debug/rcc
  DESTDIR = debug
} else {
  # release build
  win32:win32-msvc2015{
                 # compile release with MSVC 2010
    DEFINES += _CRT_SECURE_NO_WARNINGS
    LIBS +=  -L"release" -lcsf
  } else {
 # compile with mingw
  LIBS += -L"release" -lcsf
#-llibcsf
  }
  MOC_DIR = release/moc
  OBJECTS_DIR= release/objs
  UI_DIR= release/ui
  RCC_DIR= release/rcc
  DESTDIR = release
}
#INCLUDEPATH += "C:\Qt\qwt-6.1.4\src" # $${QWTDIR}/src
#INCLUDEPATH += $${QWTDIR}/include
#LIBS += $${QWTDIR}/lib
RESOURCES +=  nutshellqt.qrc
RC_FILE = nutshellqt.rc
 CONFIG += precompile_header
 PRECOMPILED_HEADER = stable.h

