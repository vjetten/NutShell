#ifndef NUTSHELLMAPDISPLAY_H
#define NUTSHELLMAPDISPLAY_H

#include <QMainWindow>
//QWT library files
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_legend.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_color_map.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_plot_layout.h>
#include <qwt_matrix_raster_data.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_rescaler.h>
#include <qwt_scale_engine.h>

#include "CsfMap.h"
#include "ui_nutshellMapdisplay.h"

//http://geography.uoregon.edu/datagraphics/color_scales.htm
class colorMapWaterLog: public QwtLinearColorMap
{
public:
   colorMapWaterLog():
      QwtLinearColorMap( QColor("#bbbbbb"), Qt::darkBlue  )
   {
      addColorStop( 0.0, Qt::yellow );
      addColorStop( 0.05, QColor("#FFFF55") );
      addColorStop( 0.1, QColor("#8080FF") );
      addColorStop( 0.5, Qt::blue );
   }
};

class colorMapWater: public QwtLinearColorMap
{
public:
   colorMapWater():
      QwtLinearColorMap( QColor("#bbbbbb"), Qt::darkBlue  )
   {
      addColorStop( 0.0, Qt::yellow );
      addColorStop( 0.2, QColor("#FFFF55") );
      addColorStop( 0.6, QColor("#8080FF") );
      addColorStop( 0.9, Qt::blue );
   }
};

class colorMapSed: public QwtLinearColorMap
{
public:
   colorMapSed():
      QwtLinearColorMap( QColor("#bbbbbb"),QColor("#AA0000") )//Qt::red)QColor("#903000") )//QColor("#cc3000"));//Qt::darkYellow);
   {
      addColorStop( 0.0, QColor("#108030"));//Qt::darkCyan );//
      addColorStop( 0.3, Qt::cyan );//QColor("#30ffcc"));
      addColorStop( 0.5, Qt::white );
      addColorStop( 0.7, Qt::yellow);
   }
};

class colorMapRedBlue: public QwtLinearColorMap
{
public:
   colorMapRedBlue():
      QwtLinearColorMap( QColor("#eeeeee"),QColor(220,  0,  0))
   {
       addColorStop(0.00,  QColor(0  ,   0,   220));
       addColorStop(0.25,  QColor(0  ,   250, 250));
       addColorStop(0.50,  QColor(0  ,   250,   0));
       addColorStop(0.75,  QColor(250,   250,   0));
   }
};

//       addColorStop(0.000,  QColor(  7 ,  90,255,255));
//       addColorStop(0.077,  QColor( 50 , 118,255,255));
//       addColorStop(0.154,  QColor( 89 , 144,255,255));
//       addColorStop(0.231,  QColor(140 , 178,255,255));
//       addColorStop(0.308,  QColor(191 , 212,255,255));
//       addColorStop(0.385,  QColor(229 , 238,255,255));
//       addColorStop(0.462,  QColor(247 , 249,255,255));
//       addColorStop(0.538,  QColor(255 , 255,204,255));
//       addColorStop(0.615,  QColor(255 , 255,153,255));
//       addColorStop(0.692,  QColor(255 , 255,  0,255));
//       addColorStop(0.769,  QColor(255 , 204,  0,255));
//       addColorStop(0.846,  QColor(255 , 153,  0,255));
//       addColorStop(0.923,  QColor(255 , 102,  0,255));
//       addColorStop(1.000,  QColor(255 ,   0,  0,255));

class nutshellMapdisplay : public QMainWindow, private Ui::nutshellMapdisplay
{
    Q_OBJECT
    
public:
    explicit nutshellMapdisplay(QWidget *parent = 0);
    ~nutshellMapdisplay();

    //======================
    // Map drawing variable
    //======================
    void setupMapPlot();
    void initMapPlot();
    void ShowMap();
    void fillDrawMapData(QString name);
    void killMapPlot();
    QwtText title;
    QwtPlotSpectrogram *drawMap;  // raster map drawing
    QwtPlot *MPlot;               // plot in which the raster map is drawn
    QwtMatrixRasterData *RD;
    QVector<double> mapData;
    QwtInterval legend;
    QwtScaleWidget *rightAxis;
    QwtPlotRescaler *mapRescaler;
    double maxAxis1, maxAxis2, maxAxis3;
    cTMap *drawMapMap;
    int drawMapType;
    QString ErrorString;
    QString mapDisplayNames;

private:
    Ui::nutshellMapdisplay *ui;
};

#endif // NUTSHELLMAPDISPLAY_H


