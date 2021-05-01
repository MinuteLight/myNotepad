#include "MainWindow.h"
#include "FindDialog.h"
#include"AppConfig.h"

#include <QMenu>
#include <QIcon>
#include <QSize>
#include <QStatusBar>
#include <QLabel>
#include <QDebug>
#include <QPalette>

MainWindow::MainWindow() :m_pFindDlg(new FindDialog(this, &mainEditor)),
m_pReplaceDlg(new ReplaceDialog(this, &mainEditor))
{

	setWindowTitle("NotePad - [ New ]");
	setAcceptDrops(true);

	m_filepath = "";

	m_isTextChanged = false;
}

MainWindow* MainWindow::NewInstance()
{
	MainWindow* ret = new MainWindow();

	if ((ret == NULL) || !ret->construct())
	{
		delete ret;
		ret = NULL;
	}

	return ret;
}

bool MainWindow::construct()
{
	bool ret = true;

	ret = ret && initMenuBar();
	ret = ret && initToolBar();
	ret = ret && initStatusBar();
	ret = ret && initMainEditor();

	AppConfig config;

	if (config.m_isVaild)
	{
		mainEditor.setFont(config.getEditorFont());

		if (!config.isAutoWrap())
		{
			mainEditor.setLineWrapMode(QPlainTextEdit::NoWrap);

			findToolBarAction("Auto Wrap")->setChecked(false);
			findMenuBarAction("Auto Wrap")->setChecked(false);

		}
		if (!config.isStatusBarVisible())
		{
			statusBar()->setVisible(false);
			findToolBarAction("Status Bar")->setChecked(false);
			findMenuBarAction("Status Bar")->setChecked(false);
		}

		if (!config.isToolBarVisible())
		{
			toolBar()->setVisible(false);

			findToolBarAction("Tool Bar")->setChecked(false);
			findMenuBarAction("Tool Bar")->setChecked(false);
		}
		qDebug() << config.mainWindowSize();
		resize(config.mainWindowSize());
		move(config.mainWindowPoint());
	}
	
	return ret;
}

bool MainWindow::initMenuBar()
{
	bool ret = true;
	QMenuBar* mb = menuBar();

	ret = ret && initFileMenu(mb);
	ret = ret && initEditMenu(mb);
	ret = ret && initFormatMenu(mb);
	ret = ret && initViewMenu(mb);
	ret = ret && initHelpMenu(mb);

	return ret;
}

bool MainWindow::initToolBar()
{
	bool ret = true;
	QToolBar* tb = addToolBar("Tool Bar");

	tb->setIconSize(QSize(16, 16));

	ret = ret && initFileToolItem(tb);

	tb->addSeparator();

	ret = ret && initEditToolItem(tb);

	tb->addSeparator();

	ret = ret && initFormatToolItem(tb);

	tb->addSeparator();

	ret = ret && initViewToolItem(tb);

	return ret;
}

bool MainWindow::initStatusBar()
{
	bool ret = true;
	QStatusBar* sb = statusBar();
	QLabel* label = new QLabel("Designed By Minute");

	if (label != NULL)
	{
		statusLbl.setMinimumWidth(200);
		statusLbl.setAlignment(Qt::AlignCenter);
		statusLbl.setText("Ln: 1    Col: 1");

		label->setMinimumWidth(200);
		label->setAlignment(Qt::AlignCenter);

		sb->addPermanentWidget(new QLabel());
		sb->addPermanentWidget(&statusLbl);
		sb->addPermanentWidget(label);
	}
	else
	{
		ret = false;
	}

	return ret;
}

bool MainWindow::initMainEditor()
{
	bool ret = true;
	QPalette p = mainEditor.palette();

	p.setColor(QPalette::Inactive, QPalette::Highlight, p.color(QPalette::Active, QPalette::Highlight));
	p.setColor(QPalette::Inactive, QPalette::HighlightedText, p.color(QPalette::Active, QPalette::HighlightedText));

	mainEditor.setPalette(p);

	mainEditor.setParent(this);

	connect(&mainEditor, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
	connect(&mainEditor, SIGNAL(copyAvailable(bool)), this, SLOT(onCopyAvailable(bool)));
	connect(&mainEditor, SIGNAL(redoAvailable(bool)), this, SLOT(onRedoAvailable(bool)));
	connect(&mainEditor, SIGNAL(undoAvailable(bool)), this, SLOT(onUndoAvailable(bool)));
	connect(&mainEditor, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));

	setCentralWidget(&mainEditor);

	return ret;
}

