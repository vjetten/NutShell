#include "nutshellqt.h"

#define MAX_DEG ((double)360.0)
//#define M_PI            3.14159265358979323846
#define M_2PI           ((double)(2*M_PI))


//---------------------------------------------------------------------------
nutshellmapattribute::nutshellmapattribute(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(Accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    a = (ATTRIBUTES *)malloc(sizeof(ATTRIBUTES));

}
//---------------------------------------------------------------------------
nutshellmapattribute::~nutshellmapattribute()
{
   if (a)
   {
      free (a);
      a = NULL;
   }
}
//---------------------------------------------------------------------------
double ScaleDeg(double x)
{
    int neg = (x < 0);
    x = fabs(x);
    x = fmod(x, MAX_DEG);
#ifdef _DEBUG
    {
//        double y = neg ? (MAX_DEG-x) : x;
//        POSTCOND(0 <= y && y < MAX_DEG);
    }
#endif
    return neg ? (MAX_DEG-x) : x;
}
/* scales a value in radians to the domain [0,2PI>
 * returns the scaled value
 */
double ScaleRad(double x)
{
    int neg = (x < 0);
    x = fabs(x);
    x = fmod(x, M_2PI);
#ifdef _DEBUG
    {
//        double y = neg ? (M_2PI-x) : x;
//        POSTCOND(0 <= y && y < M_2PI);
    }
#endif
    return neg ? (M_2PI-x) : x;
}
/* convert a radian value to degree value
 * returns scaled result in domain [0,360>
 */
double Rad2Deg(double x) /* value in radians */
{
    x = ScaleRad(x);
#ifdef _DEBUG
    {
//        double y = ((x)/M_2PI) * MAX_DEG;
//        POSTCOND(0 <= y && y < MAX_DEG);
    }
#endif
    return (double)((((long double)(x))/(2*((long double)M_PI))) * MAX_DEG);
}
/* convert a degree value to radian value
 * returns scaled result in domain [0,2PI>
 */
double Deg2Rad(double x) /* value in degrees */
{
    x = ScaleDeg(x);
#ifdef _DEBUG
    {
//        double y = ((x)/MAX_DEG) * M_2PI;
//        POSTCOND(0 <= y && y < M_2PI);
    }
#endif
    return ((x)/MAX_DEG) * M_2PI;
}
//---------------------------------------------------------------------------
int nutshellmapattribute::fill(QString name, bool newmap)
{
    if (newmap)
    {
        setWindowIcon(QIcon(":/resources/mapnew.png"));
        label_header->setText("Create new map with value 0");
    }
    else
    {
        setWindowIcon(QIcon(":/resources/header.png"));
        label_header->setText("Map header information");
    }

   // a = (ATTRIBUTES *)malloc(sizeof(ATTRIBUTES));
    OptNotSetAttr();

    bool isMap = !(name.contains(".map_vj"));

    makenewmap = newmap;
    filename = name;
    if (newmap && !isMap)
        filename = name.remove(name.count()-3,3);

    lineEdit_mapname->setText("new.map");
    lineEdit_XUL->setText("0");
    lineEdit_YUL->setText("0");
    lineEdit_nrRows->setText("1");
    lineEdit_nrCols->setText("1");
    lineEdit_celllength->setText("1");
    lineEdit_angle->setText("0");
    lineEdit_ID->setText("0");
    radioButton_YBotTop->setChecked(true);
    radioButton_scalar->setChecked(true);
    label_minValue->text().setNum(0);
    label_maxValue->text().setNum(0);

    if (isMap)
        m = Mopen(filename.toLatin1().data(),M_READ_WRITE);
//    if (!filename.isEmpty() && m == NULL &&!newmap)
//    {
//        Mclose(m); m = NULL;
//        ErrorMsg("Could not open file as PCRaster map.")
//                close();
//        return 1;
//    }

    //the map exists, OR it doesn't and newmap

    // if not newmap make the selected file the mapname
    lineEdit_nrRows->setEnabled(newmap);
    lineEdit_nrCols->setEnabled(newmap);
    groupBox_datatype->setEnabled(newmap);
    lineEdit_ID->setEnabled(newmap);
    lineEdit_mapname->setEnabled(newmap);

//    if (!newmap)
//       lineEdit_mapname->setText(QFileInfo(filename).fileName());
    if (!newmap && isMap)
    {
        lineEdit_mapname->setText(QFileInfo(filename).fileName());
        int ret = ReadAttr(true);
        if (ret == 1)
            DefaultAttr();
    }
    else
    {
        DefaultAttr();
        if (isMap)
           ReadAttr(true);
    }

    QString s;
    lineEdit_XUL->setText( s.setNum((double)a->xUL,'f',6));
    lineEdit_YUL->setText( s.setNum((double)a->yUL,'f',6));
    lineEdit_nrRows->setText( s.setNum((long)a->nrRows));
    lineEdit_nrCols->setText( s.setNum((long)a->nrCols));
    lineEdit_celllength->setText( s.setNum((double)a->cellSize,'g',6));
    if (fabs(a->angle) < 1e-20)
        a->angle = 0;
    lineEdit_angle->setText( s.setNum((double)a->angle,'g',6));
    switch (a->valueScale){
    case VS_BOOLEAN : radioButton_boolean->setChecked(true); break;
    case VS_NOMINAL : radioButton_nominal->setChecked(true); break;
    case VS_ORDINAL : radioButton_ordinal->setChecked(true); break;
    case VS_SCALAR  : radioButton_scalar->setChecked(true); break;
    case VS_DIRECTION : radioButton_directional->setChecked(true); break;
    case VS_LDD     : radioButton_ldd->setChecked(true); break;

    case VS_CLASSIFIED : radioButton_nominal->setChecked(true); break;
    case VS_CONTINUOUS :
    case VS_NOTDETERMINED :
    case VS_UNDEFINED : radioButton_scalar->setChecked(true); break;

    }
    if (a->projection == 0)
        radioButton_YTopBot->setChecked(true);
    else
        radioButton_YBotTop->setChecked(true);
    lineEdit_ID->setText( s.setNum(a->gisFileId));
    //??????????????
    label_minValue->setText( s.setNum((double)a->minVal,'f',6));
    label_maxValue->setText( s.setNum((double)a->maxVal,'f',6));
    if (newmap)
    {
        label_minValue->setText( s.setNum(0));
        label_maxValue->setText( s.setNum(0));
    }

    Mclose(m); m = NULL;
    return 0;
}
//---------------------------------------------------------------------------
QString nutshellmapattribute::getMapAttributes(QString mapname)
{
    QStringList str;
    m = Mopen(mapname.toLatin1()
.data(),M_READ_WRITE);
    if (!filename.isEmpty() && m == NULL)
        return "";

//    a = (ATTRIBUTES *)malloc(sizeof(ATTRIBUTES));
    OptNotSetAttr();


    if(m != NULL)
    {
        QString s;
        int ret = ReadAttr(true);
        if (ret == 1)
            return "";

        str << "[<font color=\"green\"><i>";
        switch (a->valueScale){
        case VS_BOOLEAN : str << "Boolean"; break;
        case VS_CLASSIFIED :
        case VS_NOMINAL : str << "Nominal"; break;
        case VS_ORDINAL : str << "Ordinal"; break;
        case VS_CONTINUOUS :
        case VS_NOTDETERMINED :
        case VS_UNDEFINED :
        case VS_SCALAR  : str << "Scalar"; break;
        case VS_DIRECTION : str << "Directional"; break;
        case VS_LDD     : str << "LDD"; break;
        }
        str << "</i></font>]";
        str << QString("[rows <font color=\"green\"><i>%1</i></font> : cols <font color=\"green\"><i>%2</i></font>]").arg((long)a->nrRows).arg((long)a->nrCols);
        str << QString("[yUL <font color=\"green\"><i>%1</i></font> : xUL <font color=\"green\"><i>%2</i></font>]").arg((double)a->yUL,0,'f',3).arg((double)a->xUL,0,'f',3);
        str << QString("[dx <font color=\"green\"><i>%1</i></font>]").arg((double)a->cellSize);
        if (fabs(a->angle) < 1e-20)
            a->angle = 0;
        str << QString("[Angle <font color=\"green\"><i>%1</i></font>").arg((double)a->angle);
        if (a->projection == 0)
            str << "Y:<font color=\"green\"><i>Top->Bot</i></font>]";
        else
            str << "Y:<font color=\"green\"><i>Bot->Top</i></font>]";
        str << QString("[Min <font color=\"green\"><i>%1</i></font>").arg((double)a->minVal);
        str << QString("Max <font color=\"green\"><i>%1</i></font>]").arg((double)a->maxVal);
    }
    Mclose(m); m = NULL;
//    if (a) {
//        free (a);
//        a = NULL;
//    }
    return str.join(" ");
}
//---------------------------------------------------------------------------
void nutshellmapattribute::OptNotSetAttr()
{
    a->projection = PT_UNDEFINED;
    a->valueScale = VS_UNDEFINED;
    a->cellRepr = CR_UNDEFINED;
    a->nrRows = MV_UINT4;
    a->nrCols = MV_UINT4;
    SET_MV_REAL8(&(a->xUL));
    SET_MV_REAL8(&(a->yUL));
    SET_MV_REAL8(&(a->cellSize));
    SET_MV_REAL8(&(a->angle));
    a->gisFileId = MV_UINT4;
    SET_MV_REAL8(&(a->minVal));
    SET_MV_REAL8(&(a->maxVal));
}
//---------------------------------------------------------------------------
void nutshellmapattribute::DefaultAttr()
{
    a->version    = 2;
    a->projection = PT_YINCT2B;
    a->valueScale = VS_SCALAR;
    a->cellRepr = CR_REAL4;
    a->xUL = 0;
    a->yUL = 0;
    a->nrRows = 1;
    a->nrCols = 1;
    a->cellSize = 1;
    a->angle = 0;
    a->gisFileId = 0;
    a->minVal = 0;
    a->maxVal = 0;
    SET_MV_REAL8(&(a->minVal));
    SET_MV_REAL8(&(a->maxVal));
}
//---------------------------------------------------------------------------
int nutshellmapattribute::ReadAttr(bool readOnly)
{
    DefaultAttr();

    if (RuseAs(m, CR_REAL8))
        goto failure;

    RgetMinVal(m, &(a->minVal));
    RgetMaxVal(m, &(a->maxVal));
    a->projection = MgetProjection(m);
    a->xUL = RgetXUL(m);
    a->yUL = RgetYUL(m);
    a->nrRows = RgetNrRows(m);
    a->nrCols = RgetNrCols(m);
    a->cellSize = RgetCellSize(m);
    a->version    = MgetVersion(m);
    a->gisFileId = MgetGisFileId(m);
    //	a->byteOrder = m->main.byteOrder;
    //	a->attrTable = m->main.attrTable;
    //	if (Merrno)
    //		goto failure;
    if (a->version == 2 || readOnly)
    { /* otherwise use defaults */
        a->valueScale = RgetValueScale(m);
        a->cellRepr = RgetCellRepr(m);
        a->angle = RgetAngle(m);
        if (a->angle < 0)
            a->angle = -Rad2Deg(-a->angle);
        else
            a->angle = Rad2Deg(a->angle);
    }
    return 0;
failure:
    return 1;
}
//---------------------------------------------------------------------------
void nutshellmapattribute::CreateMap()
{
    REAL4 *buf;
    int i;
    double angle;
    MAP *m;

    if (a->angle < 0)
        angle = -Deg2Rad(-a->angle);
    else
        angle = Deg2Rad(a->angle);

    switch (a->valueScale){
    case VS_BOOLEAN : a->cellRepr = CR_UINT1;break;
    case VS_NOMINAL : a->cellRepr = CR_INT4;break;
    case VS_ORDINAL : a->cellRepr = CR_INT4;break;
    case VS_SCALAR  : a->cellRepr = CR_REAL4;break;
    case VS_DIRECTION : a->cellRepr = CR_REAL4;break;
    case VS_LDD     : a->cellRepr = CR_UINT1;break;
    case VS_CLASSIFIED :  a->cellRepr = CR_INT4;break;
    case VS_CONTINUOUS :
    case VS_NOTDETERMINED :
    case VS_UNDEFINED : a->cellRepr = CR_REAL4;break;
    }

    QString S = QFileInfo(filename).absolutePath() +
            QDir::separator() + lineEdit_mapname->text();
    if (QFileInfo(S).exists())
    {
        int ret = QMessageBox::warning(this,"NutShell new map",QString("File already exists.\nDo you want overwrite?"),
                                       QMessageBox::Save | QMessageBox::Cancel, QMessageBox::Save);
        if (ret == 1)
            return;
    }
    //TODO warnning when map exists

    m = Rcreate(S.toLatin1()
.data(),a->nrRows,a->nrCols, a->cellRepr, a->valueScale,
                a->projection, a->xUL, a->yUL, angle, a->cellSize);
    if (m == NULL)
    {
        ErrorMsg("Could not make map");
        return;
    }

    //	PRECOND(a->gisFileId != MV_UINT4);
    if (MputGisFileId(m,a->gisFileId) == MV_UINT4)
    {
        Mclose(m); m = NULL;
        ErrorMsg("Map write error gisFileID");
        return;
    }

    buf = (REAL4 *)Rmalloc(m, a->nrCols);
    if (buf == NULL)
    {
        Mclose(m); m = NULL;
        ErrorMsg("Map data write error");
        return;
    }

    memset(buf,(REAL4)1.0,a->nrCols);

    RuseAs(m, a->cellRepr);
    for(i=0; i < (int)a->nrRows; i++)
    {
        RputRow(m,(UINT4)i,buf);
    }
    a->minVal = 1.0;
    a->maxVal = 1.0;
    RputMinVal(m, &(a->minVal));
    RputMaxVal(m, &(a->maxVal));
    free(buf);
    Mclose(m);
    m = NULL;
    return;
}
//---------------------------------------------------------------------------
void nutshellmapattribute::SetAndCloseMap()
{
    //	double angle;

    /*	if (! (IS_MV_REAL8(&(a->angle))))
 {
  if (a->angle < 0)
     angle = -Deg2Rad(-a->angle);
  else
     angle = Deg2Rad(a->angle);
 }
*/
    //if (RuseAs(m, CR_REAL8))
    //goto error2;

    m = Mopen(filename.toLatin1().data(),M_READ_WRITE);

    if (a->projection != PT_UNDEFINED)
        MputProjection(m,PT_YDECT2B);//a->projection);
    if (! (IS_MV_REAL8(&(a->xUL))))
        RputXUL(m, a->xUL);
    if (! (IS_MV_REAL8(&(a->yUL))))
        RputYUL(m, a->yUL);
    if (! (IS_MV_REAL8(&(a->angle))))
        RputAngle(m, a->angle);
    if (! (IS_MV_REAL8(&(a->cellSize))))
        RputCellSize(m, a->cellSize);
    if(a->gisFileId != MV_UINT4)
        MputGisFileId(m,a->gisFileId);

    Mclose(m);
//    m = NULL;
//    if (Merrno)
//    {
//        ErrorMsg(QString("Map write error %1").arg(Merrno));
//        return;
//    }
}

//---------------------------------------------------------------------------
void nutshellmapattribute::Accept()
{
   // store or save new with button ok
   if (!makenewmap)
       SetAndCloseMap();
   else
       CreateMap();

}
//---------------------------------------------------------------------------
bool nutshellmapattribute::checkMapAttrib()
{
    a->xUL = (REAL8) lineEdit_XUL->text().toDouble();
    a->yUL = (REAL8) lineEdit_YUL->text().toDouble();
    a->nrRows = (UINT4) lineEdit_nrRows->text().toInt();
    a->nrCols = (UINT4) lineEdit_nrCols->text().toInt();
    a->cellSize = (REAL8) lineEdit_celllength->text().toDouble();
    a->angle = (REAL8) lineEdit_angle->text().toDouble();
    if (radioButton_boolean->isChecked()) a->valueScale = VS_BOOLEAN;
    if (radioButton_nominal->isChecked()) a->valueScale = VS_NOMINAL;
    if (radioButton_ordinal->isChecked()) a->valueScale = VS_ORDINAL;
    if (radioButton_scalar->isChecked()) a->valueScale = VS_SCALAR;
    if (radioButton_directional->isChecked()) a->valueScale = VS_DIRECTION;
    if (radioButton_ldd->isChecked()) a->valueScale = VS_LDD;

    if (radioButton_YTopBot->isChecked()) a->projection = (CSF_PT)0;
    if	(radioButton_YBotTop->isChecked()) a->projection = (CSF_PT)1;

    a->gisFileId = (UINT4) lineEdit_ID->text().toInt();

    if(lineEdit_mapname->text().isEmpty())
    {
        QMessageBox::warning(this,"NutShell new map",QString("Specify a map name to save"));
        return false;
    }
    if (a->nrCols == 0 || a->nrRows == 0 || a->cellSize == 0)
    {
        QMessageBox::warning(this,"NutShell new map",QString("Number of Rows or Cols, or cellsize cannot be 0"));
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
void nutshellmapattribute::hideEvent (QHideEvent *event)
{
   // catch hide in case attribute problem, stay open
    if( !checkMapAttrib() )
    {
        event->ignore();
        show();
    }
}
//---------------------------------------------------------------------------
void nutshellmapattribute::closeEvent(QCloseEvent *event)
{
   //nothing, no save because cancel is pressed
}
//---------------------------------------------------------------------------



