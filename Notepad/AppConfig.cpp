#include "AppConfig.h"
#include<QDataStream>
#include<QFile>
#include<QApplication>


AppConfig::AppConfig()
{
	m_isVaild = restore();

}
AppConfig::AppConfig(QFont font, QPoint point, QSize size, bool isAutoWrap, bool isSbVisible, bool isTbVisible)
{
	m_editorFont = font;
	m_mainWindowPoint = point;
	m_mainWindowSize = size;
	m_isAutoWrap = isAutoWrap;
	m_isStatusBarVisible = isSbVisible;
	m_isToolBarVisible = isTbVisible;
	m_isVaild = true;

	store();
}
bool AppConfig::store()
{
	bool ret = true;
	QFile file(QApplication::applicationDirPath() + "/app.config");
	if (file.open(QIODevice::WriteOnly))
	{
		QDataStream out(&file);

		out.setVersion(QDataStream::Qt_5_6);

		out << m_editorFont;
		out << m_mainWindowPoint;
		out << m_mainWindowSize;
		out << m_isAutoWrap;
		out << m_isStatusBarVisible;
		out << m_isToolBarVisible;

		file.close();
	}
	else
	{
		ret = false;
	}

	return ret;
}
bool AppConfig::restore()
{
	bool ret = true;
	QFile file(QApplication::applicationDirPath() + "/app.config");
	if (file.open(QIODevice::ReadOnly))
	{
		QDataStream in(&file);

		in.setVersion(QDataStream::Qt_5_6);

		in >> m_editorFont;
		in >> m_mainWindowPoint;
		in >> m_mainWindowSize;
		in >> m_isAutoWrap;

		in >> m_isStatusBarVisible;
		in >> m_isToolBarVisible;

		file.close();
	}
	else
	{
		ret = false;
	}

	return ret;

}

QFont AppConfig::getEditorFont()
{
	return m_editorFont;
}
bool AppConfig::isAutoWrap()
{
	return m_isAutoWrap;
}
bool AppConfig::isStatusBarVisible()
{
	return m_isStatusBarVisible;
}
bool AppConfig::isToolBarVisible()
{
	return m_isToolBarVisible;
}


QPoint AppConfig::mainWindowPoint()
{
	return m_mainWindowPoint;
}
QSize AppConfig::mainWindowSize()
{
	return m_mainWindowSize;
}
AppConfig::~AppConfig()
{
}