bool MainWindow::initFileMenu(QMenuBar* mb)
{
	QMenu* menu = new QMenu("File(&F)", mb);
	bool ret = (menu != NULL);

	if (ret)
	{
		QAction* action = NULL;

		ret = ret && makeAction(action, menu, "New(&N)", Qt::CTRL + Qt::Key_N);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), this, SLOT(onFileNew()));
			menu->addAction(action);
		}

		ret = ret && makeAction(action, menu, "Open(&O)...", Qt::CTRL + Qt::Key_O);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), this, SLOT(onFileOpen()));
			menu->addAction(action);
		}

		ret = ret && makeAction(action, menu, "Save(&S)", Qt::CTRL + Qt::Key_S);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), this, SLOT(onFileSave()));
			menu->addAction(action);			qDebug() << "save ok";

		}

		ret = ret && makeAction(action, menu, "Save As(&A)...", 0);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), this, SLOT(onFileSaveAs()));
			menu->addAction(action);
		}

		menu->addSeparator();

		ret = ret && makeAction(action, menu, "Print(&P)...", Qt::CTRL + Qt::Key_P);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), this, SLOT(onFilePrint()));
			menu->addAction(action);
		}

		menu->addSeparator();

		ret = ret && makeAction(action, menu, "Exit(&X)", 0);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), this, SLOT(onFileExit()));
			menu->addAction(action);
		}
	}

	if (ret)
	{
		mb->addMenu(menu);
	}
	else
	{
		delete menu;
	}

	return ret;
}

bool MainWindow::initEditMenu(QMenuBar* mb)
{
	QMenu* menu = new QMenu("Edit(&E)", mb);
	bool ret = (menu != NULL);

	if (ret)
	{
		QAction* action = NULL;

		ret = ret && makeAction(action, menu, "Undo(&U)", Qt::CTRL + Qt::Key_Z);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), &mainEditor, SLOT(undo()));
			action->setEnabled(false);
			menu->addAction(action);
		}

		ret = ret && makeAction(action, menu, "Redo(&R)...", Qt::CTRL + Qt::Key_Y);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), &mainEditor, SLOT(redo()));
			action->setEnabled(false);
			menu->addAction(action);
		}

		menu->addSeparator();

		ret = ret && makeAction(action, menu, "Cut(&T)", Qt::CTRL + Qt::Key_X);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), &mainEditor, SLOT(cut()));
			action->setEnabled(false);
			menu->addAction(action);
		}

		ret = ret && makeAction(action, menu, "Copy(&C)...", Qt::CTRL + Qt::Key_C);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), &mainEditor, SLOT(copy()));
			action->setEnabled(false);
			menu->addAction(action);
		}

		ret = ret && makeAction(action, menu, "Paste(&P)...", Qt::CTRL + Qt::Key_V);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), &mainEditor, SLOT(paste()));
			menu->addAction(action);
		}

		ret = ret && makeAction(action, menu, "Delete(&L)", Qt::Key_Delete);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), this, SLOT(onEditDelete()));
			menu->addAction(action);
		}

		menu->addSeparator();

		ret = ret && makeAction(action, menu, "Find(&F)...", Qt::CTRL + Qt::Key_F);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), this, SLOT(onEditFind()));
			menu->addAction(action);
		}

		ret = ret && makeAction(action, menu, "Replace(&R)...", Qt::CTRL + Qt::Key_H);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), this, SLOT(onEditReplace()));
			menu->addAction(action);
		}

		ret = ret && makeAction(action, menu, "Goto(&G)...", Qt::CTRL + Qt::Key_G);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), this, SLOT(onEditGoto()));
			menu->addAction(action);
		}

		menu->addSeparator();

		ret = ret && makeAction(action, menu, "Select All(&A)", Qt::CTRL + Qt::Key_A);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), &mainEditor, SLOT(selectAll()));
			menu->addAction(action);
		}
	}

	if (ret)
	{
		mb->addMenu(menu);
	}
	else
	{
		delete menu;
	}

	return ret;
}

