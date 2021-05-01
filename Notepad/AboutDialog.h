#ifndef _ABOUTDIALOG_H_
#define _ABOUTDIALOG_H_

#include<QDialog>
#include<QPushButton>
#include<QLabel>
#include<QPlainTextEdit>

class AboutDialog :public QDialog
{
	Q_OBJECT

	QLabel m_logoLbl;
	QPlainTextEdit m_infoEdit;
	QPushButton m_closeBtn;
public:
	explicit AboutDialog(QWidget *parent = 0);
};
#endif
