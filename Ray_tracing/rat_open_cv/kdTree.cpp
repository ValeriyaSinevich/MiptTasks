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


kd_tree_node::kd_tree_node(vector<Object*> & objects, vector <vector<int> > & indices,
	const int _axis) : axis(_axis) {

	r = nullptr;
	l = nullptr;
	size = 1;

	if (indices[axis].size() == 1) {
		pivot_obj = objects[indices[axis][0]];
		pivot_obj->calc_bounding_box();
		bb = pivot_obj->bb;
		//r = new kd_tree_node(pivot_obj);
	}
	else {
		int midsize = (indices[axis].size() + 1) / 2;
		int mediana = indices[axis][midsize];
		Object* pivot_obj = objects[mediana];
		vector<int> res1_less;
		vector<int> res1_gr;
		vector<int> res2_less;
		vector<int> res2_gr;
		reorder_array(objects, pivot_obj, axis, indices[(axis + 1) % 3], res1_less, res1_gr);
		reorder_array(objects, pivot_obj, axis, indices[(axis + 2) % 3], res2_less, res2_gr);

		vector <vector<int> > new_indices_l(3);
		vector <vector<int> > new_indices_r(3);

		for (auto it = indices[axis].begin(); it != indices[axis].begin() + midsize; ++it) {
			new_indices_l[axis].push_back(*it);
		}
		//std::copy(indices[axis].begin(), indices[axis].begin() + mediana, new_indices_l[axis].begin());
		new_indices_l[(axis + 1) % 3] = res1_less;;
		new_indices_l[(axis + 2) % 3] = res2_less;

		//std::copy(indices[axis].begin() + mediana, indices[axis].end(), new_indices_l[axis].begin());
		for (auto it = indices[axis].begin() + midsize; it != indices[axis].end(); ++it) {
			new_indices_r[axis].push_back(*it);
		}
		new_indices_r[(axis + 1) % 3] = res1_gr;
		new_indices_r[(axis + 2) % 3] = res2_gr;

		indices.clear();

		pivot_obj->calc_bounding_box();
		if (new_indices_l.size() != 0)
			l = new kd_tree_node(objects, new_indices_l, (axis + 1) % 3);
		if (new_indices_l.size() != 0)
			r = new kd_tree_node(objects, new_indices_r, (axis + 1) % 3);
		bb = merge_bounding_boxes(l->bb, r->bb);
		size = (r == nullptr ? 0 : r->size) + (l == nullptr ? 0 : l->size);
	}
}


struct xyzComparator : public std::binary_function<point, point, bool> {
public:
	bool operator()(point const & a, point const & b) const {
		if (a.x == b.x) {
			if (a.y == b.y)
				return a.z < b.z;
			else
				return a.y < b.y;
		}
		else
			return a.x < b.x;
	}
};

struct yzxComparator : public std::binary_function<point, point, bool> {
public:
	bool operator()(point const & a, point const & b) const {
		if (a.y == b.y) {
			if (a.z == b.z)
				return a.x < b.x;
			else
				return a.z < b.z;
		}
		else
			return a.y < b.y;
	}
};

struct zxyComparator : public std::binary_function<point, point, bool> {
public:
	bool operator()(point const & a, point const & b) const {
		if (a.z == b.z) {
			if (a.x == b.x)
				return a.y < b.y;
			else
				return a.x < b.x;
		}
		else
			return a.z < b.z;
	}
};

void kd_tree_node::reorder_array(vector<Object*> & objects,
	Object *mediana,
	int axis,
	vector<int> & array_to_sort,
	vector<int> & result_array_less,
	vector<int> & result_array_gr) {
	if (axis == 0) {
		for (unsigned int i = 0; i < array_to_sort.size(); ++i) {
			Object* obj = objects[array_to_sort[i]];
			if (obj->x_0.x < mediana->x_0.x)
				result_array_less.push_back(i);
			else 
				result_array_gr.push_back(i);
		}
	}
	if (axis == 1) {
		for (unsigned int i = 0; i < array_to_sort.size(); ++i) {
			Object* obj = objects[array_to_sort[i]];
			if (obj->x_0.y < mediana->x_0.y)
				result_array_less.push_back(i);
			else
				result_array_gr.push_back(i);
		}
	}
	if (axis == 2) {
		for (unsigned int i = 0; i < array_to_sort.size(); ++i) {
			Object* obj = objects[array_to_sort[i]];
			if (obj->x_0.z < mediana->x_0.z)
				result_array_less.push_back(i);
			else
				result_array_gr.push_back(i);
		}
	}
}


kd_tree::kd_tree(vector<Object*> & objects) {

	vector<vector<int> > indices(3);

	std::map<point, int, xyzComparator> xyz;
	std::map<point, int, yzxComparator> yzx;
	std::map<point, int, zxyComparator> zxy;

	for (unsigned int i = 0; i < objects.size(); ++i) {
		xyz.insert(std::make_pair(objects[i]->x_0, i));
		yzx.insert(std::make_pair(objects[i]->x_0, i));
		zxy.insert(std::make_pair(objects[i]->x_0, i));
	}
	std::map<point, int>::iterator xyz_iterator = xyz.begin();
	std::map<point, int>::iterator yzx_iterator = yzx.begin();
	std::map<point, int>::iterator zxy_iterator = zxy.begin();

	for (std::map<point, int>::iterator i = xyz.begin(); i != xyz.end(); i++) {
		indices[0].push_back(i->second);
	}
	for (std::map<point, int>::iterator i = yzx.begin(); i != yzx.end(); i++) {
		indices[1].push_back(i->second);
	}
	for (std::map<point, int>::iterator i = zxy.begin(); i != zxy.end(); i++) {
		indices[2].push_back(i->second);
	}
	root = new kd_tree_node(objects, indices, 0);
}



void Triangle::calc_bounding_box() {
	double x = std::min(fst.x, std::min(snd.x, thrd.x));
	double y = std::min(fst.z, std::min(snd.z, thrd.z));
	double z = std::min(fst.y, std::min(snd.y, thrd.y));
	bb.left = point(x, y, z);

	x = std::max(fst.x, std::max(snd.x, thrd.x));
	y = std::max(fst.z, std::max(snd.z, thrd.z));
	z = std::max(fst.y, std::max(snd.y, thrd.y));
	bb.right = point(x, y, z);
}


void Rechtangle::calc_bounding_box() {
	double x = std::min(fst.x, std::min(snd.x, std::min(thrd.x, fourth.x)));
	double y = std::min(fst.z, std::min(snd.z, std::min(thrd.y, fourth.y)));
	double z = std::min(fst.y, std::min(snd.y, std::min(thrd.z, fourth.z)));
	bb.left = point(x, y, z);

	x = std::max(fst.x, std::max(snd.x, thrd.x));
	y = std::max(fst.z, std::max(snd.z, thrd.z));
	z = std::max(fst.y, std::max(snd.y, thrd.y));
	bb.right = point(x, y, z);
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