bool MainWindow::initFormatMenu(QMenuBar* mb)
{
	QMenu* menu = new QMenu("Format(&O)", mb);
	bool ret = (menu != NULL);

	if (ret)
	{
		QAction* action = NULL;

		ret = ret && makeAction(action, menu, "Auto Wrap(&W)", 0);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), this, SLOT(onFormatWrap()));
			action->setCheckable(true);
			action->setChecked(true);
			menu->addAction(action);
		}

		ret = ret && makeAction(action, menu, "Font(&F)...", 0);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), this, SLOT(onFormatFont()));
			menu->addAction(action);
		}
	}

	if (ret)
	{
		mb->addMenu(menu);
	}
	else
	{
		delete menu;
	}

	return ret;
}

bool MainWindow::initViewMenu(QMenuBar* mb)
{
	QMenu* menu = new QMenu("View(&V)", mb);
	bool ret = (menu != NULL);

	if (ret)
	{
		QAction* action = NULL;

		ret = ret && makeAction(action, menu, "Tool Bar(&T)", 0);

		if (ret)
		{
			action->setCheckable(true);
			action->setChecked(true);
			connect(action, SIGNAL(triggered()), this, SLOT(onViewToolBar()));
			menu->addAction(action);
		}

		ret = ret && makeAction(action, menu, "Status Bar(&S)", 0);

		if (ret)
		{
			action->setCheckable(true);
			action->setChecked(true);
			connect(action, SIGNAL(triggered()), this, SLOT(onViewStatusBar()));
			menu->addAction(action);
		}
	}

	if (ret)
	{
		mb->addMenu(menu);
	}
	else
	{
		delete menu;
	}

	return ret;
}

bool MainWindow::initHelpMenu(QMenuBar* mb)
{
	QMenu* menu = new QMenu("Help(&H)", mb);
	bool ret = (menu != NULL);

	if (ret)
	{
		QAction* action = NULL;

		ret = ret && makeAction(action, menu, "User Manual", 0);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), this, SLOT(onHelpManual()));
			menu->addAction(action);
		}

		ret = ret && makeAction(action, menu, "About NotePad...", 0);

		if (ret)
		{
			connect(action, SIGNAL(triggered()), this, SLOT(onHelpAbout()));
			menu->addAction(action);
		}
	}

	if (ret)
	{
		mb->addMenu(menu);
	}
	else
	{
		delete menu;
	}

	return ret;
}

bool MainWindow::initFileToolItem(QToolBar* tb)
{
	bool ret = true;
	QAction* action = NULL;

	ret = ret && makeAction(action, tb, "New", ":/res/pic/new.png");

	if (ret)
	{
		connect(action, SIGNAL(triggered()), this, SLOT(onFileNew()));
		tb->addAction(action);
	}

	ret = ret && makeAction(action, tb, "Open", ":/res/pic/open.png");

	if (ret)
	{
		connect(action, SIGNAL(triggered()), this, SLOT(onFileOpen()));
		tb->addAction(action);
	}

	ret = ret && makeAction(action, tb, "Save", ":/res/pic/save.png");

	if (ret)
	{
		connect(action, SIGNAL(triggered()), this, SLOT(onFileSave()));
		tb->addAction(action);
	}

	ret = ret && makeAction(action, tb, "Save As", ":/res/pic/saveas.png");

	if (ret)
	{
		connect(action, SIGNAL(triggered()), this, SLOT(onFileSaveAs()));
		tb->addAction(action);
	}

	ret = ret && makeAction(action, tb, "Print", ":/res/pic/print.png");

	if (ret)
	{
		connect(action, SIGNAL(triggered()), this, SLOT(onFilePrint()));
		tb->addAction(action);
	}


	return ret;
}

