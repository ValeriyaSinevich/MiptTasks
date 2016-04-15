#include "parser.h"

Parser::Parser() {}

point Parser::parse_input(screen & scr, objects & objs, vector<torch> & torches) {
	point a(-50, 0, 100);
	point b(50, 0, 100);
	point c(0, 100, 100);
	vect n = calc_normal(a, b, c);
	color col(255, 0, 0);
	objs.triangles.push_back(Triangle(normalize(n), col, a, b, c));

	point a2(-70, 0, 80);
	point b2(70, 0, 80);
	point c2(0, 40, 150);

	torches.push_back(torch(point(0, 0, 0), 1000000));
	vect n2 = calc_normal(a2, b2, c2);
	color col2(255, 255, 0);
	objs.triangles.push_back(Triangle(normalize(n2), col2, a2, b2, c2));

	point center(0, 0, 100);
	point location(0, 0, 0);
	scr = screen(center - location, center, vect(1, 0, 0), 500, 500);
	return location;
}


