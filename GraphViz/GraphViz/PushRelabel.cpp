#include "PushRelabel.h"

#define INF INT_MAX

using namespace algo;


network::network(int nV, QObject* parent){
		net.resize(nV);
		vertices.resize(nV, vertex(0, 0));
		for (unsigned int i = 0; i < vertices.size(); ++i) {
			vertices[i].no = i;
		}
}

void network::addEdge(int s, int t, int c, int n) {
	if (net[s].find(t) == net[s].end())
		net[s][t] = Edge(s, t, c, n);
	else
		net[s][t].c += c;

	if (net[t].find(s) == net[t].end()) {
		net[t].insert(std::make_pair(s, Edge(t, s, 0, -1)));
	}
}

void network::Discharge(int u) {
	while (vertices[u].e > 0 && vertices[u].next != net[u].end()) {
		int v = vertices[u].next->second.to;
		if (push(u, v))
			vertices[u].next++;
	}
	if (vertices[u].e > 0)
		Relabel(u);
	else
		q.pop();
}

void network::Step() {
	int u = q.front();
	Discharge(u);
}

void network::DischargeStep(int u) {
	dischargeStepDone = 0;
	while (!dischargeStepDone && vertices[u].e > 0 && vertices[u].next != net[u].end()) {
		int v = vertices[u].next->second.to;
		if (push(u, v))
			vertices[u].next++;
	}
	if (!dischargeStepDone) {
		if (vertices[u].e > 0)
			Relabel(u);
		else {
			q.pop();
			curDischarging = -1;
			if (q.empty())
				emit this->finished();
			else {
				curDischarging = q.front();
				DischargeStep(curDischarging);
			}
		}
	}
}
void network::ministep() {
	if (curDischarging == -1) {
		if (q.empty())
			emit this->finished();
		else {
			curDischarging = q.front();
			DischargeStep(curDischarging);
		}
	}
	else {
		DischargeStep(curDischarging);
		if (vertices[curDischarging].e == 0) {
			q.pop();
			curDischarging = -1;
			if (q.empty())
				emit this->finished();
			else {
				curDischarging = q.front();
			}
		}
	}
}

void network::PreFlow(){
	initialize();

	while (!q.empty()) {
		Step();
	}
}

int network::minH(int u){
	int min = INT_MAX;

	for (unordered_map<int, Edge> ::iterator v = net[u].begin(); v != net[u].end(); ++v){
		if (v->second.c - v->second.Flow > 0 && vertices[v->second.to].h < min)
			min = vertices[v->second.to].h;
	}
	return min;
}

void network::initialize() {
	for (unsigned int i = 0; i < vertices.size(); ++i)
		vertices[i].next = net[i].begin();

	for (unordered_map <int, Edge> ::iterator v = net[0].begin(); v != net[0].end(); ++v) {
		v->second.Flow = v->second.c;
		net[v->second.to][0].Flow = -v->second.c;
		vertices[v->second.to].e += v->second.c;
		if (v->second.to != vertices.size() - 1){
			q.push(v->second.to);
		}
		vertices[0].e -= v->second.c;
	}
	vertices[0].h = vertices.size();
}


void network::Relabel(int u) {
	if (u == vertices.size() - 1 || u == 0)
		return;
	int prevH = vertices[u].h;
	vertices[u].h = minH(u) + 1;
	vertices[u].next = net[u].begin();
	if (prevH == vertices[u].h)
		DischargeStep(u);
	else
		emit(this->VertexRelabeled(u, vertices[u].h));
}


bool network::push(int u, int v) {
	if (vertices[u].e <= 0 || u == vertices.size() - 1 || u == 0 || !(vertices[u].h == vertices[v].h + 1))
		return true;
	else{
		if (vertices[v].e == 0 && v != vertices.size() - 1 && v != 0 && net[u][v].c - net[u][v].Flow != 0)
			q.push(v);
		long long addVal = min(vertices[u].e, net[u][v].c - net[u][v].Flow);
		net[u][v].Flow += addVal;
		net[v][u].Flow -= addVal;
		vertices[u].e -= addVal;
		vertices[v].e += addVal;
		if (addVal > 0) {
			emit this->EdgePushed(u, v, net[v][u].Flow);
			dischargeStepDone = 1;
		}
		return (addVal == net[u][v].c - net[u][v].Flow);
	}
}


bool lessNumber(Edge a, Edge b){
	return (a.number < b.number);
}
