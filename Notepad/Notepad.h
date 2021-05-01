#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Notepad.h"

class Notepad : public QMainWindow
{
	Q_OBJECT

public:
	Notepad(QWidget *parent = Q_NULLPTR);

private:
	Ui::NotepadClass ui;
};
