#ifndef SHAPES_H
#define SHAPES_H

#include <algorithm>
#include <vector>
#include "utilities.h"

struct torch {
	point location;
	int intensivity;
	torch() : location(0, 0, 0), intensivity(0) {}
	torch(point p, int _intensivity) : location(p), intensivity(_intensivity) {}
};



struct Object {
	color c;

	point x_0;
	bounding_box bb;
	Object(){}
	Object(point _x_0) : x_0(_x_0), c(0, 0, 0) {}
	Object(point p, color _c) : x_0(p), c(_c) {}

	virtual point check_intersection(point p, vect ray) = 0;
	virtual double calc_color_intensivity(torch & t, point p, point camera_location) = 0;
	virtual void calc_bounding_box() = 0;
};


struct Planar_Object : public Object {
	vect normal;
	Planar_Object(vect _normal, point _x_0) : Object(_x_0), normal(_normal) {
	}
	Planar_Object(vect _normal, point _x_0, color _c) : Object(_x_0, _c), normal(_normal) {}

	virtual bool inside(point p) = 0;
	virtual point check_intersection(point p, vect ray);
	virtual double calc_color_intensivity(torch & t, point p, point camera_location);
	virtual void calc_bounding_box() = 0;
};



struct Sphere : public Object {
	Sphere(vect _normal, point _x_0) : Object(_x_0), normal(_normal)  {}
	vect normal;

	point check_intersection(point p, vect ray);
	double calc_color_intensivity(torch & t, point p, point camera_location);
	void calc_bounding_box();
};



struct Triangle : public Planar_Object {
	point fst, snd, thrd;
	Triangle(vect _normal, point _fst, point _snd, point _thrd) : Planar_Object(_normal, _fst),
		fst(_fst), snd(_snd), thrd(_thrd) {}
	Triangle(vect _normal, color _c, point _fst, point _snd, point _thrd)
		: Planar_Object(_normal, _fst, _c), fst(_fst), snd(_snd), thrd(_thrd) {}

	virtual bool inside(point p);
	virtual void calc_bounding_box();
	//Triangle(color _c, point _fst, point _snd, point _thrd)
	//: Object(calc_normal(_fst, _snd, _thrd), _fst, _c), fst(_fst), snd(_snd), thrd(_thrd) {}
};

struct Rechtangle : public Planar_Object {
	point fst, snd, thrd, fourth;//from leftest highest clockwise
	Rechtangle(vect _normal, point _fst, point _snd, point _thrd, point _fourth) : Planar_Object(_normal, _fst),
		fst(_fst), snd(_snd), thrd(_thrd), fourth(_fourth) {}
	Rechtangle(vect _normal, color _c, point _fst, point _snd, point _thrd, point _fourth)
		: Planar_Object(_normal, _fst, _c),
		fst(_fst), snd(_snd), thrd(_thrd), fourth(_fourth) {}

	virtual bool inside(point p);
	virtual void calc_bounding_box();
};

#endif