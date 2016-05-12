#ifndef BASIC_STRUCTS_H
#define BASIC_STRUCTS_H

#include <vector>
#include <cstdlib>
#include <map>
#include <algorithm>
#include <iostream>
#include <set>

#include "shapes.h"
#include "utilities.h"


using std::vector;

struct point {
	double x, y, z;
	point(double a, double b, double c) : x(a), y(b), z(c) {}
	point() : x(0), y(0), z(0) {}

	friend std::ostream & operator << (std::ostream & output,
		const point &p) {
		output << p.x << ' ' << p.y << ' ' << p.z << ' ';
		return output;
	}
};

typedef point vect;


struct bounding_box {
	point left, right;
	bounding_box(point _a, point _b) : left(_a), right(_b) {}
	bounding_box() {};
};


struct color {
	vector<int> rgb = vector<int>(3);
	vector<int> hsv = vector<int>(3);
	color(int a, int b, int c) {
		rgb[0] = a;
		rgb[1] = b;
		rgb[2] = c;
	}
	color() : color(0, 0, 0) {};

	void to_rgb();
	void to_hsv();
	void set_intensivity(double k);
};



#endif
