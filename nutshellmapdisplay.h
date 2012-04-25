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

//namespace Ui {
//class nutshellMapdisplay;
//}
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


