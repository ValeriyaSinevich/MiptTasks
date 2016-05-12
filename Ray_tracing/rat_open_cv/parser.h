#ifndef DRAWER_H
#define DRAWER_H
#include <cmath>
#include "View.h"


class Parser
{
private:


public:
	Parser();
	static point parse_input(screen & scr, kd_tree * &objects_container, vector<torch> & torches, vector<Object*> & objs);
	static point parse_stl_input(screen & scr, kd_tree *objects_container,
		vector<torch> & torches, std::string file, vector<Object*> & objs);
	static void read_stl_triangles(vector<Object*> objects, std::string file);
};

#endif 