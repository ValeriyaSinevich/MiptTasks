#ifndef KD_TREE
#define KD_TREE

#include "utilities.h"
#include "shapes.h"

struct kd_tree_node {
	bounding_box bb;
	Object *pivot_obj;

	int axis;
	double value;
	int size;

	kd_tree_node* l;
	kd_tree_node* r;
	~kd_tree_node();
	//kd_tree_node(Object * obj);

	kd_tree_node(vector<Object*> & objects, vector <vector<int> > & indices,
		const int _axis);

	//kd_tree_node(Object * obj, double value, int axis);
	void reorder_array(vector<Object*> & objects,
		Object *mediana,
		int axis,
		vector<int> & array_to_sort,
		vector<int> & result_array_less,
		vector<int> & result_array_gr);
};


class kd_tree {
public:
	kd_tree_node* root;
	kd_tree() {};
	~kd_tree();
	kd_tree(vector<Object*> & objects);
};
#endif