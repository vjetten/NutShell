
#include "nutshellQt.h"

//---------------------------------------------------------------------------
nutshellMapdisplay::nutshellMapdisplay(QWidget *parent) :
    QMainWindow(parent)
//    ui(new Ui::nutshellMapdisplay)
{
    //ui->setupUi(this);
    	setupUi(this);
}
//---------------------------------------------------------------------------
nutshellMapdisplay::~nutshellMapdisplay()
{
    //delete ui;
}
//---------------------------------------------------------------------------
void nutshellMapdisplay::initMapPlot()
{
   drawMapType = 3;

   maxAxis1 = -1e20;
   maxAxis2 = -1e20;
   maxAxis3 = -1e20;
}
//---------------------------------------------------------------------------
void nutshellMapdisplay::setupMapPlot()
{
   title.setText("");
   title.setFont(QFont("MS Shell Dlg 2",12));
   MPlot = new QwtPlot(title, this);
   // make the plot window
   Layout_Map->insertWidget(0, MPlot, 1);
   MPlot->canvas()->setBorderRadius( 0 );

   // attach plot to widget in UI

   QwtPlotGrid *grid = new QwtPlotGrid();
   grid->setPen( QPen( Qt::DotLine ) );
   grid->attach( MPlot );

   drawMap = new QwtPlotSpectrogram();
   drawMap->setRenderThreadCount( 0 ); // use system specific thread count
   drawMap->attach( MPlot );
   // make the raster drawing

   RD = new QwtMatrixRasterData();
   // raster data to link to plot

   rightAxis = new QwtScaleWidget();
   rightAxis = MPlot->axisWidget( MPlot->yRight );
   rightAxis->setColorBarEnabled( true );
   rightAxis->setColorBarWidth( 20 );
   // legend to the right of the plot

   mapRescaler = new QwtPlotRescaler( MPlot->canvas() );
   mapRescaler->setReferenceAxis( QwtPlot::yLeft );
   mapRescaler->setAspectRatio( QwtPlot::xBottom, 1.0 );
   mapRescaler->setAspectRatio( QwtPlot::yRight, 0.0 );
   mapRescaler->setAspectRatio( QwtPlot::xTop, 0.0 );
   mapRescaler->setRescalePolicy( QwtPlotRescaler::Fitting );
   // rescaling fixed to avoid deformation


   QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( MPlot->canvas() );
   magnifier->setAxisEnabled( MPlot->yRight, false );

   QwtPlotPanner *panner = new QwtPlotPanner( MPlot->canvas() );
   panner->setAxisEnabled( MPlot->yRight, false );

   maxAxis1 = -1e20;
   maxAxis2 = -1e20;
   maxAxis3 = -1e20;

   drawMapType = 3;
}
//---------------------------------------------------------------------------
void nutshellMapdisplay::fillDrawMapData(QString name)
{
   cTMap *M = new cTMap();
   M->PathName = name;
   bool res = M->LoadFromFile();
   if (!res)
       Error("oei");
   M->ResetMinMax();
   drawMapMap = M;

   mapData.clear();
   // copy map data into vector for the display structure
   for(int r = M->nrRows-1; r >= 0; r--)
      for(int c=0; c < M->nrCols; c++)
      {
         if(!IS_MV_REAL8(&M->Data[r][c]))
         {
            mapData += M->Data[r][c];
         }
         else
            mapData << (double)-1e20;
      }

   // set intervals for rasterdata, x,y,z min and max
   RD->setValueMatrix( mapData, M->nrCols );
   // set column number to divide vector into rows
   RD->setInterval( Qt::XAxis, QwtInterval( 0, (double)M->nrCols, QwtInterval::ExcludeMaximum ) );
   RD->setInterval( Qt::YAxis, QwtInterval( 0, (double)M->nrRows, QwtInterval::ExcludeMaximum ) );
   // set x/y axis intervals
}

