#ifndef VIEW_H
#define VIEW_H

#include <vector>
#include <cstdlib>
#include <map>
#include <algorithm>
#include <iostream>
#include <set>
#include "kd_tree.h"
#include "utilities.h"
#include "shapes.h"

using namespace std;

//#ifndef kd_tree
//#ifndef norm

//#ifndef  Rechtangle

struct torch;
struct screen;
using std::vector;


std::pair<point, color>  view_bounding_box(kd_tree_node* subtree, point location,
	vect ray, vector<torch> & torches);
point intersec_bb_ray(bounding_box bb, point location, vect ray);

//class kd_tree;
//struct kd_tree_node;
//
//class sorter : public std::binary_function<point, point, bool> {
//	point viewer;
//public:
//	bool operator()(point const & a, point const & b) const {
//		return square_distance(a, viewer) < square_distance(b, viewer);
//	}
//
//};







//struct objects {
//	kd_tree;
//	objects() {};
//};


//void view(screen & scr, objects & objs, vector<vector<color> > & img);


class Viewer {
public:
	Viewer() {};
	Viewer(point _location) : location(_location){}
	void view(screen & scr, int start, int end, kd_tree *objs, vector<vector<color> > & img, vector<torch> & torches);
	std::pair<Object*, point>  view_bounding_box(kd_tree_node* subtree,
		vect ray, vector<torch> & torches, point loc);
	point intersec_bb_ray(bounding_box bb, point loc, vect ray);
	color calc_color(vector<torch> & torches, std::pair<Object*, point> intersec_point, vect ray);
private:
	point location;
	//void push_ray(vector<point> & scr_points, kd_tree *objs);

	//template<class Type>
	//point check_object_intersec(vect ray, Type & obj);
};




struct screen {
	point center;
	int width;//even
	int height;//even
	vect normal;
	vect x_dir;
	vect y_dir;

	point lh, rh, rb, lb;

	//vector<Rechtangle> box;
	vector<point> scr_points;

	screen() {};
	screen(vect _normal, point _center, vect _x_dir, int width, int height);
	void calc_screen_points();
	void calc_screen_box();
};

//
//
//point closest_from_two(point viewer, point const & a, point const & b);
//
//bool closer(point viewer, point const & a, point const & b);








//template<class Type>
//point Viewer::check_object_intersec(vect ray, Type & obj) {
//	point plane_intersec_point =
//		plane_ray_find_intersection(obj.normal, scalar_mult(obj.normal, obj.x_0), ray, location);
//	bool res = inside(obj, plane_intersec_point);
//	if (res == false)
//		return point(not_a_point, not_a_point, not_a_point);
//	else
//		return plane_intersec_point;
//}


#endif




