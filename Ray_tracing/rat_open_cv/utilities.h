#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <fstream>
#include <algorithm>
using std::vector;
const double not_a_point = 1e18;

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

bool closer(point viewer, point const & a, point const & b);
point closest_from_two(point viewer, point const & a, point const & b);
double norm(vect v);
vect normalize(vect v);
double norm2(vect v);
vect calc_normal(point a, point b, point c);
long double square_distance(point a, point b);
vect operator - (point a, point b);
vect operator - (point a);
vect operator + (point a, point b);
bool operator == (point a, point b);
vect operator * (vect a, vect b);
vect operator * (vect a, double k);
vect operator / (vect a, double k);
long double scalar_mult(vect a, vect b);
point plane_ray_find_intersection(vect _normal, double d, vect a, point x_0);
bool two_points_to_one_side(point fst, point snd, point start, point end);
double angle(vect a, vect b);
bool two_points_to_one_side(point fst, point snd, point start, point end);
bool inside_bounding_box(bounding_box bb, point p);
double proportional(vect a, vect b);


template <typename T>
T& closest(T viewer, T a, T b) {
	return closest_from_two(viewer, a, b);
}

template<class T, typename... Args>
T & closest(T viewer, T a, T b, Args ... args) {
	return closest(viewer, closest_from_two(viewer, a, b), args...);
}

#endif