#include "View.h"
#include "utilities.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::min;
using std::max;

const color black(0, 0, 0);


//bool point_on_interval(vect a, point x_0, point p) {
//	double t = proportional(p - x_0, a);
//	if (t != not_a_point) {
//		point intersec = x_0 + a * t;
//		if (intersec.x < max(x_0.x, (x_0 + a).x) && intersec.x > min(x_0.x, (x_0 + a).x)
//			&& intersec.y < max(x_0.x, (x_0 + a).x) && intersec.y < min()
//			&& intersec.z < max(x_0.x, (x_0 + a).x) && intersec.z < min())
//	}
//}



point Viewer::intersec_bb_ray(bounding_box bb, point loc, vect ray) {
	point t_x_min = loc + ray * ((bb.left.x - loc.x) / ray.x);
	point t_x_max = loc + ray * ((bb.right.x - loc.x) / ray.x);

	point t_y_min = loc + ray * ((bb.left.y - loc.y) / ray.y);
	point t_y_max = loc + ray * ((bb.right.y - loc.y) / ray.y);

	point t_z_min = loc + ray * ((bb.left.z - loc.z) / ray.z);
	point t_z_max = loc + ray * ((bb.right.z - loc.z) / ray.z);

	t_x_min = inside_bounding_box(bb, t_x_min) ? t_x_min : not_a_point;
	t_x_max = inside_bounding_box(bb, t_x_max) ? t_x_max : not_a_point;
	t_y_min = inside_bounding_box(bb, t_y_min) ? t_y_min : not_a_point;
	t_y_max = inside_bounding_box(bb, t_y_max) ? t_y_max : not_a_point;
	t_z_min = inside_bounding_box(bb, t_z_min) ? t_z_min : not_a_point;
	t_z_max = inside_bounding_box(bb, t_z_max) ? t_z_max : not_a_point;

	return closest(loc, t_x_min, t_x_max, t_y_min, t_y_max, t_z_min, t_z_max);
}


std::pair<Object*, point>  Viewer::view_bounding_box(kd_tree_node* subtree,
	vect ray, point loc) {
	if (subtree->size == 1) {
		point intersec_point = subtree->pivot_obj->check_intersection(loc, ray);
		if (intersec_point != not_a_point)  {
			//double q = 1. / 255;
			////cout << *scr_point << endl << intersec_point << endl;
			//double t = 50 * q;
			//for (vector<torch>::iterator trch = torches.begin(); trch != torches.end(); ++trch) {
			//	t += subtree->pivot_obj->calc_color_intensivity(*trch, intersec_point, location) * q;
			//}
			//color _c = subtree->pivot_obj->c;
			//_c.set_intensivity(t);
			return (std::make_pair(subtree->pivot_obj, intersec_point));
		}
		else return std::make_pair(subtree->pivot_obj, intersec_point);
	}
	point l_bb_intersec = intersec_bb_ray(subtree->l->bb, loc, ray);
	point r_bb_intersec = intersec_bb_ray(subtree->r->bb, loc, ray);
	if (subtree->r == nullptr || (r_bb_intersec == not_a_point)) {
		return view_bounding_box(subtree->l, ray, loc);
	}
	else if (subtree->l == nullptr || (l_bb_intersec == not_a_point)) {
		return view_bounding_box(subtree->r, ray, loc);
	}
	else {
		/*point l_bb_intersec = intersec_bb_ray(subtree->l->bb, loc, ray);
		point r_bb_intersec = intersec_bb_ray(subtree->r->bb, loc, ray);*/
		if (closer(loc, l_bb_intersec, r_bb_intersec)) {
			std::pair<Object*, point> l_bb_intersec_precise =
				view_bounding_box(subtree->l,  ray, loc);
			if (l_bb_intersec_precise.second != not_a_point &&
				closer(loc, l_bb_intersec_precise.second, r_bb_intersec))
				return l_bb_intersec_precise;
			else {
				std::pair<Object*, point> r_bb_intersec_precise =
					view_bounding_box(subtree->r, ray, loc);
				return (closer(loc, l_bb_intersec_precise.second, r_bb_intersec_precise.second) ?
					l_bb_intersec_precise : r_bb_intersec_precise);
			}
		}
		else {
			std::pair<Object*, point> r_bb_intersec_precise =
				view_bounding_box(subtree->r, ray, loc);
			if (r_bb_intersec_precise.second != not_a_point &&
				closer(loc, r_bb_intersec_precise.second, l_bb_intersec))
				return r_bb_intersec_precise;
			else {
				std::pair<Object*, point> l_bb_intersec_precise =
					view_bounding_box(subtree->l, ray, loc);
				return closer(loc, l_bb_intersec_precise.second, r_bb_intersec_precise.second) ?
					l_bb_intersec_precise : r_bb_intersec_precise;
			}
		}
	}
}


