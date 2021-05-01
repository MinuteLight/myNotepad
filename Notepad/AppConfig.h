#ifndef _APPCONFIG_H_
#define _APPCONFIG_H_

#include<QObject>
#include<QFont>
#include<QPoint>
#include<QSize>
class AppConfig:public QObject
{
	Q_OBJECT

	QFont m_editorFont;
	bool m_isAutoWrap;
	bool m_isStatusBarVisible;
	bool m_isToolBarVisible;
	
	QPoint m_mainWindowPoint;
	QSize m_mainWindowSize;


	bool restore();


public:
	AppConfig();
	AppConfig(QFont font, QPoint m_mainWindowPoint,QSize m_mainWindowSize,bool isAutoWrap, bool isSbVisible, bool isTbVisible);
	~AppConfig();
	bool store();

	QFont getEditorFont();
	bool isAutoWrap();
	bool isStatusBarVisible();
	bool isToolBarVisible();


	QPoint mainWindowPoint();
	QSize mainWindowSize();


	bool m_isVaild;

};

#endif //_APPCONFIG_H_
