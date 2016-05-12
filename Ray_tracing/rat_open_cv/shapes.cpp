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
		return point(not_a_point, not_a_point, not_a_point);
	else
		return plane_intersec_point;
}

point Sphere::check_intersection(point p, vect ray) {
	vect normalUp = ((x_0 - p) * ray);
	double distance2 = norm2(normalUp) / norm2(ray);
	if (distance2 > norm2(normal)) {
		return point(not_a_point, not_a_point, not_a_point);
	}
	else {
		double startDistance = sqrt(norm2((x_0 - p)) - distance2);
		vect normedDirection = normalize(ray);
		vect projection = p + normedDirection * startDistance;
		double intersecDistance = sqrt(norm2(normal) - distance2);

		vect candidate1 = projection - normedDirection * intersecDistance;
		vect candidate2 = projection + normedDirection * intersecDistance;

		double t1 = scalar_mult((candidate1 - p), normedDirection);
		double t2 = scalar_mult((candidate2 - p), normedDirection);
		t1 = ((t1 <= 0) ? not_a_point : t1);
		t2 = ((t2 <= 0) ? not_a_point : t2);
		double t = std::min(t1, t2);
		if (t == not_a_point) {
			return point(not_a_point, not_a_point, not_a_point);
		}
		return p + normedDirection * t;
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
	vect projection = normal * scalar_mult(ray, normalize(normal));
	vect diff = projection + ray;
	point d = p - ray + diff * 2;
	return d - p;
}

vect Sphere::calc_reflection(vect ray, point p) {
	normal = p - x_0;
	vect projection = normal * scalar_mult(ray, normalize(normal));
	vect diff = projection + ray;
	point d = p - ray + diff * 2;
	return d - p;
}