//---------------------------------------------------------------------------
void nutshellMapdisplay::ShowMap()
{
   fillDrawMapData(mapDisplayNames);
   double MaxV = (double)drawMapMap->MH.maxVal;
   double MinV = (double)drawMapMap->MH.minVal;
   double nrCols = (double)drawMapMap->MH.nrCols;
   double nrRows = (double)drawMapMap->MH.nrRows;


   if (drawMapType == 1)
   {
      MPlot->setTitle("Runoff (l/s)");
      drawMap->setColorMap(new colorMapWaterLog());
      maxAxis1 = qMax(maxAxis1, MaxV);
//      if (doubleSpinBoxRO->value() > 0)
//         maxAxis1 = doubleSpinBoxRO->value();
      RD->setInterval( Qt::ZAxis, QwtInterval( 0, qMax(0.1, maxAxis1)));
   }
   else
      if (drawMapType == 2)
      {
         MPlot->setTitle("Infiltration (mm)");
         drawMap->setColorMap(new colorMapWater());
         maxAxis2 = qMax(maxAxis2, MaxV);
//         if (doubleSpinBoxINF->value() > 0)
//            maxAxis2 = doubleSpinBoxINF->value();
         RD->setInterval( Qt::ZAxis, QwtInterval( 0, maxAxis2));
      }
      else
         if (drawMapType == 3)
         {
            MPlot->setTitle("Soil loss (kg/cell)");
            drawMap->setColorMap(new colorMapSed());
            maxAxis3 = qMax(maxAxis3, MaxV);
//            if (doubleSpinBoxSL->value() > 0)
//               maxAxis3 = doubleSpinBoxSL->value();
//            RD->setInterval( Qt::ZAxis, QwtInterval( -maxAxis3, maxAxis3));
            RD->setInterval( Qt::ZAxis, QwtInterval( MinV, maxAxis3));
            // use max and -max for sediemnt delivery so that white legend color is in the middle, no activity
            // cyan is deposition and dark orange is erosion
         }
   drawMap->setData(RD);
   // link raster data to drawMap

   // add legend right of axis
   if (drawMapType == 1)
   {
      // log scale for runoff
      rightAxis->setColorMap( drawMap->data()->interval( Qt::ZAxis ), new colorMapWaterLog());
      if (maxAxis1 < 100)
         MPlot->setAxisScale( MPlot->yRight, 0.1, qMax(1.0,maxAxis1));
      else
         MPlot->setAxisScale( MPlot->yRight, 1.0, qMax(10.0,maxAxis1));
      MPlot->setAxisScaleEngine( MPlot->yRight, new QwtLog10ScaleEngine() );
   }
   else
      if (drawMapType == 2)
      {
         // lin scale for infiltration
         rightAxis->setColorMap( drawMap->data()->interval( Qt::ZAxis ), new colorMapWater());
         MPlot->setAxisScale( MPlot->yRight, 0.0, maxAxis2);
         MPlot->setAxisScaleEngine( MPlot->yRight, new QwtLinearScaleEngine() );
      }
      else
         if (drawMapType == 3)
         {
            //lin scale with mirrored max and -max for soil loss
            rightAxis->setColorMap( drawMap->data()->interval( Qt::ZAxis ), new colorMapSed());
//            MPlot->setAxisScale( MPlot->yRight, -maxAxis3, maxAxis3);
            MPlot->setAxisScale( MPlot->yRight, MinV, maxAxis3);
            MPlot->setAxisScaleEngine( MPlot->yRight, new QwtLinearScaleEngine() );
         }
   MPlot->enableAxis( MPlot->yRight );

   MPlot->plotLayout()->setAlignCanvasToScales( true );

   MPlot->setAxisScale( MPlot->xBottom, 0.0, nrCols, nrCols/20);
   MPlot->setAxisMaxMinor( MPlot->xBottom, 0 );
   MPlot->setAxisScale( MPlot->yLeft, 0.0, nrRows, nrRows/20);
   MPlot->setAxisMaxMinor( MPlot->yLeft, 0 );


   mapRescaler->setEnabled( true );
   for ( int axis = 0; axis < QwtPlot::axisCnt; axis++ )
      mapRescaler->setExpandingDirection( QwtPlotRescaler::ExpandUp );

   MPlot->replot();
   mapRescaler->rescale();

   drawMapMap->KillMap();
   drawMapMap = NULL;
}
//---------------------------------------------------------------------------
