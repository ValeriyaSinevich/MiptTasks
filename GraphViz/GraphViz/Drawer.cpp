#include "Drawer.h"

using namespace visualization;

vertex::vertex(int i, int _h, int _w, int n, int _e) : number(i), h(_h), w(_w), e(_e) {
		forces = vector<pair<double, double> >(n);
	}
vertex::vertex(){}

Drawer::Drawer(QWidget *parent){
	std::ifstream input;
	input.open("input.txt");
	int m, n;
	input >> n >> m;

	Net = new algo::network(n, this); 

	levels = 2 * n;

	for (unsigned int i = 0; i < m; ++i){
		int v, to, w;
		input >> v >> to >> w;
		if (v == to) {
			w = 0;
		}
		Net->addEdge(--v, --to, w, i);
	}

	drawInitial();
	QRect window = this->rect();
	for (unsigned int j = 0; j < vertices.size(); ++j) {
		vertices[j].level = 2 * j + 1;
		vertices[j].h = (levels - vertices[j].level) * ((window.height() - 120) / (levels + 1));
	}
	eps = INT_MAX;
	while (eps > 1e-4)  {
		c4 *= 0.9;
		levelforceBased();
	}
	c4 = 0.1;
	for (unsigned int j = 0; j < vertices.size(); ++j) {
		if (vertices[j].h > window.height() - 120) {
			int delta = vertices[j].h - (window.height() - 120);
			for (unsigned int i = 0; i < vertices.size(); ++i) {
				vertices[i].h -= delta;
			}
			break;
		}
	}
	for (unsigned int j = 0; j < vertices.size(); ++j) {
		if (vertices[j].w < 20) {
			int delta = 20 - vertices[j].w;
			for (unsigned int i = 0; i < vertices.size(); ++i) {
				vertices[i].w += delta;
			}
			break;
		}
	}
	prevH = window.height();
	prevW = window.width();
}

void Drawer::drawInitial() {
	QRect window = this->rect();
	vertices.resize(Net->vertices.size());
	for (unsigned int i = 0; i < Net->vertices.size(); ++i) {
		int h = window.height();
		int w = window.width();
		vertices[i] = vertex(i, rand() % w, rand() % h, Net->vertices.size(), Net->vertices[i].e);
	}
}

void Drawer::levelforceBased() {
	QRect window = this->rect();
	
	double distance;
	pair <double, double> force;
	for (unsigned int i = 0; i < vertices.size(); ++i) {
		for (unsigned int j = 0; j < vertices.size(); ++j) {
			if (i == j) {
				vertices[i].forces[j].first = vertices[i].forces[j].second = 0;
				continue;
			}
			double force = 0;
			distance = sqrt(pow(vertices[i].h - vertices[j].h, 2.0) + pow(vertices[i].w - vertices[j].w, 2.0));
			force = c3 / pow(distance, 2.0);
			vertices[i].forces[j] = std::make_pair(force * (vertices[i].w - vertices[j].w) / distance, force * (vertices[i].h - vertices[j].h) / distance); // sin(angle));
			if (Net->net[i].find(j) != Net->net[i].end() && Net->net[j].find(i) != Net->net[j].end()) {
				force = c1 * log(distance / c2);
				vertices[i].forces[j].first -= force * (vertices[i].w - vertices[i].w) / distance;
				vertices[i].forces[j].second -= force * (vertices[i].h - vertices[j].h) / distance; // sin(angle));
			}
		}

		for (unsigned int j = 0; j < vertices[i].forces.size(); ++j) {
			vertices[i].result_force.first += vertices[i].forces[j].first;
			vertices[i].result_force.second += vertices[i].forces[j].second;
		}
	}

	eps = 0;

	for (unsigned int i = 0; i < vertices.size(); ++i) {
		vertices[i].w += c4 * vertices[i].result_force.first;
		vertices[i].h = (levels - vertices[i].level) * ((window.height() - 120) / (levels + 1));
		qDebug() << vertices[i].h << "\n";
		eps = std::max(eps, c4 * sqrt(pow(vertices[i].result_force.first, 2.0) + pow(vertices[i].result_force.second, 2.0)));
	}
}

