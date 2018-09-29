#include "nutshellqt.h"

//---------------------------------------------------------------------------
nutshellLegend::nutshellLegend(QWidget *parent) :
   QDialog(parent)
{
   setupUi(this);

   connect(toolButton_clearall, SIGNAL(clicked()), this, SLOT(ClearAll()));
   connect(toolButton_deletenrs, SIGNAL(clicked()), this, SLOT(RemoveNumbers()));
   connect(toolButton_addnrs, SIGNAL(clicked()), this, SLOT(AddNumbers()));
//   connect(pushButton_accept, SIGNAL(clicked()), this, SLOT(Accept()));
  // connect(pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
   //connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));
   connect(buttonBox, SIGNAL(accepted()), this, SLOT(Accept()));
   connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));

//   theLegend = NULL;
   legendmodel = NULL;
}
//---------------------------------------------------------------------------
nutshellLegend::~nutshellLegend()
{
}
//---------------------------------------------------------------------------
int nutshellLegend::CountClasses(MAP *in)
{
   int r, c;
   INT4 *Data;
   int nrCols = (int)RgetNrCols(in);
   int nrRows = (int)RgetNrRows(in);

   RuseAs(in, CR_INT4);
   Data = new INT4[(int)RgetNrCols(in)];

   list.clear();

   for (r = 0; r < nrRows; r++)
   {
      RgetSomeCells(in, (UINT4)r*nrCols, (UINT4)nrCols, Data);
      for (c = 0; c < nrCols; c++)
         if(!IS_MV_INT4(&Data[c]))
         {
            if(!list.contains(Data[c]))
               list << (long)Data[c];
         }
   }

   delete[] Data;

   qSort(list.begin(), list.end());

   return(list.count());
}
//---------------------------------------------------------------------------
void nutshellLegend::makelegend(QString name)
{
   int sizel1;
   bool empty;
   CSF_LEGEND *theLegend;

   theLegend = NULL;

   legendmodel = new QStandardItemModel(0, 2, this);
   legendmodel->setHeaderData(0, Qt::Horizontal, QObject::tr("Unit Nr"));
   legendmodel->setHeaderData(1, Qt::Horizontal, QObject::tr("Legend entry"));

   filename = name;

   QApplication::setOverrideCursor(Qt::WaitCursor);

   MAP *legendmap = Mopen(name.toLatin1()
.data(),M_READ);

   if (legendmap == NULL)
   {
      ErrorMsg("Error reading the legend information.")
            Mclose(legendmap);
      return;
   }

   sizel1 = CountClasses(legendmap);
   sizel = MgetNrLegendEntries(legendmap);

   empty = sizel == 0;
   if (empty)
      sizel = sizel1+1; //add one for the map title

//   theLegend = (CSF_LEGEND *)malloc(sizeof(CSF_LEGEND)*sizel);
   theLegend = new CSF_LEGEND[sizel];
   memset(theLegend, 0, sizeof(CSF_LEGEND)*sizel);


   QApplication::restoreOverrideCursor();

   if (!empty && !MgetLegend(legendmap, theLegend))
   {
      ErrorMsg("Could not read the legend.");
      Mclose(legendmap);
      return;
   }

   Mclose(legendmap);

   if (empty)
   {
      theLegend[0].descr[0] = '\0';
      for (int i = 0; i < sizel-1; i++)
      {
         theLegend[i+1].nr = list.at(i);
         theLegend[i+1].descr[0] = '\0';
      }
   }

   legendTitle->setText(theLegend[0].descr);
   for (int i = 0; i < sizel-1; i++)
   {
      QString s;
      QStandardItem* item1 = new QStandardItem(s.setNum(theLegend[i+1].nr));
      legendmodel->setItem(i, 0, item1);
      QStandardItem* item2 = new QStandardItem(QString(theLegend[i+1].descr));
      legendmodel->setItem(i, 1, item2);
   }
   prevrowcount = legendmodel->rowCount();
   legendView->setModel(legendmodel);

   if (theLegend)
   {
       delete[] theLegend;
       theLegend = NULL;
   }
}
//---------------------------------------------------------------------------
void nutshellLegend::AddNumbers()
{
   for (int i = 0; i < sizel-1; i++)
   {
      QModelIndex index = legendmodel->index(i, 1, QModelIndex());
      QString text = legendmodel->data(index, Qt::DisplayRole).toString();
      text = QString("[%1] %2").arg(list.at(i)).arg(text);
      legendmodel->setData(index, text);
   }
}
//---------------------------------------------------------------------------

void nutshellLegend::RemoveNumbers()
{
   for (int i = 0; i < sizel-1; i++)
   {
      QModelIndex index = legendmodel->index(i, 1, QModelIndex());
      QString text = legendmodel->data(index, Qt::DisplayRole).toString();
      int j = text.indexOf("]") + 1;
      text.remove(0,j);
      legendmodel->setData(index, text);
   }
}
//---------------------------------------------------------------------------

void nutshellLegend::ClearAll()
{
   for (int i = 0; i < sizel-1; i++)
   {
      QModelIndex index = legendmodel->index(i, 1, QModelIndex());
      legendmodel->setData(index, QString(""));
   }
   legendTitle->setText("");
}
//---------------------------------------------------------------------------
void nutshellLegend::Accept()
{
   MAP *in;
   CSF_LEGEND *theLegend;
//   theLegend = (CSF_LEGEND *)malloc(sizeof(CSF_LEGEND)*sizel);
   theLegend = new CSF_LEGEND[sizel];

   memset(theLegend, 0, sizeof(CSF_LEGEND)*sizel);

   //copy the header to theLegend
   strncpy(theLegend[0].descr,legendTitle->text().toLatin1()
,63);
   theLegend[0].nr = 0;

   //copy the entries to theLegend
   for (int i = 0; i < sizel-1; i++)
   {
      QModelIndex index = legendmodel->index(i, 1, QModelIndex());
      QString text = legendmodel->data(index, Qt::DisplayRole).toString();
      strncpy(theLegend[i+1].descr, text.toLatin1()
,63);

      index = legendmodel->index(i, 0, QModelIndex());
      theLegend[i+1].nr = legendmodel->data(index, Qt::DisplayRole).toInt();
   }

   // put theLegend in the map
   in = Mopen(filename.toLatin1()
.data(),M_READ_WRITE);
   if (in == NULL)
   {
      ErrorMsg("Cannot write legend to map");
//      if (theLegend)
//      {
//          delete[] theLegend;
//          theLegend = NULL;
//      }
      return;
   }

   MputLegend(in, theLegend, sizel);
   Mclose(in);
//   if (theLegend)
//   {
//       delete[] theLegend; //free(theLegend);
//       theLegend = NULL;
//   }
hide();
}

//---------------------------------------------------------------------------
void nutshellLegend::closeEvent(QCloseEvent *event)
{
//
}
//---------------------------------------------------------------------------
void nutshellLegend::hideEvent(QHideEvent *event)
{
//
}
