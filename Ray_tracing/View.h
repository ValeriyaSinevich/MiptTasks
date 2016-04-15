#include <vector>
#include <cstdlib>
#include <map>
#include <algorithm>
#include <iostream>

using std::vector;

const double not_a_point = 1e18;


struct point {
	double x, y, z;
	point(double a, double b, double c) : x(a), y(b), z(c) {}
	point() : x(0), y(0), z(0) {}

	friend std::ostream & operator << (std::ostream & output,
		const point &p){
		output << p.x << ' ' << p.y << ' ' << p.z << ' ';
		return output;
	}
};

typedef point vect;


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


struct Object {
	vect normal;
	color c;
	point x_0;
	Object(vect _normal, point _x_0) : normal(_normal), x_0(_x_0), c(0, 0, 0) {}
	Object(vect _normal, point _x_0, color _c) : normal(_normal), x_0(_x_0), c(_c) {}
};

struct Triangle : public Object {
	point fst, snd, thrd;
	Triangle(vect _normal, point _fst, point _snd, point _thrd) : Object(_normal, _fst),
		fst(_fst), snd(_snd), thrd(_thrd) {}
	Triangle(vect _normal, color _c, point _fst, point _snd, point _thrd)
		: Object(_normal, _fst, _c), fst(_fst), snd(_snd), thrd(_thrd) {}
	//Triangle(color _c, point _fst, point _snd, point _thrd)
		//: Object(calc_normal(_fst, _snd, _thrd), _fst, _c), fst(_fst), snd(_snd), thrd(_thrd) {}
};

struct Rechtangle : public Object {
	point fst, snd, thrd, fourth;//from leftest highest clockwise
	Rechtangle(vect _normal, point _fst, point _snd, point _thrd, point _fourth) : Object(_normal, _fst),
		fst(_fst), snd(_snd), thrd(_thrd), fourth(_fourth) {}
	Rechtangle(vect _normal, color _c, point _fst, point _snd, point _thrd, point _fourth)
		: Object(_normal, _fst, _c),
		fst(_fst), snd(_snd), thrd(_thrd), fourth(_fourth) {}
};

struct torch {
	point location;
	int intensivity;
	torch() {}
	torch(point p, int _intensivity) : location(p), intensivity(_intensivity) {}
};


vect normalize(vect v);
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
bool inside(Triangle t, point p);
bool inside(Rechtangle t, point p);
double calc_color_intensivity(torch & t, point p, vect normal);
double angle(vect a, vect b);


struct objects {
	vector<Triangle> triangles;
	objects() {};
};


struct screen {
	point center;
	int width;//even
	int height;//even
	vect normal;
	vect x_dir;
	vect y_dir;

	point lh, rh, rb, lb;

	vector<Rechtangle> box;
	vector<point> scr_points;
	
	screen() {};
	screen(vect _normal, point _center, vect _x_dir, int width, int height);
	void calc_screen_points();
	void calc_screen_box();
};
//void view(screen & scr, objects & objs, vector<vector<color> > & img);


class Viewer {
public:
	Viewer() {};
	Viewer(point _location) : location(_location){}
	void view(screen & scr, int start, int end, objects & objs, vector<vector<color> > & img, vector<torch> & torches);
private:
	point location;
	void push_ray(vector<point> & scr_points, objects & objs);

	template<class Type>
	point check_object_intersec(vect ray, Type & obj);
};


template<class Type>
point Viewer::check_object_intersec(vect ray, Type & obj) {
	point plane_intersec_point =
		plane_ray_find_intersection(obj.normal, scalar_mult(obj.normal, obj.x_0), ray, location);
	bool res = inside(obj, plane_intersec_point);
	if (res == false)
		return point(not_a_point, not_a_point, not_a_point);
	else
		return plane_intersec_point;
}




