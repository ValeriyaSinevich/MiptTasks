#include "View.h"


//kd_tree_node::kd_tree_node(Object * obj) {
//	
//	obj->calc_bounding_box();
//	bb = obj->bb;
//
//	r = nullptr;
//	l = nullptr;
//}

bounding_box merge_bounding_boxes(bounding_box a, bounding_box b) {
	double left_x = std::min(a.left.x, b.left.x);
	double left_y = std::min(a.left.y, b.left.y);
	double left_z = std::min(a.left.z, b.left.z);

	double right_x = std::max(a.right.x, b.right.x);
	double right_y = std::max(a.right.y, b.right.y);
	double right_z = std::max(a.right.z, b.right.z);

	return bounding_box(point(left_x, left_y, left_z), point(right_x, right_y, right_z));
}


bool xComp(Object* const & a, Object* const & b) {
	if (a->x_0.x == b->x_0.x) {
		if (a->x_0.y == b->x_0.y)
			return a->x_0.z < b->x_0.z;
		else
			return a->x_0.y < b->x_0.y;
	}
	else
		return a->x_0.x < b->x_0.x;
}

bool yComp(Object* const & a, Object* const & b) {
	if (a->x_0.y == b->x_0.y) {
		if (a->x_0.z == b->x_0.z)
			return a->x_0.x < b->x_0.x;
		else
			return a->x_0.z < b->x_0.z;
	}
	else
		return a->x_0.y < b->x_0.y;
}

bool zComp(Object* const & a, Object* const & b) {
	if (a->x_0.z == b->x_0.z) {
		if (a->x_0.x == b->x_0.x)
			return a->x_0.y < b->x_0.y;
		else
			return a->x_0.x < b->x_0.x;
	}
	else
		return a->x_0.z < b->x_0.z;
}


kd_tree_node::kd_tree_node(vector<Object*> & objects, int start, int end, const int _axis) : axis(_axis) {
	r = nullptr;
	l = nullptr;
	size = 1;

	if (end - start == 1) {
		pivot_obj = objects[start];
		pivot_obj->calc_bounding_box();
		bb = pivot_obj->bb;
		//r = new kd_tree_node(pivot_obj, axis);
	}
	else {


		int midsize = (end - start + 1) / 2;

		switch (axis) {
		case 0:
			std::nth_element(objects.begin() + start, objects.begin() + start + midsize, objects.begin() + end, xComp);
			break;
		case 1:
			std::nth_element(objects.begin() + start, objects.begin() + start + midsize, objects.begin() + end, yComp);
			break;
		case 2:
			std::nth_element(objects.begin() + start, objects.begin() + start + midsize, objects.begin() + end, zComp);
			break;
		}
		pivot_obj = *(objects.begin() + midsize);

		//vector<Object*> array_less;
		//vector<Object*> array_gr;

		//reorder_array(objects, pivot_obj, axis, array_less, array_gr);
		if (midsize > 0)
			l = new kd_tree_node(objects, start, start + midsize, (axis + 1) % 3);
		if (end - midsize - start > 0)
			r = new kd_tree_node(objects, start + midsize, end, (axis + 1) % 3);


		if (l == nullptr) {
			if (r == nullptr) {
				bb = pivot_obj->bb;
				pivot_obj->calc_bounding_box();
			}
			else
				bb = r->bb;
		}
		else
			bb = merge_bounding_boxes(l->bb, r->bb);

		size = (r == nullptr ? 0 : r->size) + (l == nullptr ? 0 : l->size);
	}
}

void kd_tree_node::reorder_array(vector<Object*> & objects,
	Object *pivot_object,
	int axis,
	vector<Object*> & result_array_less,
	vector<Object*> & result_array_gr) {

	for (vector<Object*>::iterator i = objects.begin(); i != objects.end(); ++i) {
		switch (axis) {
		case 0:
			if ((*(*i)).x_0.x < (*pivot_object).x_0.x) 
				result_array_less.push_back(*i);
			else 
				result_array_less.push_back(*i);		
			break;
		case 1:
			if ((*(*i)).x_0.y < (*pivot_object).x_0.y)
				result_array_less.push_back(*i);
			else
				result_array_less.push_back(*i);
			break;
		case 2:
			if ((*(*i)).x_0.z < (*pivot_object).x_0.z)
				result_array_less.push_back(*i);
			else
				result_array_less.push_back(*i);
			break;
		}
	}
}


kd_tree::kd_tree(vector<Object*> & objects) {
	root = new kd_tree_node(objects, 0, objects.size(), 0);
}



kd_tree::~kd_tree() {
	delete root;
}


kd_tree_node::~kd_tree_node() {
	//delete pivot_obj;

	if (r != nullptr)
		delete r;
	if (l != nullptr)
		delete l;
}

