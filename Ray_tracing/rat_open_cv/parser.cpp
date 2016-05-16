#include "parser.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using std::string;

Parser::Parser() {}


vector<string> split(string str, char delimiter) {
	vector<string> internal;
	std::stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (std::getline(ss, tok, delimiter)) {
		if (tok != "")
			internal.push_back(tok);
	}

	return internal;
}

double string_to_double(const std::string& s)
{
	std::istringstream i(s);
	double x;
	if (!(i >> x))
		return 0;
	return x;
}


void Parser::read_stl_triangles(vector<Object*> & objects, std::string file) {
	std::ifstream in;
	in.open(file);

	std::string line;

	std::getline(in, line);

	vector<string> splitted_string = split(line, ' ');

	vector<point> vertices_for_triangle(3);
	int k = 0;

	while (splitted_string.size() == 0 || splitted_string[0] != "endsolid") {
		if (splitted_string.size() == 0) {
			std::getline(in, line);
			splitted_string = split(line, ' ');
			continue;
		}
		if (splitted_string[0] == "vertex") {
			vertices_for_triangle[k] =
				point(string_to_double(splitted_string[1]),
				string_to_double(splitted_string[2]),
				string_to_double(splitted_string[3]));
			if (k == 2) {
				vect n = calc_normal(vertices_for_triangle[0],
					vertices_for_triangle[1],
					vertices_for_triangle[2]);
				Object* newTriangle = new Triangle(normalize(n),
					color(255, 0, 0),
					vertices_for_triangle[0],
					vertices_for_triangle[1],
					vertices_for_triangle[2]);
				objects.push_back(newTriangle);
				k = 0;
			}
			else 
				k++;
		} 
		std::getline(in, line);
		splitted_string = split(line, ' ');
	}
}


point Parser::parse_stl_input(screen & scr, kd_tree * &objects_container,
	vector<torch> & torches, std::string file, vector<Object*> & objects) {
	//vector<Object*> objects;
	read_stl_triangles(objects, file);
	
	objects_container = new kd_tree(objects);

	torches.push_back(torch(point(1000, 0, 0), 1000000000));
	torches.push_back(torch(point(1000, 1000, 1000), 1000000000));

	point center(0, -150, 0);
	point location(0, 15, 2);
	scr = screen(center - location, center, vect(1, 0, 0), 250, 250);
	return location;
}

point Parser::parse_input(screen & scr, kd_tree * &objects_container, vector<torch> & torches, vector<Object*> & objs) {

	//vector<Object*> objs;

	point a(-100, 0, 200);
	point b(100, 0, 200);
	point c(0, 200, 200);
	vect n = calc_normal(a, b, c);
	color col(80, 0, 0);
	Object *newObject1 = new Triangle(normalize(n), col, a, b, c, 0.5);
	objs.push_back(newObject1);

	point a2(-140, 0, 180);
	point b2(140, 0, 180);
	point c2(0, 150, 250);
	vect n2 = calc_normal(a2, b2, c2);
	color col2(100, 0, 130);

	Object *newobject2 = new Triangle(normalize(n2), col2, a2, b2, c2, 0.5);
	objs.push_back(newobject2);

	//point a3(120, 100, 180);
	//point b3(200, 70, 260);
	//point c3(170, -200, 250);
	//vect n3 = calc_normal(a2, b2, c2);
	//color col8(255, 0, 255);

	//Object *newobject3 = new Triangle(normalize(n3), col8, a3, b3, c3, 0.5);
	//objs.push_back(newobject3);

	Object *newObject3 = new Sphere(vect(50, 50, 50), color (0, 70, 150), point(200, 200, 200), 0.2);
	objs.push_back(newObject3);

	Object *newObject4 = new Sphere(vect(50, 50, 50), color(40, 200, 180), point(-200, 200, 200), 0.2);
	objs.push_back(newObject4);


	Object *newObject5 = new Sphere(vect(50, 50, 50), color(40, 0, 130), point(0, -150, 400), 0.2);
	objs.push_back(newObject5);

	Object *newObject6 = new Sphere(vect(50, 50, 50), color(230, 64, 9), point(-150, -150, 400), 0.2);
	objs.push_back(newObject6);

	Object *newObject7 = new Sphere(vect(70, 50, 50), color(9, 255, 180), point(200, -150, 400), 0.2);
	objs.push_back(newObject7);

	Object *newObject8 = new Sphere(vect(0, 10, 0), color(0, 30, 30), point(0, -100, 80), 0.1);
	objs.push_back(newObject8);

	Object *rech1 = new Rechtangle(vect(0, 1, 0), color(0, 255, 0), 0.1,
		point(50, -150, 100), point(50, -150, 150), point(-50, -150, 150), point(-50, -150, 100));
	objs.push_back(rech1);

	//Object *rech2 = new Rechtangle(vect(0, 1, 0), color(100, 100, 100), 0.9,
	//	point(50, -50, 100), point(50, -50, 180), point(-50, -50, 180), point(-50, -50, 100));
	//objs.push_back(rech2);

	Object *newObject9 = new Sphere(vect(0, 10, 0), color(100, 0, 255), point(0, -200, 120), 0.9);
	objs.push_back(newObject9);

	Object *newObject10 = new Sphere(vect(0, 30, 0), color(0, 120, 120), point(-200, -100, 80), 0.1);
	objs.push_back(newObject10);

	Object *newObject11 = new Sphere(vect(0, 50, 0), color(225, 225, 225), point(-100, 190, 400), 0.1);
	objs.push_back(newObject11);

	//Object *newObject12 = new Sphere(vect(0, 20, 0), color(225, 225, 225), point(0, 0, 50), 0.1);
	//objs.push_back(newObject12);

	Object *box1 = new Rechtangle(vect(1, 0, 0), color(140, 140, 0), 0.6,
		point(-250, 250, 100), point(-250, -250, 100), point(-250, -250, 600), point(-250, 250, 600));
	Object *box2 = new Rechtangle(vect(-1, 0, 0), color(140, 140, 0), 0.6,
		point(250, -250, 100), point(250, 250, 100), point(250, 250, 600), point(250, -250, 600));
	Object *box3 = new Rechtangle(vect(0, 1, 0), color(140, 140, 0), 0.6,
		point(-250, -250, 100), point(250, -250, 100), point(250, -250, 600), point(-250, -250, 600));
	Object *box4 = new Rechtangle(vect(0, -1, 0), color(140, 140, 0), 0.6,
		point(-250, 250, 100), point(250, 250, 100), point(250, 250, 600), point(-250, 250, 600));
	Object *box5 = new Rechtangle(vect(0, 0, -1), color(140, 140, 0), 0.6,
		point(-250, 250, 600), point(250, 250, 600), point(250, -250, 600), point(-250, -250, 600));

	objs.push_back(box1);
	objs.push_back(box2);
	objs.push_back(box3);
	objs.push_back(box4);
	objs.push_back(box5);



	objects_container = new kd_tree(objs);

	torches.push_back(torch(point(0, 185, 220), 10000000));
	torches.push_back(torch(point(0, 100, 120), 40000000));
	torches.push_back(torch(point(0, 0, 0), 30000000));


	point center(0, 0, 100);
	point location(0, 0, -200);
	scr = screen(center - location, center, vect(1, 0, 0), 500, 500);
	return location;
}


