#include "visuallica.h"
#include <QtWidgets/QApplication>

#ifdef STATIC
#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
Q_IMPORT_PLUGIN(QICOPlugin)
#endif

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Visuallica w;
	w.show();
	return a.exec();
}