void Drawer::forceBased() {
	QRect window = this->rect();
	double distance;
	pair <double, double> force;
	for (unsigned int i = 0; i < vertices.size(); ++i) {
		for (unsigned int j = 0; j < vertices.size(); ++j) {
			if (i == j) {
				vertices[i].forces[j].first = vertices[i].forces[j].second = 0;
				continue;
			}
			double force = 0;
			distance = sqrt(pow(vertices[i].h - vertices[j].h, 2.0) + pow(vertices[i].w - vertices[j].w, 2.0));
			force = c3 / pow(distance, 2.0);
				//double angle = atan2((vertices[j].h - vertices[i].h), (vertices[i].w - vertices[j].w));
			vertices[i].forces[j] = std::make_pair(force * (vertices[i].w - vertices[j].w) / distance, force * (vertices[i].h - vertices[j].h) / distance); // sin(angle));
			if (Net->net[i].find(j) != Net->net[i].end() && Net->net[j].find(i) != Net->net[j].end()) {
				force = c1 * log(distance / c2);
				vertices[i].forces[j].first -= force * (vertices[i].w - vertices[i].w) / distance;
				vertices[i].forces[j].second -= force * (vertices[i].h - vertices[j].h) / distance; // sin(angle));
			}
		}

		for (unsigned int j = 0; j < vertices[i].forces.size(); ++j) {
			vertices[i].result_force.first += vertices[i].forces[j].first;
			vertices[i].result_force.second += vertices[i].forces[j].second;
		}
	}

	eps = 0;

	for (unsigned int i = 0; i < vertices.size(); ++i) {
		vertices[i].w += c4 * vertices[i].result_force.first;
		vertices[i].h += c4 * vertices[i].result_force.second;
		eps = std::max(eps, c4 * sqrt(pow(vertices[i].result_force.first, 2.0) + pow(vertices[i].result_force.second, 2.0)));
	}
}

void Drawer::finished(void) {
	delete Next;
	QPainter painter;
	QRect window = this->rect();
	painter.setFont(QFont("Helvetica", 7, QFont::Bold));
	painter.drawText(QPointF(window.width() * 4 / 5, window.height() * 4 / 5), QString("Algorithm Finished!"));
	Text->setEnabled(true);
}

void Drawer::reDraw() {//when changing size
	//some homothety
	QRect window = this->rect();
	int h = window.height();
	int w = window.width();

	int coefH = h / prevH;
	int coefW = w / prevW;

	for (int i = 0; i < vertices.size(); ++i) {
		vertices[i].h = vertices[i].h * coefH;
		vertices[i].w = vertices[i].w * coefW;
	}

	prevH = h;
	prevW = w;
}

void Drawer::afterRelabel(int vertex, int newH) {//when changing vertex place
	qDebug() << "we are inside AfterRelabel\n";
	vertices[vertex].level = newH;
	qDebug() << newH << "\n";
	update();
	eps = INT_MAX;
	while (eps > 1e-4)  {
		c4 *= 0.9;
		//for (int i = 0; i < 150; ++i){
		levelforceBased();
		//update();
	}
	QRect window = this->rect();
	for (unsigned int j = 0; j < vertices.size(); ++j) {
		if (vertices[j].w > window.width() - 20) {
			for (unsigned int i = 0; i < vertices.size(); ++i) {
				vertices[i].h -= 20;
			}
			break;
		}
	}
	for (unsigned int j = 0; j < vertices.size(); ++j) {
		if (vertices[j].w < 20) {
			for (unsigned int i = 0; i < vertices.size(); ++i) {
				vertices[i].w += 20;
			}
			break;
		}
	}
	update();
}

void Drawer::afterPush(int from, int to, int newFlow) {//when changing vertex place
	vertices[from].e = Net->vertices[from].e;
	vertices[to].e = Net->vertices[to].e;
	if (Net->net[from][to].number == -1)
		lastPushed = Net->net[to][from].number;
	else {
		lastPushed = Net->net[from][to].number;
	}
	update();
}

void Drawer::NextClicked() {
		Net->ministep();
}

void Drawer::StartClicked() {
	vertices[0].level = vertices.size();
	for (unsigned int i = 1; i < vertices.size(); ++i) {
		vertices[i].level = 0;
	}
	//static int count = 0;
	//int count = 0;
	eps = INT_MAX;
	while (eps > 1e-4)  {
		c4 *= 0.9;
	//for (int i = 0; i < 150; ++i){
		levelforceBased();
	}
	Net->initialize();
	for (unsigned int i = 0; i < vertices.size(); ++i) {
		vertices[i].w += 50;
		vertices[i].e = Net->vertices[i].e;
	}
	update();
	QPushButton * start = qobject_cast<QPushButton *>(sender());
	start->hide();
}

