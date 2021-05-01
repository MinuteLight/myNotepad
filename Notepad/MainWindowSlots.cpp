#include"MainWindow.h"
#include"FindDialog.h"
#include"AboutDialog.h"
#include"AppConfig.h"
#include<QDebug>
#include<QFileDialog>
#include<QFile>
#include<QMessageBox>
#include<QTextStream>
#include<QMap>
#include<QFileDevice>
#include<QStringList>
#include<QMimeData>
#include<QEvent>
#include<QDragEnterEvent>
#include<QDropEvent>
#include<QFileInfo>
#include<QMenuBar>
#include<QToolBar>
#include<QObjectList>
#include<QKeyEvent>
#include<QApplication>
#include<QStatusBar>
#include<QFontDialog>
#include<QDesktopServices>
#include<QInputDialog>
#include<QSize>
#include<QPoint>

QString MainWindow::saveCurrentData(QString path)
{
	QString ret = path;
	if (ret == "")
	{
		ret = showFileDialog(QFileDialog::AcceptSave, "Save");
	}
	if (ret != "")
	{

		QFile file(ret);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QTextStream out(&file);

			out << mainEditor.toPlainText();

			file.close();

			setWindowTitle("NotePad - [ " + ret + " ]");

			m_isTextChanged = false;
		}
		else
		{
			showErrorMessage("Save Failed");

			ret = "";
		}
	}
	return ret;

}
void MainWindow::onFileOpen()
{
	preEditorChange();

	if (!m_isTextChanged)
	{
		QString path = showFileDialog(QFileDialog::AcceptOpen, "Open");
		openFileToEditor(path);
	}

}
void MainWindow::openFile(QString path)
{
	preEditorChange();

	if (!m_isTextChanged)
	{
		openFileToEditor(path);
	}
}

void MainWindow::openFileToEditor(QString path)
{
	if (path != "")
	{
		QFile file(path);

		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			mainEditor.setPlainText(file.readAll());

			m_filepath = path;

			file.close();

			setWindowTitle("NotePad - [ " + m_filepath + " ]");

			m_isTextChanged = false;
		}
		else
		{
			showErrorMessage("打开失败！");

			m_filepath = "";
		}
	}

}
void MainWindow::onFileSave()
{
	QString path = saveCurrentData(m_filepath);

	if (path != NULL)
	{
		m_filepath = path;
	}
}
QString MainWindow::showFileDialog(QFileDialog::AcceptMode mode, QString title)
{
	QString ret = "";
	QFileDialog fd(this);
	QStringList filters;
	QMap<QString, QString> map;
	const char* filterArray[][2] =
	{
		{ "Text Files (*.txt)" ,".txt" },
		{ "All Files (*)"      ,"*" },
		{ NULL				   , NULL }
	};
	for (int i = 0; filterArray[i][0] != NULL; i++)
	{
		filters.append(filterArray[i][0]);
		map.insert(filterArray[i][0], filterArray[i][1]);
	}


	fd.setWindowTitle(title);
	fd.setAcceptMode(mode);
	//	fd.setFilter(filters);



	if (mode == QFileDialog::AcceptOpen)
	{
		fd.setFileMode(QFileDialog::ExistingFile);
	}


	if (fd.exec() == QFileDialog::Accepted)
	{
		ret = fd.selectedFiles()[0];

	}
	return ret;
}
void MainWindow::showErrorMessage(QString message)
{
	QMessageBox msb(this);
	msb.setStandardButtons(QMessageBox::Ok);
	msb.setWindowTitle("Error");
	msb.setText(message);
	msb.setIcon(QMessageBox::Critical);
	msb.exec();
}
void MainWindow::onFileSaveAs()
{
	QString path = saveCurrentData();

	if (path != NULL)
	{
		m_filepath = path;
	}
}
void MainWindow::onTextChanged()
{
	if (!m_isTextChanged)
	{
		setWindowTitle("*" + windowTitle());
	}

	m_isTextChanged = true;


}
int MainWindow::showQueryMessage(QString message)
{
	QMessageBox msb(this);

	msb.setWindowTitle("Query");
	msb.setText(message);
	msb.setIcon(QMessageBox::Question);
	msb.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

	return msb.exec();

}
void MainWindow::onFileNew()
{
	preEditorChange();
	if (!m_isTextChanged)
	{
		mainEditor.clear();

		m_filepath = "";

		setWindowTitle("NotePad - [New] ");

		m_isTextChanged = false;
	}
}
void MainWindow::preEditorChange()
{
	if (m_isTextChanged)
	{
		int r = showQueryMessage("Do you want to save the changes to file ?");

		switch (r)
		{
		case QMessageBox::Yes:
			saveCurrentData(m_filepath);
			break;
		case QMessageBox::No:
			m_isTextChanged = false;
			break;
		case QMessageBox::Cancel:
			break;
		default:
			break;
		}
	}
}
void MainWindow::closeEvent(QCloseEvent* e)
{
	preEditorChange();

	if (!m_isTextChanged)
	{
		QFont font = mainEditor.font();
		bool isAutoWrap = (mainEditor.lineWrapMode() == QPlainTextEdit::WidgetWidth);
		bool isSbVisible = ((findMenuBarAction("Status Bar")->isChecked()) && (findToolBarAction("Status Bar")->isChecked()));
		bool isTbVisible = ((findMenuBarAction("Tool Bar")->isChecked()) && (findToolBarAction("Tool Bar")->isChecked()));


		AppConfig(font, pos(),size(),isAutoWrap, isSbVisible, isTbVisible);
		QMainWindow::closeEvent(e);
	}
	else
	{
		e->ignore();
	}
}
void MainWindow::dragEnterEvent(QDragEnterEvent* e)
{
	if (e->mimeData()->hasUrls())
	{
		e->acceptProposedAction();
	}
	else
	{
		e->ignore();
	}
}
void MainWindow::dropEvent(QDropEvent* e)
{
	if (e->mimeData()->hasUrls())
	{
		QList<QUrl> list = e->mimeData()->urls();
		QString path = list[0].toLocalFile();
		QFileInfo fi(path);
		if (fi.isFile())
		{
			preEditorChange();

			if (!m_isTextChanged)
			{
				openFileToEditor(path);
			}
		}
	}
	else
	{
		e->ignore();
	}
}

