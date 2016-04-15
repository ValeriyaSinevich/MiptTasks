#ifndef DRAWER_H
#define DRAWER_H
#include <cmath>
#include "View.h"


class Parser
{
private:


public:
	Parser();
	static point parse_input(screen & scr, objects & objs, vector<torch> & torches);
};

#endif 