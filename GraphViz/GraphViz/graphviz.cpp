#include "graphviz.h"

GraphViz::GraphViz(QWidget *parent)
	: QMainWindow(parent)
{
	//area = new visualization::Drawer();
	//setCentralWidget(area);
//	qDebug() << "connected\n";

	ui.setupUi(this);
}

GraphViz::~GraphViz()
{

}
