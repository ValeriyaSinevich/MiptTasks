#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

#include <QtWidgets/QMainWindow>
#include "ui_graphviz.h"
#include <QtGui>
#include "Drawer.h"
#include <QDebug>



class GraphViz : public QMainWindow
{
	Q_OBJECT

public:
	GraphViz(QWidget *parent = 0);
	~GraphViz();

	//visualization::Drawer *area;


private:
	Ui::GraphVizClass ui;
};

#endif // GRAPHVIZ_H
