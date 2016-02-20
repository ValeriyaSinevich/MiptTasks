#ifndef DRAWER_H
#define DRAWER_H
#include <QtWidgets/QMainWindow>
#include <qpainter.h>
#include "PushRelabel.h"
#include <cmath>
#include <QPushButton>
#include <QtWidgets/QLabel>

namespace visualization {
	class vertex {
	public:
		vertex(int i, int _h, int _w, int n, int _e);
		vertex();
		int number;
		int h;
		int w;
		int level;
		int e = 0;
		vector<pair <double, double> > forces;
		pair<double, double> result_force = std::make_pair(0,0);
	};

	class Drawer : public QWidget
	{
		Q_OBJECT

	public:
		QPushButton *Prev;
		QPushButton *Next;
		QPushButton *Start;
		QLabel *Text;


		vector<vertex> vertices;
		algo::network *Net;
		Drawer(QWidget *parent = 0);
		int prevH;
		int prevW;

		const double c1 = 2;
		const double c2 = 1;
		const double c3 = 1;
	    double c4 = 0.1;
		double eps = INT_MAX;
		
		int levels;

		int lastPushed = -2;

		void levelforceBased();
		void paintEvent(QPaintEvent *event);
		void drawInitial();
		void reDraw();//when size has changed
		void draw();
		void forceBased();
		pair<QPointF, QPointF> drawArrow(vertex u, vertex v);
		QPointF pointOnLine(vertex u, vertex v);

	signals:
		void VertexRelabeled(int, int);
		void EdgePushed(int, int, int);
	public slots:
		void NextClicked();
		void StartClicked();
		void PrevClicked();
		void afterPush(int u, int v, int newFlow);
		void afterRelabel(int u, int newH);
		void finished(void);
	};
}

#endif 