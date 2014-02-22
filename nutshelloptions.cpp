#include "nutshellqt.h"


//---------------------------------------------------------------------------
nutshellOptions::nutshellOptions(QWidget *parent) :
		QDialog(parent)
{
	setupUi(this);

	connect(toolButton_1, SIGNAL(clicked()), this, SLOT(findPcrcalcDir()));
	connect(toolButton_2, SIGNAL(clicked()), this, SLOT(findAguilaDir()));
	connect(toolButton_3, SIGNAL(clicked()), this, SLOT(findMapeditDir()));
	//	connect(pushButton_ok, SIGNAL(clicked()), this, SLOT(closew()));
	//	connect(pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));

	baseDirs.clear();
}
//---------------------------------------------------------------------------
nutshellOptions::~nutshellOptions()
{
}
//---------------------------------------------------------------------------
void nutshellOptions::setupOptions(QStringList SL)
{
	baseDirs = SL;
	lineEdit_pcrcalcDir->setText(SL[0]);
	lineEdit_aguilaDir->setText(SL[1]);
	lineEdit_mapeditDir->setText(SL[2]);
}
//---------------------------------------------------------------------------
QStringList nutshellOptions::getOptions()
{
	baseDirs.clear();
	baseDirs << lineEdit_pcrcalcDir->text()
			<< lineEdit_aguilaDir->text() << lineEdit_mapeditDir->text();
	return baseDirs;
}
//---------------------------------------------------------------------------
void nutshellOptions::findPcrcalcDir()
{
	QString dir;
	QString olddir = lineEdit_pcrcalcDir->text();
    dir = setExistingDirectory("pcrcalc dir",baseDirs[0]);
	if (!dir.endsWith(QDir::separator()))
		dir = dir + QDir::separator();
	if (QFileInfo(dir+"pcrcalc.exe").exists())
    {
		lineEdit_pcrcalcDir->setText(dir);
    }
	else
	{
		lineEdit_pcrcalcDir->setText(olddir);
		ErrorMsg("pcrcalc.exe not found.");
	}
}
//---------------------------------------------------------------------------
void nutshellOptions::findAguilaDir()
{
	QString dir;
	QString olddir = lineEdit_pcrcalcDir->text();
    dir = setExistingDirectory("aguila dir",baseDirs[1]);
	if (!dir.endsWith(QDir::separator()))
		dir = dir + QDir::separator();
	if (QFileInfo(dir+"aguila.exe").exists())
		lineEdit_aguilaDir->setText(dir);
	else
	{
		lineEdit_aguilaDir->setText(olddir);
		ErrorMsg("aguila.exe not found.");
	}
}
//---------------------------------------------------------------------------
void nutshellOptions::findMapeditDir()
{
	QString dir;
	QString olddir = lineEdit_pcrcalcDir->text();
    dir = setExistingDirectory("mapedit dir",baseDirs[2]);
	if (!dir.endsWith(QDir::separator()))
		dir = dir + QDir::separator();
	if (QFileInfo(dir+"mapedit.exe").exists())
		lineEdit_mapeditDir->setText(dir);
	else
	{
		lineEdit_mapeditDir->setText(olddir);
		ErrorMsg("mapedit.exe not found.");
	}
}
//---------------------------------------------------------------------------
QString nutshellOptions::setExistingDirectory(QString title, QString bd)
{
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
	QString directory =
			QFileDialog::getExistingDirectory(this, title, bd, options);
	if (!directory.isEmpty())
		return(directory);
	else
		return "";
}
//---------------------------------------------------------------------------
