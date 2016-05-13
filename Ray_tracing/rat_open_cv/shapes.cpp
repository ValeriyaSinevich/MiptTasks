#include "shapes.h"

bool Triangle::inside(point p) {
	return (
		two_points_to_one_side(fst, p, snd, thrd) &&
		two_points_to_one_side(snd, p, fst, thrd) &&
		two_points_to_one_side(thrd, p, fst, snd));
}

bool Rechtangle::inside(point p) {
	return (
		two_points_to_one_side(fst, p, snd, thrd) &&
		two_points_to_one_side(snd, p, thrd, fourth) &&
		two_points_to_one_side(fourth, p, fst, snd) &&
		two_points_to_one_side(thrd, p, fst, fourth));
}

point Planar_Object::check_intersection(point p, vect ray) {
	point plane_intersec_point =
		plane_ray_find_intersection(normal, scalar_mult(normal, x_0), ray, p);
	bool res = inside(plane_intersec_point);
	if (res == false)
		return not_a_point;
	else
		return plane_intersec_point;
}

point Sphere::check_intersection(point p, vect ray) {
	vect normed_ray = normalize(ray);
	point proj = p + normed_ray * scalar_mult(x_0 - p, normed_ray);
	double distance2 = norm2(proj - x_0);
	if (distance2 > norm2(normal)) {
		return not_a_point;
	}
	else {
		double intersec_distance = sqrt(norm2(normal) - distance2);

		vect candidate1 = proj - normed_ray * intersec_distance;
		vect candidate2 = proj - normed_ray * intersec_distance;

		double t1 = scalar_mult((candidate1 - p), normed_ray);
		double t2 = scalar_mult((candidate2 - p), normed_ray);
		t1 = ((t1 <= 0) ? INF : t1);
		t2 = ((t2 <= 0) ? INF : t2);
		double t = std::min(t1, t2);
		if (t == INF) {
			return not_a_point;
		}
		return p + normed_ray * t;
	}
}

void Sphere::calc_bounding_box() {
	double left_x = x_0.x - norm(normal);
	double left_y = x_0.y - norm(normal);
	double left_z = x_0.z - norm(normal);

	double right_x = x_0.x + norm(normal);
	double right_y = x_0.y + norm(normal);
	double right_z = x_0.z + norm(normal);

	bb = bounding_box(point(left_x, left_y, left_z), point(right_x, right_y, right_z));
}

vect Planar_Object::calc_reflection(vect ray, point p) {
	normal = normalize(normal);
	if (scalar_mult(ray, normalize(normal)) > 0)
		normal = -normal;
	vect projection = normal * scalar_mult(ray, normal/*normalize(normal)*/);
	vect diff = projection + ray;
	return -ray + diff * 2;
}

vect Sphere::calc_reflection(vect ray, point p) {
	//normal = normalize(p - x_0);
	normal = p - x_0;
	long double k = scalar_mult(ray, normalize(normal));
	vect projection = normalize(normal) * scalar_mult(ray, normalize(normal));
	vect diff = projection + ray;
	return -ray + diff * 2;
}



void Triangle::calc_bounding_box() {
	double x = std::min(fst.x, std::min(snd.x, thrd.x));
	double y = std::min(fst.y, std::min(snd.y, thrd.y));
	double z = std::min(fst.z, std::min(snd.z, thrd.z));
	bb.left = point(x, y, z);

	x = std::max(fst.x, std::max(snd.x, thrd.x));
	y = std::max(fst.y, std::max(snd.y, thrd.y));
	z = std::max(fst.z, std::max(snd.z, thrd.z));
	bb.right = point(x, y, z);
}


void Rechtangle::calc_bounding_box() {
	double x = std::min(fst.x, std::min(snd.x, std::min(thrd.x, fourth.x)));
	double y = std::min(fst.y, std::min(snd.y, std::min(thrd.z, fourth.z)));
	double z = std::min(fst.z, std::min(snd.z, std::min(thrd.y, fourth.y)));
	bb.left = point(x, y, z);

	x = std::max(fst.x, std::max(snd.x, thrd.x));
	y = std::max(fst.y, std::max(snd.y, thrd.y));
	z = std::max(fst.z, std::max(snd.z, thrd.z));
	bb.right = point(x, y, z);
}


