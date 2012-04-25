#-------------------------------------------------
#
# Project created by QtCreator 2010-07-04T08:04:34
#
#-------------------------------------------------

QT       += core gui
TARGET = nutshellQt
TEMPLATE = app
QWTDIR = c:/Qt/qwt
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
    CsfMap.cpp \
    nutshellmapdisplay.cpp
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
    CsfMap.h \
    nutshellmapdisplay.h
FORMS    += nutshellqt.ui \
    nutshelllegend.ui \
    nutshellmapattribute.ui \
    nutshellhelp.ui \
    nutshelloptions.ui \
    findreplaceform.ui \
    nutshellmapdisplay.ui
CONFIG(debug, debug|release) {
    LIBS += -L"debug" \
        -llibcsfd
    LIBS += -L"$${QWTDIR}/lib" -lqwtd
    DESTDIR = debug
	 MOC_DIR = debug/moc
    OBJECTS_DIR= debug/objs
	 UI_DIR= debug/ui
    RCC_DIR= debug/rcc
}
else {
    LIBS += -L"release" \
        -llibcsf
    LIBS += -L"$${QWTDIR}/lib" -lqwt
    DESTDIR = release
	 MOC_DIR = release/moc
    OBJECTS_DIR= release/objs
	 UI_DIR= release/ui
    RCC_DIR= release/rcc
}
RESOURCES += \
    nutshellqt.qrc
RC_FILE = nutshellqt.rc
CONFIG += precompile_header
INCLUDEPATH += $${QWTDIR}/src
PRECOMPILED_HEADER = stable.h


