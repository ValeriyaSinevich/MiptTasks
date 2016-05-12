#include "basic_structs.h"

void color::to_rgb() {

}
void color::to_hsv() {

}
void color::set_intensivity(double k) {
	rgb[0] = min(255., rgb[0] * k);
	rgb[1] = min(255., rgb[1] * k);
	rgb[2] = min(255., rgb[2] * k);
}

color black = color(0, 0, 0);


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