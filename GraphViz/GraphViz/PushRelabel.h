#ifndef PUSHRELABEL
#define PUSHRELABEL
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <fstream>
#include <unordered_map>
#include <climits>
#include <QObject>
#include <QDebug>

using std::vector;
using std::pair;
using std::find_if;
using std::min;
using std::cin;
using std::cout;
using std::unordered_map;
using std::queue;

namespace algo {

	class Edge{
	public:
		int from, to, number;
		long long c, Flow;
		Edge(){}
		Edge(int g, int a, int d, int n) : from(g), to(a), c(d), Flow(0), number(n){}
	};

	class vertex{
	public:
		vertex(int h, long long e) :h(h), e(e){ }
		long long e;
		int h, no;
		unordered_map <int, Edge>::iterator next;
	};

	class network : public QObject{
		Q_OBJECT
	public:
		network(int nV, QObject* parent = 0);

		vector<unordered_map <int, Edge> > net;
		vector<vertex> vertices;

		queue <int> q;


		int curDischarging = -1;
		bool dischargeStepDone = 0;

		void addEdge(int s, int t, int c, int n);

		void Discharge(int u);
		void DischargeStep(int u);

		void PreFlow();
		void Step();
		void ministep();

		int minH(int u);

		void initialize();
		void Relabel(int u);
		bool push(int u, int v);
	signals:
		void VertexRelabeled(int, int);
		void EdgePushed(int, int, int);
		void finished(void);
	};

	bool lessNumber(Edge a, Edge b);
}
#endif 
