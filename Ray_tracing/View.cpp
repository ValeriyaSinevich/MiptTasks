#include "View.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::min;
using std::max;

color black = color(0, 0, 0);

long double square_distance(point a, point b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}

class sorter : public std::binary_function<point, point, bool> {
	point viewer;
public:
	bool operator()(point const & a, point const & b) const {
		return square_distance(a, viewer) < square_distance(b, viewer);
	}

};

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
vect operator - (point a){
	return vect(-a.x, -a.y, -a.z);
}
vect operator * (vect a, double k) {
	return vect(a.x * k, a.y * k, a.z * k);
}

bool operator == (point a, point b) {
	return (a.x == b.x && a.y == b.y && a.z == b.z);
}

vect operator / (vect a, double k) {
	return vect(a.x / k, a.y / k, a.z / k);
}

long double scalar_mult (vect a, vect b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

point plane_ray_find_intersection(vect _normal, double d, vect a, point x_0) {
	long double n_x_0 = scalar_mult(_normal,  x_0);
	long double n_a = scalar_mult(_normal,  a);
	double t = (d - n_x_0) / n_a;
	return x_0 + a * t;
}

double norm(vect v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vect normalize(vect v) {
	double v_norm = norm(v);
	return v / v_norm;
}

screen::screen(vect _normal, point _center, vect _x_dir, int _width, int _height) : x_dir(normalize(_x_dir)),
	normal(normalize(_normal)), center(_center), width(_width), height(_height) {
	y_dir = normalize(_x_dir * _normal);
	//cout << x_dir << endl << y_dir << endl << normal << endl;
}


void screen::calc_screen_box() {
	if (scr_points.size() == 0)
		return;
	else {
		Rechtangle left(x_dir, black, lh, lh + normal * width, lb + normal * width, lb);
		box.push_back(left);
		Rechtangle right(x_dir, black, rh, rh + normal * width, rb + normal * width, rb);
		box.push_back(right);
		Rechtangle bottom(y_dir, black, lb, lb + normal * width, rb + normal * width, rb);
		box.push_back(bottom);
		Rechtangle up(y_dir, black, lh, lh + normal * width, rh + normal * width, rh);
		box.push_back(up);
	}
}


void screen::calc_screen_points() {
	for (int i = -height / 2; i < height / 2; ++i) {
		for (int j = -width / 2; j < width / 2; ++j) {
			//cout << center + y_dir * i + x_dir * j;
			//cout << y_dir * i << endl << x_dir * j << endl;
			scr_points.push_back(center + y_dir * i + x_dir * j);
		}
	}
	//std::cout << scr_points.size();
	lh = scr_points[0];

	rh = scr_points[width - 1];
	rb = scr_points[scr_points.size() - 1];
	lb = scr_points[width * (height - 1)];
}


bool two_points_to_one_side(point fst, point snd, point start, point end) {
	vect side = end - start;
	return (scalar_mult((side * (fst - start)), (side * (snd - start))) >= 0);
}

double proportional(vect a, vect b) {
	if (a.x / b.x == a.y / b.y && a.x / b.x == a.z / b.z)
		return a.x / b.x;
	else
		return not_a_point;
}

//bool point_on_interval(vect a, point x_0, point p) {
//	double t = proportional(p - x_0, a);
//	if (t != not_a_point) {
//		point intersec = x_0 + a * t;
//		if (intersec.x < max(x_0.x, (x_0 + a).x) && intersec.x > min(x_0.x, (x_0 + a).x)
//			&& intersec.y < max(x_0.x, (x_0 + a).x) && intersec.y < min()
//			&& intersec.z < max(x_0.x, (x_0 + a).x) && intersec.z < min())
//	}
//}

bool inside(Triangle t, point p) {
	return (
		two_points_to_one_side(t.fst, p, t.snd, t.thrd) &&
		two_points_to_one_side(t.snd, p, t.fst, t.thrd) &&
		two_points_to_one_side(t.thrd, p, t.fst, t.snd));
}

bool inside(Rechtangle t, point p) {
	return (
		two_points_to_one_side(t.fst, p, t.snd, t.thrd) &&
		two_points_to_one_side(t.snd, p, t.thrd, t.fourth) &&
		two_points_to_one_side(t.fourth, p, t.fst, t.snd) &&
		two_points_to_one_side(t.thrd, p, t.fst, t.fourth));
}

double angle(vect a, vect b) {
	return scalar_mult(a, b) / (norm(a) * norm(b));
}


double calc_color_intensivity(torch & t, point p, vect normal) {
	if (scalar_mult(normal, p - t.location) < 0)
		normal = -normal;
	double cos = max(0., angle(p - t.location, normal));
	double distance = square_distance(t.location, p);
	return t.intensivity / distance * cos;
}



void Viewer::view(screen & scr, int start, int end, objects & objs, vector<vector<color> > & img, vector<torch> & torches) {

	std::map<point, color, sorter> intersec_points;

	int  k = start, l = 0, m = 0;

	for (unsigned int i = start; i < end; ++i) {
		intersec_points.clear();
		if (scr.scr_points[i] == point(0, 0, 100)) {
			cout << " control" << endl;
		}

		vect ray = normalize(scr.scr_points[i] - location);
		for (vector<Triangle>::iterator obj = objs.triangles.begin(); obj != objs.triangles.end(); obj++) {
			point intersec_point = check_object_intersec<Triangle>(ray, *obj);

			if (intersec_point.x != not_a_point)  {
				double q = 1. / 255;
				//cout << *scr_point << endl << intersec_point << endl;
				double t = 50 * q;
				for (vector<torch>::iterator trch = torches.begin(); trch != torches.end(); ++trch) {
					t += calc_color_intensivity(*trch, intersec_point, obj->normal) * q;
				}

				color _c = obj->c;
				_c.set_intensivity(t);
				intersec_points.insert(std::make_pair(intersec_point, _c));
			}
		}
		//for (vector<Rechtangle>::iterator wall = scr.box.begin(); wall != scr.box.end(); wall++) {
		//	point intersec_point = check_object_intersec<Rechtangle>(ray, *wall);
		//	cout << *scr_point << endl << intersec_point << endl << endl;
		//	if (intersec_point.x >= 0)
		//		intersec_points.insert(std::make_pair(check_object_intersec(ray, *wall), wall->c));
		//}
		if (intersec_points.size() > 0) {
			std::pair<point, color> first_intersection_point = *intersec_points.begin();
			l = k / scr.width;
			m = k % scr.width;
			//cout << l << ' ' << m << endl;
			//cout << img[19][19].rgb[0] << endl;
			img[l][m] = first_intersection_point.second;

		}
		k++;
		//cout << img[19][19].rgb[0] << endl;
	}

}

void color::to_rgb() {

}
void color::to_hsv() {

}
void color::set_intensivity(double k) {
	rgb[0] = min(255., rgb[0]* k);
	rgb[1] = min(255., rgb[1] * k);
	//cout << rgb[2] * k << endl;
	rgb[2] = min(255., rgb[2] * k);
}