QAction* MainWindow::findMenuBarAction(QString text)
{
	QAction* ret = NULL;
	const QObjectList& list = menuBar()->children();

	for (int i = 0; i < list.count(); i++)
	{
		QMenu* menu = dynamic_cast<QMenu*>(list[i]);



		if (menu != NULL)
		{
			QList<QAction*> actions = menu->actions();

			for (int j = 0; j < actions.count(); j++)
			{

				if (actions[j]->text().startsWith(text))
				{

					ret = actions[j];
					break;
				}
			}
		}
	}


	return ret;
}

QAction* MainWindow::findToolBarAction(QString text)
{
	QAction* ret = NULL;
	const QObjectList& list = children();

	QToolBar* tb = toolBar();
	QList<QAction*> actions = tb->actions();

	for (int j = 0; j < actions.count(); j++)
	{
		if (actions[j]->toolTip().startsWith(text))
		{
			ret = actions[j];
			break;
		}
	}
	return ret;
}

void MainWindow::onRedoAvailable(bool available)
{

	findMenuBarAction("Redo")->setEnabled(available);
	findToolBarAction("Redo")->setEnabled(available);
}
void MainWindow::onUndoAvailable(bool available)
{

	findMenuBarAction("Undo")->setEnabled(available);
	findToolBarAction("Undo")->setEnabled(available);
}
void MainWindow::onCopyAvailable(bool available)
{

	findMenuBarAction("Copy")->setEnabled(available);
	findMenuBarAction("Cut")->setEnabled(available);

	findToolBarAction("Copy")->setEnabled(available);
	findToolBarAction("Cut")->setEnabled(available);
}
/*void MainWindow::onFilePrint()
{
	QPrintDialog dlg(this);
}*/
void MainWindow::onCursorPositionChanged()
{
	//光标字符位置
	int pos = mainEditor.textCursor().position();
	//ln行，col列
	int ln = 0, col = 0;
	//标记光标所在行的首位置
	int flag = -1;

	QString text = mainEditor.toPlainText();

	for (int i = 0; i < pos; i++)
	{
		if (text[i] == '\n')
		{
			ln++;
			flag = i;
		}
		else
		{

		}

	}
	flag++;
	col = pos - flag;
	statusLbl.setText("Ln: " + QString::number(ln + 1) + "    Col: " + QString::number(col + 1));


}
void MainWindow::onEditDelete()
{
	QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Delete, Qt::NoModifier);
	QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Delete, Qt::NoModifier);

	QApplication::sendEvent(&mainEditor, &keyPress);
	QApplication::sendEvent(&mainEditor, &keyRelease);
}
void MainWindow::onFileExit()
{
	close();
}
void MainWindow::onEditFind()
{
	m_pFindDlg->show();
}
void MainWindow::onEditReplace()
{
	m_pReplaceDlg->show();
}
void MainWindow::onEditGoto()
{
	bool ok = false;
	int ln = QInputDialog::getInt(this, "Goto", "Line:", 1, 1, mainEditor.document()->lineCount(), 1, &ok);

	if (ok)
	{
		int pos = 0;
		int next = -1;
		QString text = mainEditor.toPlainText();
		QTextCursor c = mainEditor.textCursor();

		for (int i = 0; i < ln; i++)
		{
			pos = next + 1;
			next = text.indexOf('\n', pos);
		}
		c.setPosition(pos);
		mainEditor.setTextCursor(c);
	}

}
void MainWindow::onViewToolBar()
{

	QObjectList list = children();
	QToolBar* tb = toolBar();
	bool visible;

	visible = tb->isVisible();
	tb->setVisible(!visible);

	findMenuBarAction("Tool Bar")->setChecked(!visible);
	findToolBarAction("Tool Bar")->setChecked(!visible);
}
void MainWindow::onViewStatusBar()
{
	QStatusBar* sb = statusBar();

	bool visible = sb->isVisible();

	sb->setVisible(!visible);

	findMenuBarAction("Status Bar")->setChecked(!visible);
	findToolBarAction("Status Bar")->setChecked(!visible);
}
void MainWindow::onHelpAbout()
{

	AboutDialog(this).exec();

}
void MainWindow::onFormatFont()
{
	bool ok = false;
	QFont font = QFontDialog::getFont(&ok, mainEditor.font(), this);

	if (ok)
	{
		mainEditor.setFont(font);
	}

}
void MainWindow::onFormatWrap()
{
	QPlainTextEdit::LineWrapMode mode = mainEditor.lineWrapMode();

	if (mode == QPlainTextEdit::NoWrap)
	{
		mainEditor.setLineWrapMode(QPlainTextEdit::WidgetWidth);
		findMenuBarAction("Auto Wrap")->setCheckable(true);
		findToolBarAction("Auto Wrap")->setChecked(true);

	}
	else
	{
		mainEditor.setLineWrapMode(QPlainTextEdit::NoWrap);
		findMenuBarAction("Auto Wrap")->setCheckable(false);
		findToolBarAction("Auto Wrap")->setChecked(false);
	}
}
void MainWindow::onHelpManual()
{
	//QDesktopServices::openUrl(QUrl("file:///C:/Users/haha/Desktop/1.txt"));
}