bool MainWindow::initEditToolItem(QToolBar* tb)
{
	bool ret = true;
	QAction* action = NULL;

	ret = ret && makeAction(action, tb, "Undo", ":/res/pic/undo.png");

	if (ret)
	{
		connect(action, SIGNAL(triggered()), &mainEditor, SLOT(undo()));

		tb->addAction(action);

		action->setEnabled(false);
	}

	ret = ret && makeAction(action, tb, "Redo", ":/res/pic/redo.png");

	if (ret)
	{
		connect(action, SIGNAL(triggered()), &mainEditor, SLOT(redo()));
		action->setEnabled(false);
		tb->addAction(action);
	}

	ret = ret && makeAction(action, tb, "Cut", ":/res/pic/cut.png");

	if (ret)
	{
		connect(action, SIGNAL(triggered()), &mainEditor, SLOT(cut()));
		action->setEnabled(false);
		tb->addAction(action);
	}

	ret = ret && makeAction(action, tb, "Copy", ":/res/pic/copy.png");

	if (ret)
	{
		connect(action, SIGNAL(triggered()), &mainEditor, SLOT(copy()));
		action->setEnabled(false);
		tb->addAction(action);
	}

	ret = ret && makeAction(action, tb, "Paste", ":/res/pic/paste.png");

	if (ret)
	{
		connect(action, SIGNAL(triggered()), &mainEditor, SLOT(paste()));
		tb->addAction(action);
	}

	ret = ret && makeAction(action, tb, "Find", ":/res/pic/find.png");

	if (ret)
	{
		tb->addAction(action);
	}

	ret = ret && makeAction(action, tb, "Replace", ":/res/pic/replace.png");

	if (ret)
	{
		tb->addAction(action);
	}

	ret = ret && makeAction(action, tb, "Goto", ":/res/pic/goto.png");

	if (ret)
	{
		tb->addAction(action);
	}

	return ret;
}

bool MainWindow::initFormatToolItem(QToolBar* tb)
{
	bool ret = true;
	QAction* action = NULL;

	ret = ret && makeAction(action, tb, "Auto Wrap", ":/res/pic/wrap.png");

	if (ret)
	{
		connect(action, SIGNAL(triggered()), this, SLOT(onFormatWrap()));
		action->setCheckable(true);
		action->setChecked(true);
		tb->addAction(action);
	}

	ret = ret && makeAction(action, tb, "Font", ":/res/pic/font.png");

	if (ret)
	{
		connect(action, SIGNAL(triggered()), this, SLOT(onFormatFont()));
		tb->addAction(action);
	}

	return ret;
}

bool MainWindow::initViewToolItem(QToolBar* tb)
{
	bool ret = true;
	QAction* action = NULL;

	ret = ret && makeAction(action, tb, "Tool Bar", ":/res/pic/tool.png");

	if (ret)
	{
		action->setCheckable(true);
		action->setChecked(true);
		connect(action, SIGNAL(triggered()), this, SLOT(onViewToolBar()));
		tb->addAction(action);
	}

	ret = ret && makeAction(action, tb, "Status Bar", ":/res/pic/status.png");

	if (ret)
	{
		action->setCheckable(true);
		action->setChecked(true);
		connect(action, SIGNAL(triggered()), this, SLOT(onViewStatusBar()));
		tb->addAction(action);
	}

	return ret;
}

bool MainWindow::makeAction(QAction*& action, QWidget* parent, QString text, int key)
{
	bool ret = true;

	action = new QAction(text, parent);

	if (action != NULL)
	{
		action->setShortcut(QKeySequence(key));
	}
	else
	{
		ret = false;
	}

	return ret;
}

bool MainWindow::makeAction(QAction*& action, QWidget* parent, QString tip, QString icon)
{
	bool ret = true;

	action = new QAction("", parent);

	if (action != NULL)
	{
		action->setToolTip(tip);
		action->setIcon(QIcon(icon));
	}
	else
	{
		ret = false;
	}

	return ret;
}

MainWindow::~MainWindow()
{

}
QToolBar* MainWindow::toolBar()
{
	QObjectList list = children();
	QToolBar* tb = NULL;
	for (int i = 0; i < list.count(); i++)
	{
		tb = dynamic_cast<QToolBar*>(list[i]);

		if (tb != NULL)
		{
			break;
		}
	}
	return tb;

}