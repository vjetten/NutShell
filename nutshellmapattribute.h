#ifndef NUTSHELLMAPATTRIBUTE_H
#define NUTSHELLMAPATTRIBUTE_H

#include <QtGui>
#include "csf.h"
#include "ui_nutshellmapattribute.h"


typedef struct ATTRIBUTES
{
 bool   cloneCreation;
 /* all MV_ if not set: */
 /* not editable existing map: */
 UINT4    nrRows;
 UINT4    nrCols;
 CSF_VS valueScale;
 CSF_CR  cellRepr;
 CSF_PT projection;
/* co-ordinate of upper left corner
 */
 REAL8    xUL;
 REAL8    yUL;

 REAL8    cellSize;
 REAL8    angle;
 /* never editable */
 REAL8  minVal;
 REAL8  maxVal;
 UINT2 version;
 UINT4 gisFileId;
 UINT4 byteOrder;
 UINT4 attrTable;
} ATTRIBUTES;

#define NR_MENU_ATTRS     10

typedef enum ATTR_NRS {
	ATTR_nrRows=0,
	ATTR_nrCols=1,
	ATTR_valueScale=2,
	ATTR_cellRepr=3,
	ATTR_projection=4,
	ATTR_xUL=5,
	ATTR_yUL=6,
	ATTR_cellSize=7,
	ATTR_angle=8,
	ATTR_gisFileId=9,
	ATTR_minVal=10,
	ATTR_maxVal=11,
	ATTR_version=12,
	ATTR_byteOrder=13,
	ATTR_attrTable=14
} ATTR_NRS;

class nutshellmapattribute : public QDialog, private Ui::nutshellmapattribute
{
	Q_OBJECT

public:
	explicit nutshellmapattribute(QWidget *parent = 0);
	~nutshellmapattribute();

	ATTRIBUTES *a;
	MAP *m;
	QString filename;
    QString dirname;
	bool makenewmap;
    bool readyToClose;

	int fill(QString name, bool newmap);
	void OptNotSetAttr();
	void DefaultAttr();
	int ReadAttr(bool readOnly);
	void CreateMap();
	void SetAndCloseMap();
	void closeEvent(QCloseEvent *event);
    void hideEvent(QHideEvent *event);
	QString getMapAttributes(QString mapname);
    bool checkMapAttrib();

public slots:
	void Accept();

};

#endif // NUTSHELLMAPATTRIBUTE_H
