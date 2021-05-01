#ifndef _REPLACEDIALOG_H_
#define _REPLACEDIALOG_H_

#include "FindDialog.h"
#include<QPlainTextEdit>
#include<QPushButton>
#include<QLabel>
class ReplaceDialog :
	public FindDialog
{
	Q_OBJECT

	QLabel replaceLbl;
	QLineEdit replaceEdit;
	QPushButton replaceBtn;
	QPushButton replaceAllBtn;
protected slots:
	void onReplaceClicked();
	void onReplaceAllClicked();


public:
	explicit ReplaceDialog(QWidget *parent = 0, QPlainTextEdit* pText = 0);
	void initControl();
	void connectSlot();
	~ReplaceDialog();
};

#endif