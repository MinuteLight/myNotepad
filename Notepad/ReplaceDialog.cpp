#include "ReplaceDialog.h"
#include<QDebug>


ReplaceDialog::ReplaceDialog(QWidget *parent, QPlainTextEdit* pText)
	:FindDialog(parent,pText)
{
	
	initControl();
	connectSlot();
	setWindowTitle("Replace");

}

void ReplaceDialog::initControl()
{
	replaceBtn.setText("Replace");
	replaceAllBtn.setText("Replace All");
	replaceLbl.setText("Replace To");

	m_layout.removeWidget(&m_radioGrpBx);
	m_layout.removeWidget(&m_matchChkBx);
	m_layout.removeWidget(&m_closeBtn);

	m_layout.addWidget(&replaceLbl, 1, 0);
	m_layout.addWidget(&replaceEdit, 1, 1);
	m_layout.addWidget(&replaceBtn, 1, 2);

	m_layout.addWidget(&m_matchChkBx, 2, 0);
	m_layout.addWidget(&m_radioGrpBx, 2, 1);
	m_layout.addWidget(&replaceAllBtn, 2, 2);

	m_layout.addWidget(&m_closeBtn, 3, 2);


}
void ReplaceDialog::connectSlot()
{
	connect(&replaceBtn, SIGNAL(clicked()), this, SLOT(onReplaceClicked()));
	connect(&replaceAllBtn, SIGNAL(clicked()), this, SLOT(onReplaceAllClicked()));


}
void ReplaceDialog::onReplaceClicked()
{
	QString target = m_findEdit.text();
	QString to = replaceEdit.text();

	if ((target != "") && (to != "") && (m_pText != NULL))
	{
		QString selectText = m_pText->textCursor().selectedText();

		if (selectText == target)
		{
			m_pText->insertPlainText(to);
		}
		
		onEditFindClicked();

	}
	
}
void ReplaceDialog::onReplaceAllClicked()
{
	QString target = m_findEdit.text();
	QString to = replaceEdit.text();

	if ((target != "") && (to != "") && (m_pText != NULL))
	{
		QString text = m_pText->toPlainText();

		text.replace(target,to, m_matchChkBx.isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

		m_pText->clear();

		m_pText->insertPlainText(text);
	}
}

ReplaceDialog::~ReplaceDialog()
{
}
