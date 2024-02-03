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
    nutshellProcess.cpp \
    nutshellhelp.cpp \
    nutshelloptions.cpp \
    findreplaceform.cpp \
    CsfMap.cpp \
    nutshellevent.cpp \
    nutshelltreeview.cpp \
    nutshellCommandwindow.cpp \
    pcrlib/angle.c \
    pcrlib/attravai.c \
    pcrlib/attrsize.c \
    pcrlib/cellsize.c \
    pcrlib/create2.c \
    pcrlib/csfglob.c \
    pcrlib/csfsup.c \
    pcrlib/delattr.c \
    pcrlib/dumconv.c \
    pcrlib/endian.c \
    pcrlib/file.c \
    pcrlib/filename.c \
    pcrlib/gattrblk.c \
    pcrlib/gattridx.c \
    pcrlib/gcellrep.c \
    pcrlib/gdattype.c \
    pcrlib/getattr.c \
    pcrlib/getx0.c \
    pcrlib/gety0.c \
    pcrlib/ggisfid.c \
    pcrlib/gmaxval.c \
    pcrlib/gminval.c \
    pcrlib/gnrcols.c \
    pcrlib/gnrrows.c \
    pcrlib/gproj.c \
    pcrlib/gputproj.c \
    pcrlib/gvalscal.c \
    pcrlib/gvartype.c \
    pcrlib/gversion.c \
    pcrlib/ismv.c \
    pcrlib/kernlcsf.c \
    pcrlib/legend.c \
    pcrlib/mclose.c \
    pcrlib/mopen.c \
    pcrlib/moreattr.c \
    pcrlib/mperror.c \
    pcrlib/pgisfid.c \
    pcrlib/pmaxval.c \
    pcrlib/pminval.c \
    pcrlib/putallmv.c \
    pcrlib/putattr.c \
    pcrlib/putsomec.c \
    pcrlib/putx0.c \
    pcrlib/puty0.c \
    pcrlib/pvalscal.c \
    pcrlib/rattrblk.c \
    pcrlib/rcomp.c \
    pcrlib/rcoords.c \
    pcrlib/rdup2.c \
    pcrlib/reseterr.c \
    pcrlib/rextend.c \
    pcrlib/rmalloc.c \
    pcrlib/rrowcol.c \
    pcrlib/ruseas.c \
    pcrlib/setangle.c \
    pcrlib/setmv.c \
    pcrlib/setvtmv.c \
    pcrlib/strconst.c \
    pcrlib/strpad.c \
    pcrlib/swapio.c \
    pcrlib/trackmm.c \
    pcrlib/vs2.c \
    pcrlib/vsdef.c \
    pcrlib/vsis.c \
    pcrlib/vsvers.c \
    pcrlib/wattrblk.c \
    pcrlib/_getcell.c \
    pcrlib/_getrow.c \
    pcrlib/_gsomece.c \
    pcrlib/_putcell.c \
    pcrlib/_rputrow.c

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

FORMS    += nutshellqt.ui \
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
INCLUDEPATH += C:\Qt\qwtma\src
#LIBS += c:/qt/qwtma/lib/libqwt.dll.a
#INCLUDEPATH += C:\Qt\msys64\mingw64\include\qwt-qt5
LIBS += C:\Qt\qwt-6.1-ma\lib\libqwt.dll.a
LIBS += c:/qt/msys64/mingw64/lib/libgdal.dll.a


RESOURCES +=  nutshellqt.qrc
RC_FILE = nutshellqt.rc
