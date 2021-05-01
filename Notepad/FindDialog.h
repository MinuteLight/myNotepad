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
	����˳�򣬴��µ��ϣ��Ӻ�ǰ������QGroupBox��Ϊ�������Ҫ��������������棬��Ȼ����ʱ���ᵼ�³������
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
