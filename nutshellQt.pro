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
    nutshellCommandwindow.cpp \
    pcrlib/mopen.c \
    pcrlib/angle.c \
    pcrlib/dumconv.c \
    pcrlib/_getrow.c \
    pcrlib/_gsomece.c \
    pcrlib/mperror.c \
    pcrlib/pvalscal.c \
    pcrlib/rrowcol.c \
    pcrlib/vsdef.c \
    pcrlib/attravai.c \
    pcrlib/endian.c \
    pcrlib/getx0.c \
    pcrlib/gvalscal.c \
    pcrlib/pgisfid.c \
    pcrlib/rattrblk.c \
    pcrlib/ruseas.c \
    pcrlib/vsis.c \
    pcrlib/attrsize.c \
    pcrlib/file.c \
    pcrlib/gety0.c \
    pcrlib/gvartype.c \
    pcrlib/pmaxval.c \
    pcrlib/rcomp.c \
    pcrlib/setangle.c \
    pcrlib/vsvers.c \
    pcrlib/filename.c \
    pcrlib/ggisfid.c \
    pcrlib/gversion.c \
    pcrlib/pminval.c \
    pcrlib/rcoords.c \
    pcrlib/setmv.c \
    pcrlib/wattrblk.c \
    pcrlib/cellsize.c \
    pcrlib/gattrblk.c \
    pcrlib/gmaxval.c \
    pcrlib/ismv.c \
    pcrlib/putallmv.c \
    pcrlib/rdup2.c \
    pcrlib/setvtmv.c \
    pcrlib/gattridx.c \
    pcrlib/gminval.c \
    pcrlib/kernlcsf.c \
    pcrlib/putattr.c \
    pcrlib/strconst.c \
    pcrlib/create2.c \
    pcrlib/gcellrep.c \
    pcrlib/gnrcols.c \
    pcrlib/legend.c \
    pcrlib/_putcell.c \
    pcrlib/reseterr.c \
    pcrlib/strpad.c \
    pcrlib/csfglob.c \
    pcrlib/gdattype.c \
    pcrlib/gnrrows.c \
    pcrlib/mclose.c \
    pcrlib/putsomec.c \
    pcrlib/rextend.c \
    pcrlib/swapio.c \
    pcrlib/csfsup.c \
    pcrlib/getattr.c \
    pcrlib/gproj.c \
    pcrlib/putx0.c \
    pcrlib/rmalloc.c \
    pcrlib/trackmm.c \
    pcrlib/delattr.c \
    pcrlib/_getcell.c \
    pcrlib/gputproj.c \
    pcrlib/moreattr.c \
    pcrlib/puty0.c \
    pcrlib/_rputrow.c \
    pcrlib/vs2.c

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

win32 {
    INCLUDEPATH += C:\prgc\lisemgit\qwt\git\src
    INCLUDEPATH += C:\Qt\msys64\mingw64\include
    LIBS += C:/prgc/lisemgit/qwt/git/lib/libqwt.dll.a
    LIBS += c:/qt/msys64/mingw64/lib/libgdal.dll.a
    #LIBS += -L../pcrlib/libpcraster_raster_format.a
}

unix {
    INCLUDEPATH += ./pcrlib
    INCLUDEPATH += /usr/include
    INCLUDEPATH += /usr/include/qwt
    INCLUDEPATH += /usr/include/gdal
    LIBS += -L/usr/local/qwt-6.4.0-ma/lib -lqwt
    LIBS += -L/usr/lib/x86_64-linux-gnu -lgdal
}

RESOURCES +=  nutshellqt.qrc
RC_FILE = nutshellqt.rc
