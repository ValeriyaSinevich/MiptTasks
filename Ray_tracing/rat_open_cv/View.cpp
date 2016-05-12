#include "View.h"
#include "utilities.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::min;
using std::max;


//bool point_on_interval(vect a, point x_0, point p) {
//	double t = proportional(p - x_0, a);
//	if (t != not_a_point) {
//		point intersec = x_0 + a * t;
//		if (intersec.x < max(x_0.x, (x_0 + a).x) && intersec.x > min(x_0.x, (x_0 + a).x)
//			&& intersec.y < max(x_0.x, (x_0 + a).x) && intersec.y < min()
//			&& intersec.z < max(x_0.x, (x_0 + a).x) && intersec.z < min())
//	}
//}



point intersec_bb_ray(bounding_box bb, point location, vect ray) {
	point t_x_min = location + ray * ((bb.left.x - location.x) / ray.x);
	point t_x_max = location + ray * ((bb.left.x - location.x) / ray.x);

	point t_y_min = location + ray * ((bb.left.y - location.y) / ray.y);
	point t_y_max = location + ray * ((bb.right.y - location.y) / ray.y);

	point t_z_min = location + ray * ((bb.left.z - location.z) / ray.z);
	point t_z_max = location + ray * ((bb.right.z - location.z) / ray.z);

	t_x_min = inside_bounding_box(bb, t_x_min) ? t_x_min : point(0, 0, 0);
	t_x_max = inside_bounding_box(bb, t_x_max) ? t_x_max : point(0, 0, 0);
	t_y_min = inside_bounding_box(bb, t_y_min) ? t_y_min : point(0, 0, 0);
	t_y_max = inside_bounding_box(bb, t_y_max) ? t_y_max : point(0, 0, 0);
	t_z_min = inside_bounding_box(bb, t_z_min) ? t_z_min : point(0, 0, 0);
	t_z_max = inside_bounding_box(bb, t_z_max) ? t_z_max : point(0, 0, 0);

	return closest(location, t_x_min, t_x_max, t_y_min, t_y_max, t_z_min, t_z_max);
}


std::pair<point, color>  view_bounding_box(kd_tree_node* subtree, point location,
	vect ray, vector<torch> & torches) {
	if (subtree->size == 1) {
		point intersec_point = subtree->pivot_obj->check_intersection(location, ray);
		if (intersec_point.x != not_a_point)  {
			double q = 1. / 255;
			//cout << *scr_point << endl << intersec_point << endl;
			double t = 50 * q;
			for (vector<torch>::iterator trch = torches.begin(); trch != torches.end(); ++trch) {
				t += subtree->pivot_obj->calc_color_intensivity(*trch, intersec_point, location) * q;
			}
			color _c = subtree->pivot_obj->c;
			_c.set_intensivity(t);
			return std::make_pair(intersec_point, _c);
		}
		else return std::make_pair(intersec_point, color(0, 0, 0));
	}
	if (subtree->r == nullptr || (intersec_bb_ray(subtree->r->bb, location, ray).x == not_a_point)) {
		return view_bounding_box(subtree->l, location, ray, torches);
	}
	else if (subtree->l == nullptr || (intersec_bb_ray(subtree->l->bb, location, ray).x == not_a_point)) {
		return view_bounding_box(subtree->r, location, ray, torches);
	}
	else {
		point l_bb_intersec = intersec_bb_ray(subtree->l->bb, location, ray);
		point r_bb_intersec = intersec_bb_ray(subtree->r->bb, location, ray);
		if (closer(location, l_bb_intersec, r_bb_intersec)) {
			std::pair<point, color> l_bb_intersec_precise =
				view_bounding_box(subtree->l, location, ray, torches);
			if (l_bb_intersec_precise.first.x != not_a_point &&
				closer(location, l_bb_intersec_precise.first, r_bb_intersec))
				return l_bb_intersec_precise;
			else {
				std::pair<point, color> r_bb_intersec_precise =
					view_bounding_box(subtree->r, location, ray, torches);
				return (closer(location, l_bb_intersec_precise.first, r_bb_intersec_precise.first) ?
					location, l_bb_intersec_precise : r_bb_intersec_precise);
			}
		}
		else {
			std::pair<point, color> r_bb_intersec_precise =
				view_bounding_box(subtree->r, location, ray, torches);
			if (r_bb_intersec_precise.first.x != not_a_point &&
				closer(location, r_bb_intersec_precise.first, l_bb_intersec))
				return r_bb_intersec_precise;
			else {
				std::pair<point, color> l_bb_intersec_precise =
					view_bounding_box(subtree->l, location, ray, torches);
				return closer(location, l_bb_intersec_precise.first, r_bb_intersec_precise.first) ?
					location, l_bb_intersec_precise : r_bb_intersec_precise;
			}
		}
	}
}



void Viewer::view(screen & scr, int start, int end, kd_tree *objs_container,
	vector<vector<color> > & img, vector<torch> & torches) {

	//std::map<point, color, sorter> intersec_points;

	int  k = start, l = 0, m = 0;

	for (unsigned int i = start; i < end; ++i) {
		//intersec_points.clear();
		if (scr.scr_points[i] == point(0, 0, 100)) {
			cout << " control" << endl;
		}

		vect ray = normalize(scr.scr_points[i] - location);

		std::pair<point, color> intersec_point = view_bounding_box(objs_container->root, location, ray, torches);

		l = k / scr.width;
		m = k % scr.width;
		img[l][m] = intersec_point.second;
		k++;
		//cout << img[19][19].rgb[0] << endl;
	}
	//delete objs_container;
}


color black = color(0, 0, 0);


screen::screen(vect _normal, point _center, vect _x_dir, int _width, int _height) : x_dir(normalize(_x_dir)),
normal(normalize(_normal)), center(_center), width(_width), height(_height) {
	y_dir = normalize(_x_dir * _normal);
	//cout << x_dir << endl << y_dir << endl << normal << endl;
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


double Planar_Object::calc_color_intensivity(torch & t, point p, point camera_location) {
	if (scalar_mult(normal, camera_location - p) < 0)
		normal = -normal;
	double cos = std::max(0., angle(t.location - p, normal));
	double distance = square_distance(t.location, p);
	return t.intensivity / distance * cos;
}

double Sphere::calc_color_intensivity(torch & t, point p, point camera_location) {
	if (scalar_mult(normal, camera_location - p) < 0)
		normal = -normal;
	double cos = std::max(0., angle(t.location - p, normal));
	double distance = square_distance(t.location, p);
	return t.intensivity / distance * cos;
}






