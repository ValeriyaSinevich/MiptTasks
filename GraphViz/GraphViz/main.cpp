#include "graphviz.h"
#include <QtWidgets/QApplication>
#include <cstdlib>


int main(int argc, char *argv[])
{
	
	QApplication a(argc, argv);

	/*BSTEPA *b = new BSTEPA();
	QMetaObject::Connection x = QObject::connect(&b->a, SIGNAL(smth(void)), b, SLOT(handler(void)));
	if (x) {
		qDebug() << "great!\n";
	}
	
	b->a.emitter();
	int xx = 1;
	for (int i = 0; i < 1000 * 1000 * 10; ++i) {
		xx += xx * xx * xx;
	}
	qDebug() << xx << "\n";*/
	GraphViz w;
	w.show();
	return a.exec();
}
