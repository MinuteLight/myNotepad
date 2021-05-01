#include <QApplication>
#include "MainWindow.h"
#include<QFileInfo>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow* w = MainWindow::NewInstance();
	int ret = -1;

	if (w != NULL)
	{
		if (argc > 1)
		{
			QFileInfo fi(argv[1]);

			if (fi.exists())
			{
				//open the file
				w->openFile(fi.absoluteFilePath());
			}
		}

		w->show();

		ret = a.exec();
	}

	delete w;

	return ret;
}
