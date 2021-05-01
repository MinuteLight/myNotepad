#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include"FindDialog.h"
#include"ReplaceDialog.h"
#include <QMainWindow>
#include <QKeySequence>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QPlainTextEdit>
#include <QLabel>
#include <QFileDialog>
#include<QMimeData>
#include<QTextCursor>
#include<QSharedPointer>

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	QPlainTextEdit mainEditor;
	QLabel statusLbl;
	QString m_filepath;
	bool m_isTextChanged;

	QSharedPointer<FindDialog> m_pFindDlg;
	QSharedPointer<ReplaceDialog> m_pReplaceDlg;

	MainWindow();
	MainWindow(const MainWindow&);
	MainWindow& operator= (const MainWindow&);

	bool construct();
	bool initMenuBar();
	bool initToolBar();
	bool initStatusBar();
	bool initMainEditor();
	bool initFileMenu(QMenuBar* mb);
	bool initEditMenu(QMenuBar* mb);
	bool initFormatMenu(QMenuBar* mb);
	bool initViewMenu(QMenuBar* mb);
	bool initHelpMenu(QMenuBar* mb);
	bool initFileToolItem(QToolBar* tb);
	bool initEditToolItem(QToolBar* tb);
	bool initFormatToolItem(QToolBar* tb);
	bool initViewToolItem(QToolBar* tb);
	bool makeAction(QAction*& action, QWidget* parent, QString text, int key);
	bool makeAction(QAction*& action, QWidget* parent, QString tip, QString icon);

	QString showFileDialog(QFileDialog::AcceptMode mode, QString title);
	int showQueryMessage(QString message);
	void showErrorMessage(QString message);
	QString saveCurrentData(QString path = "");
	void preEditorChange();
	void openFileToEditor(QString path);
	QAction* findMenuBarAction(QString text);
	QAction* findToolBarAction(QString text);


protected:
	void closeEvent(QCloseEvent* e);
	void dragEnterEvent(QDragEnterEvent* e);
	void dropEvent(QDropEvent* e);
private slots:
	void onFileNew();
	void onFileOpen();
	void onFileSave();
	void onFileSaveAs();
	//void onFilePrint();
	void onEditDelete();
	void onFileExit();
	void onEditFind();
	void onEditReplace();
	void onEditGoto();

	void onFormatFont();
	void onFormatWrap();


	void onViewToolBar();
	void onViewStatusBar();

	void onHelpAbout();
	void onHelpManual();

	void onTextChanged();
	

	void onCopyAvailable(bool available);
	void onRedoAvailable(bool available);
	void onUndoAvailable(bool available);
	void onCursorPositionChanged();
public:
	static MainWindow* NewInstance();
	QToolBar* toolBar();
	void openFile(QString path);
	~MainWindow();
};

#endif // _MAINWINDOW_H_

