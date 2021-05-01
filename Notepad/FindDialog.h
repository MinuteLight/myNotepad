#ifndef _FINDDIALOG_H_
#define _FINDDIALOG_H_
#include <QDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QTextCursor>
#include <QPointer>
#include <QPlainTextEdit>

class FindDialog :
	public QDialog
{
	Q_OBJECT
protected:
	/**
	析构顺序，从下到上，从后到前，所以QGroupBox作为父类必须要在它的子类的上面，否然析构时将会导致程序崩溃
	*/

	QGridLayout m_layout;

	QGroupBox m_radioGrpBx;
	QHBoxLayout m_hbLayout;	

	QLabel m_findLbl;
	QLineEdit m_findEdit;
	QPushButton m_findBtn;
	QPushButton m_closeBtn;
	QCheckBox m_matchChkBx;
	
	QRadioButton m_backwardBtn;
	QRadioButton m_forwardBtn; 

	QPointer<QPlainTextEdit> m_pText;

protected slots:
	void onEditFindClicked();
	void onCloseClicked();
public:
	explicit FindDialog(QWidget* parent = 0,QPlainTextEdit* pText=0);
	
	void initControl();
	void connectSlot();

	void setPlainTextEdit(QPlainTextEdit* pText);
	QPlainTextEdit* getPlainTextEdit();



	bool event(QEvent* evt);


};
#endif //_FINDDIALOG_H_
