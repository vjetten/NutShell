#-------------------------------------------------
#
# Project created by QtCreator 2010-07-04T08:04:34
#
#-------------------------------------------------

QT       += core gui
TARGET = nutshell
TEMPLATE = app
CONFIG += exceptions
SOURCES += main.cpp\
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
    nutslellCommandwindow.cpp \
    nutshellhelp.cpp \
    nutshelloptions.cpp \
    findreplaceform.cpp \
    CsfMap.cpp
HEADERS  += nutshellqt.h \
    csfattr.h \
    csfimpl.h \
    csftypes.h \
	 csf.h \
    nutshelllegend.h \
    nutshellmapattribute.h \
    nutshelleditor.h \
    nutshellhighlighter.h \
    stable.h \
    nutshellhelp.h \
    nutshelloptions.h \
    findreplaceform.h \
    CsfMap.h
FORMS    += nutshellqt.ui \
    nutshelllegend.ui \
    nutshellmapattribute.ui \
    nutshellhelp.ui \
    nutshelloptions.ui \
    findreplaceform.ui \
    nutshellmapdisplay.ui
CONFIG(debug, debug|release) {
  # debug build
    LIBS += -L"debug" -llibcsfd
    DESTDIR = debug
	 MOC_DIR = debug/moc
    OBJECTS_DIR= debug/objs
	 UI_DIR= debug/ui
    RCC_DIR= debug/rcc
} else {
  # release build
  win32:win32-msvc2010{
                 # compile release with MSVC 2010
    DEFINES += _CRT_SECURE_NO_WARNINGS
    LIBS += -L"release/vc" -L"D:/prgc/libcsf/bin" -lcsfvc
    DESTDIR = release/vc
  } else {
                 # compile with mingw
    LIBS += -L"release" -llibcsf
    DESTDIR = release
  }
  MOC_DIR = release/moc
  OBJECTS_DIR= release/objs
  UI_DIR= release/ui
  RCC_DIR= release/rcc
}
RESOURCES += nutshellqt.qrc
RC_FILE = nutshellqt.rc
CONFIG += precompile_header
PRECOMPILED_HEADER = stable.h

