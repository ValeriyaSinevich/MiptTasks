#include "graphviz.h"
#include <QtWidgets/QApplication>
#include <cstdlib>


int main(int argc, char *argv[])
{
	
	QApplication a(argc, argv);
	GraphViz w;
	w.show();
	return a.exec();
}