color Viewer::calc_color(kd_tree* objs_container, vector<torch> & torches, std::pair<Object*, point> intersec_point, vect ray, int default_light) {
	double q = 1. / 255;
	//cout << *scr_point << endl << intersec_point << endl;
	double t = default_light * q;
 	for (vector<torch>::iterator trch = torches.begin(); trch != torches.end(); ++trch) {
		vect ray_from_torch = intersec_point.second - trch->location;
		std::pair<Object*, point>  ray_from_torch_intersec =
			view_bounding_box(objs_container->root, ray_from_torch, trch->location);
		bool a = (ray_from_torch_intersec.second == intersec_point.second);
		if (a)
			t += intersec_point.first->calc_color_intensivity(*trch, intersec_point.second, ray) * q;
	}
	color c = intersec_point.first->c;
	c.set_intensivity(t);
	return c;
}



void Viewer::view(screen & scr, int start, int end, kd_tree *objs_container,
	vector<vector<color> > & img, vector<torch> & torches) {

	//std::map<point, color, sorter> intersec_points;

	int  k = start, l = 0, m = 0;

	for (unsigned int i = start; i < end; ++i) {
		//intersec_points.clear();
		if (scr.scr_points[i] == point(0, 25, 100)) {
			cout << " control" << endl;
		}
		if (i == 189597) {
			cout << " control" << endl;
		}

		vect ray = normalize(scr.scr_points[i] - location);

		std::pair<Object*, point> intersec_point = view_bounding_box(objs_container->root, ray, location);

		if (intersec_point.second == not_a_point) {
			l = k / scr.width;
			m = k % scr.width;
			img[l][m] = black;
			k++;
			continue;
		}

		vect reflected_ray = normalize(intersec_point.first->calc_reflection(ray, intersec_point.second));

		std::pair<Object*, point> intersec_point_reflected =
			view_bounding_box(objs_container->root, reflected_ray, intersec_point.second);

		
		color c = calc_color(objs_container, torches, intersec_point, ray, 50);

		if (intersec_point_reflected.second != not_a_point) {
			color add_c = calc_color(objs_container, torches, intersec_point_reflected, reflected_ray, 0);
			//double distance = square_distance(intersec_point_reflected.second, intersec_point.second);
			//add_c = add_c * (1 /  distance);
			double koef = intersec_point.first->reflect_k /*/distance*/;
			if (add_c != black)
				c = add_c * koef + c * (1 - koef);
			/*if (c.rgb[0] == 0 && c.rgb[1] == 0 && c.rgb[2] == 0)
				cout << "black\n";*/
		}

		l = k / scr.width;
		m = k % scr.width;
		img[l][m] = c;
		k++;
		//cout << img[19][19].rgb[0] << endl;
	}
	//delete objs_container;
}





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


double Planar_Object::calc_color_intensivity(torch & t, point p, vect ray) {
	if (scalar_mult(normal, -ray) < 0)
		normal = -normal;	
	double cos = std::max(0., angle(t.location - p, normal));
	double distance = square_distance(t.location, p);
	return t.intensivity / distance * cos;
}

double Sphere::calc_color_intensivity(torch & t, point p,vect ray) {
	normal = p - x_0;
	if (scalar_mult(normal, -ray) < 0)
		normal = -normal;
	double cos = std::max(0., angle(t.location - p, normal));
	double distance = square_distance(t.location, p);
	return t.intensivity / distance * cos;
}