void Drawer::PrevClicked() {

}

QPointF Drawer::pointOnLine(vertex u, vertex v) {
	int arrowLength = 30;
	int arrowWindth = 5;
	double length = sqrt(pow((u.h - v.h), 2) + pow((u.w - v.w), 2));
	double cos = (u.w - v.w) / length;
	double sin = (u.h - v.h) / length;
	double orthCos = sin;
	double orthSin = -cos;
	vertex point(-1, v.h + arrowLength * sin, v.w + arrowLength * cos, 0, 0);
	return QPointF(point.w, point.h);
}

pair<QPointF, QPointF> Drawer::drawArrow(vertex u, vertex v) {
	int arrowLength = 30;
	int arrowWindth = 5;
	double length = sqrt(pow((u.h - v.h), 2) + pow((u.w - v.w), 2));
	double cos = (u.w - v.w) / length;
	double sin = (u.h - v.h) / length;
	double orthCos = -sin;
	double orthSin = cos;
	vertex point(-1, v.h + arrowLength * sin, v.w + arrowLength * cos, 0, 0);
	vertex f(-1, point.h + orthSin * arrowWindth, point.w + orthCos * arrowWindth, 0, 0);
	vertex t(-1, point.h - orthSin * arrowWindth, point.w - orthCos * arrowWindth, 0, 0);
	return std::make_pair(QPointF(f.w, f.h), QPointF(t.w, t.h));
}

void Drawer::draw() {
	QRect window = this->rect();
	QPainter painter(this);

	vector<int> level(levels);
	int levelH = (window.height() - 120) / (levels + 1);
	for (int i = 1; i <= levels; ++i) {
		int h = levelH * i;
		painter.setPen(Qt::gray);
		painter.drawLine(QPointF(0, h), QPointF(window.width(), h));
	}

	for (vertex &v : vertices) {
		painter.setPen(Qt::black);
		if (v.e == 0)
			painter.setBrush(QBrush(Qt::blue));
		else
			painter.setBrush(QBrush(Qt::yellow));
		if (v.number == 0 || v.number == vertices.size() - 1)
			painter.setBrush(Qt::green);
		painter.drawEllipse(QPointF(v.w, v.h), 10, 10);
		painter.setFont(QFont("Helvetica", 7, QFont::Bold));
		painter.drawText(QPointF(v.w, v.h - 12), QString::number(v.number));
		painter.setPen(Qt::black);
		for (unsigned int i = 0; i < Net->net.size(); ++i) {
			vertex v = vertices[i];
			for (pair<const int, algo::Edge> &e : Net->net[i]) {
				vertex u = vertices[e.first];
				if (e.second.number == lastPushed) {
					QPen pen(Qt::red);
					pen.setWidth(3);
					painter.setPen(pen);
					lastPushed = -2;
				} else
					painter.setPen(Qt::black);
				painter.drawLine(QPointF(v.w, v.h), QPointF(u.w, u.h));
				int label_h = (v.h + u.h) / 2;
				int label_w = (v.w + u.w) / 2 + 10;
				painter.setFont(QFont("Arial", 10));
				if (Net->net[u.number].at(v.number).number != -1) {
					painter.drawText(QPointF(label_w, label_h), QString::number(Net->net[u.number].at(v.number).Flow) + "\/"
						+ QString::number(Net->net[u.number].at(v.number).c));
					pair<QPointF, QPointF> arrow = drawArrow(u, v);
					painter.setBrush(QBrush(Qt::black));
					QVector<QPointF> polygon;
					polygon.push_back(arrow.first);
					polygon.push_back(arrow.second);
					polygon.push_back(QPointF(v.w, v.h));
					
					QPolygonF pol(polygon);
					painter.drawConvexPolygon(pol);
					/*QPointF p = pointOnLine(u, v);
					painter.drawEllipse(p, 2, 2);*/
				}
			}
		}
	}
}

void Drawer::paintEvent(QPaintEvent *event) {
	QRect window = this->rect();
	/*if (prevW != window.width())
		reDraw();
	else */
		draw();
}