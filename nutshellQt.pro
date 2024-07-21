#-------------------------------------------------
#
# Project created by QtCreator 2010-07-04T08:04:34
#
#-------------------------------------------------

QT       += core gui widgets
TARGET = nutshell
TEMPLATE = app
CONFIG += exceptions
#QMAKE_CXXFLAGS += -Wall
#CONFIG += static
#CONFIG += warn_off
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
    nutshellProcess.cpp \
    nutshellhelp.cpp \
    nutshelloptions.cpp \
    findreplaceform.cpp \
    CsfMap.cpp \
    nutshellevent.cpp \
    nutshelltreeview.cpp \
    nutshellCommandwindow.cpp


HEADERS  += include/nutshellqt.h \
    include/csfattr.h \
    include/csfimpl.h \
    include/csftypes.h \
    include/csf.h \
    include/nutshelllegend.h \
    include/nutshellmapattribute.h \
    include/nutshelleditor.h \
    include/nutshellhighlighter.h \
    include/nutshellhelp.h \
    include/nutshelloptions.h \
    include/findreplaceform.h \
    include/CsfMap.h \
    include/pcrtypes.h

FORMS  += nutshellqt.ui \
    nutshelllegend.ui \
    nutshellmapattribute.ui \
    nutshelloptions.ui \
    findreplaceform.ui \
    mapedit.ui

#INCLUDEPATH+=$${QTDIR}/lib

CONFIG(debug, debug|release) {
  MOC_DIR = debug/moc
  OBJECTS_DIR= debug/objs
  UI_DIR= debug/ui
  RCC_DIR= debug/rcc
  DESTDIR = debug
} else {
  MOC_DIR = release/moc
  OBJECTS_DIR= release/objs
  UI_DIR= release/ui
  RCC_DIR= release/rcc
  DESTDIR = release
}

INCLUDEPATH += ./include
INCLUDEPATH += C:\Qt\msys64\mingw64\include
#INCLUDEPATH += C:\Qt\msys64\mingw64\include\qwt-qt5
LIBS += C:/prgc/lisemgit/qwt/git/lib/libqwt.dll.a
LIBS += c:/qt/msys64/mingw64/lib/libgdal.dll.a
INCLUDEPATH += C:/prgc/lisemgit/qwt/git/src
#LIBS += -L../pcrlib/libpcraster_raster_format.a

RESOURCES +=  nutshellqt.qrc
RC_FILE = nutshellqt.rc
