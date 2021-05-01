#include "AboutDialog.h"
#include<QPixmap>
#include<QPalette>
#include<QDebug>
AboutDialog::AboutDialog(QWidget *parent)
	:QDialog(parent,Qt::WindowCloseButtonHint),m_logoLbl(this),m_infoEdit(this),m_closeBtn(this)
{
	QPixmap pm(":/res/pic/dt.png");
	pm = pm.scaled(120, 120, Qt::KeepAspectRatio);
	m_logoLbl.setPixmap(pm);
	
	m_logoLbl.resize(120, 120);
	m_logoLbl.move(20, 20);

	m_infoEdit.insertPlainText("NotePad Project\n\nPlatform: Qt 5.6\n\nVersion: 1.0.0\n\nCopyright: Minute Light");
	m_infoEdit.resize(180, 130);
	m_infoEdit.move(200, 30);
	m_infoEdit.setReadOnly(true);
	
	QPalette p = m_infoEdit.palette();
	p.setColor(QPalette::Active, QPalette::Base, palette().color(QPalette::Active, QPalette::Background));
	p.setColor(QPalette::Inactive, QPalette::Base, palette().color(QPalette::Inactive, QPalette::Background));
	m_infoEdit.setPalette(p);
	m_infoEdit.setFrameStyle(QFrame::NoFrame);

	m_closeBtn.setText("Close");
	m_closeBtn.resize(100, 30);
	m_closeBtn.move(273, 175);

	setFixedSize(390, 230);
	setWindowTitle("About NotePad");

	connect(&m_closeBtn, SIGNAL(clicked()), this, SLOT(close()));


}

