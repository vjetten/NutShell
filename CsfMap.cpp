/*
 * csfmap: used in the NutSHhell Explorer

 * Author: VJ 1210112
 */

/*!
 \file CsfMap.cpp
 \brief file operations  class for PCRaster maps.

  provide basic functionality to read and write PCRaster CSF maps, \n
  can be altered to link to other file formats.

 functions: \n
 - void cTMap::KillMap(void)  \n
 - void cTMap::GetMapHeader(QString Name)  \n
 - void cTMap::CreateMap(QString Name) \n
 - bool cTMap::LoadFromFile() \n
 - void cTMap::MakeMap(cTMap *dup, REAL8 value) \n
 - void cTMap::ResetMinMax(void) \n
 - void cTMap::WriteMap(QString Name) \n
 - void cTMap::WriteMapSeries(QString Dir, QString Name, int count) \n
*/


#include "CsfMap.h"
//#include "error.h"


//---------------------------------------------------------------------------
cTMap::cTMap()
{
   Data = NULL;
   Created = false;
   nrRows = 0;
   nrCols = 0;
   MapName = "";
   PathName = "";
}
//---------------------------------------------------------------------------
cTMap::~cTMap()
{
   KillMap();
}
//---------------------------------------------------------------------------
void cTMap::KillMap()
{
   if (Data && nrRows > 0)
   {
      for (int r=0; r < nrRows; r++)
         delete[] Data[r];
      delete[] Data;
      Data = NULL;
   }
   Created = false;
}
//---------------------------------------------------------------------------
void cTMap::GetMapHeader(QString Name)
{
   MAP *m = Mopen(Name.toLatin1(), M_READ);
//   originalPixmap.save(fileName, format.toLatin1());

   if (m == NULL)
      Error(QString("Map %1 cannot be opened.").arg(Name));
//   {
//      Error(QString("Map %1 cannot be opened.").arg(Name));
//      throw 1;
//   }

   MH = m->raster;
   projection = m->main.projection;

   // easier to separate these
   nrRows = (int)MH.nrRows;
   nrCols = (int)MH.nrCols;

   Mclose(m);
}
//---------------------------------------------------------------------------
void cTMap::CreateMap(QString Name)
{
   // if exist delete main map data structure and set Created to false
   if (Created)
      KillMap();

   // now get the header for nrrows and nrcols
   GetMapHeader(Name);

   Data = new REAL8*[nrRows];
   for(int r=0; r < nrRows; r++)
      Data[r] = new REAL8[nrCols];

   if (Data == NULL)
      Error(QString("Cannot create data structure for map: %1").arg(Name));
//   {
//      Error(QString("Cannot create data structure for map: %1").arg(Name));
//      throw 1;
//   }

   Created = true;
}
//---------------------------------------------------------------------------
bool cTMap::LoadFromFile()
{
   MAP *m;
   QFileInfo fi(PathName);

   if (!fi.exists())
      Error(QString("Map %1 does not exist.").arg(PathName));
//   {
//      Error(QString("Map %1 does not exist.").arg(PathName));
//      throw 1;
//   }

   // make map structure
   CreateMap(PathName);

   if (!Created)
      return(false);

   MapName = PathName;

   m = Mopen(MapName.toLatin1()
.constData(), M_READ);

   if (!m)
      return(false);

   RuseAs(m, CR_REAL8); //RgetCellRepr(m));
   for(int r=0; r < nrRows; r++)
      RgetSomeCells(m, (UINT4)r*nrCols, (UINT4)nrCols, Data[r]);

   if (RgetCellRepr(m) == CR_REAL8)
      ResetMinMax();

   Mclose(m);
   return(true);
}
//---------------------------------------------------------------------------
void cTMap::MakeMap(cTMap *dup, REAL8 value)
{
   if (dup == NULL)
      return;
   //MH = dup->MH;
   MH.minVal = value;
   MH.maxVal = value;
   nrRows = dup->nrRows;
   nrCols = dup->nrCols;
   MH.nrRows = nrRows;
   MH.nrCols = nrCols;
   MH.valueScale = dup->MH.valueScale;
   MH.cellRepr = dup->MH.cellRepr;
   MH.xUL = dup->MH.xUL;
   MH.yUL = dup->MH.yUL;
   MH.cellSize  = dup->MH.cellSize;
   //rMH.cellSizeY  = dup->MH.cellSize;
   MH.angle = dup->MH.angle;
   projection = dup->projection;

   Data = new REAL8*[nrRows];
   for(int r=0; r < nrRows; r++)
      Data[r] = new REAL8[nrCols];

   if (Data == NULL)
      return;

   for(int r = 0; r < nrRows; r++)
      SetMemMV(Data[r],nrCols,CR_REAL8);

   for(int r=0; r < nrRows; r++)
      for(int c=0; c < nrCols; c++)
         if (!IS_MV_REAL8(&dup->Data[r][c]))
         {
            Data[r][c] = value;
         }

   Created = true;
}
//---------------------------------------------------------------------------
void cTMap::ResetMinMax(void)
{
   REAL8 minv = 1e30, maxv = -1e30;

   if (!Created)
      return;

   for(int r=0; r < nrRows; r++)
      for(int c=0; c < nrCols; c++)
         if (!IS_MV_REAL8(&Data[r][c]))
         {
            if (maxv < Data[r][c]) maxv = Data[r][c];
            if (minv > Data[r][c]) minv = Data[r][c];
         }

   MH.minVal = minv;
   MH.maxVal = maxv;
}
//---------------------------------------------------------------------------
void cTMap::WriteMap(QString Name)
{
   MAP *out;
   long r, c;
   REAL4 *Dt;

   if (!Created)
      return;

   if (Name.isEmpty())
   {
      ErrorString = "Cannot write file, file name empty";
      throw 1;
   }

   ResetMinMax();

   Dt = new REAL4[nrCols];
   // make an array for output

   MH.cellRepr = CR_REAL4;
   out = Rcreate(Name.toLatin1().constData(),nrRows, nrCols, (CSF_CR)MH.cellRepr, VS_SCALAR,
                 (CSF_PT)projection, MH.xUL, MH.yUL, MH.angle, MH.cellSize);
   RuseAs(out, CR_REAL4);

   for(r=0; r < nrRows; r++)
   {
      for(c=0; c < nrCols; c++)
         Dt[c] = (REAL4)Data[r][c];


      if (RputRow(out, r, Dt) != (UINT4)nrCols)
      {
         ErrorString = "rputrow write error with" + Name;
    	   throw 1;
      }
   }

   delete Dt;

   Mclose(out);

}
//---------------------------------------------------------------------------
void cTMap::WriteMapSeries(QString Dir, QString Name, int count)
{
   QString path;
   QString nam, dig;
   QFileInfo fi(Name);

   if (Name.indexOf(".") < 0)
   {

    	nam = Name + "00000000";

    	nam.remove(8, 80);
    	dig.setNum(count);

    	if (count > 999)
    	{
    		nam.remove(8,1);
    		nam = nam + dig;
    		nam.insert(9,QString("."));
    	}
    	else
    		if (count > 99)
    			nam = nam + "." + dig;
    		else
    			if (count > 9)
    				nam = nam + "." + "0" + dig;
    			else
    				nam = nam + "." + "00" + dig;
   }
   else
      nam = Name;

   path = Dir + nam;
   WriteMap(path);
}


