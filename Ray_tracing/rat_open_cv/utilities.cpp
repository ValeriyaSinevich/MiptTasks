#include "utilities.h"

long double square_distance(point a, point b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}


point closest_from_two(point viewer, point const & a, point const & b) {
	return ((square_distance(a, viewer) < square_distance(b, viewer)) ? a : b);
}

bool closer(point viewer, point const & a, point const & b) {
	return square_distance(a, viewer) < square_distance(b, viewer);
}

vect calc_normal(point a, point b, point c) {
	return (b - a) * (c - b);
}


vect operator - (point a, point b) {
	return vect(a.x - b.x, a.y - b.y, a.z - b.z);
}

vect operator + (point a, point b) {
	return vect(a.x + b.x, a.y + b.y, a.z + b.z);
}

vect operator * (vect a, vect b) {
	return vect(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
vect operator - (point a) {
	return vect(-a.x, -a.y, -a.z);
}
vect operator * (vect a, double k) {
	return vect(a.x * k, a.y * k, a.z * k);
}

bool operator == (point a, point b) {
	return (a.x == b.x && a.y == b.y && a.z == b.z);
}

bool operator != (point a, point b) {
	return (a.x != b.x || a.y != b.y || a.z != b.z);
}

vect operator / (vect a, double k) {
	return vect(a.x / k, a.y / k, a.z / k);
}

long double scalar_mult(vect a, vect b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

point plane_ray_find_intersection(vect _normal, double d, vect a, point x_0) {
	long double n_x_0 = scalar_mult(_normal, x_0);
	long double n_a = scalar_mult(_normal, a);
	double t = (d - n_x_0) / n_a;
	return x_0 + a * t;
}

color operator + (color a, color b) {
	return (color(a.rgb[0] + b.rgb[0], a.rgb[1] + b.rgb[1], a.rgb[1] + b.rgb[1]));
}

color operator * (color a, double k) {
	return (color(a.rgb[0] * k, a.rgb[1] * k, a.rgb[1] * k));
}

void color::set_intensivity(double k) {
	rgb[0] = std::min(255., rgb[0] * k);
	rgb[1] = std::min(255., rgb[1] * k);
	rgb[2] = std::min(255., rgb[2] * k);
}

double norm2(vect v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

double norm(vect v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vect normalize(vect v) {
	double v_norm = norm(v);
	return v / v_norm;
}

double angle(vect a, vect b) {
	return scalar_mult(a, b) / (norm(a) * norm(b));
}


bool two_points_to_one_side(point fst, point snd, point start, point end) {
	vect side = end - start;
	return (scalar_mult((side * (fst - start)), (side * (snd - start))) >= 0);
}



bool inside_bounding_box(bounding_box bb, point p) {
	if (p.x >= bb.left.x && p.x <= bb.right.x
		&& p.y >= bb.left.y && p.y <= bb.right.y &&
		p.z >= bb.left.z && p.z <= bb.right.z)
		return true;
	else
		return false;
}


double proportional(vect a, vect b) {
	if (a.x / b.x == a.y / b.y && a.x / b.x == a.z / b.z)
		return a.x / b.x;
	else
		return INF;
}
