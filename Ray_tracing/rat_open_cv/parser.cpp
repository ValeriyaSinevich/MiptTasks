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
					color(255, 255, 255),
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

	point a(-50, 0, 100);
	point b(50, 0, 100);
	point c(0, 100, 100);
	vect n = calc_normal(a, b, c);
	color col(255, 0, 0);
	Object *newObject1 = new Triangle(normalize(n), col, a, b, c);
	objs.push_back(newObject1);

	point a2(-70, 0, 80);
	point b2(70, 0, 80);
	point c2(0, 40, 150);
	vect n2 = calc_normal(a2, b2, c2);
	color col2(255, 255, 0);

	Object *newObject2 = new Triangle(normalize(n2), col2, a2, b2, c2);
	objs.push_back(newObject2);

	objects_container = new kd_tree(objs);

	torches.push_back(torch(point(0, 0, 0), 1000000));

	point center(0, 0, 100);
	point location(0, 0, 0);
	scr = screen(center - location, center, vect(1, 0, 0), 500, 500);
	return location;
}


