#include<set>
#include <cstdlib>
#include <mutex>
#include <cstdio>
#include <thread>
#include <memory>
#include <vector>
#include <atomic>
#include <cstdio>
#include <iostream>

template<typename T>
struct node;

template <typename T>
class memory {
	std::mutex mtx;
	std::set<node<T>*> mem;
	std::vector<node<T>> stored_mem;
	typename std::set<node<T>*>::const_iterator ptr;
public:
	memory() {	
		stored_mem.resize(100000);
		for (unsigned int i = 0; i < 100000; ++i) {
			mem.insert(&stored_mem[i]);
		}
		ptr = mem.begin();
	}

	node<T>* node_malloc() {
		std::lock_guard<std::mutex> lock(mtx);
		if (ptr != mem.end()) {
			return *ptr;
		}
		else {
			return nullptr;
		}
	}

	void node_free(node<T>* a) {
		std::lock_guard<std::mutex> lock(mtx);
		mem.insert(a);
	}
};


template<typename T>
struct node
{
	std::shared_ptr<T> data;
	node* next;
	node(T const& data_) :
		data(std::make_shared<T>(data_))
	{}
	node(){ data = nullptr; }
	~node(){}
};



template<typename T>
class lock_free_stack{
	typedef node<T> Node;
private:
	std::atomic<node<T>*> head;
	memory<T> mem;
public:
	lock_free_stack() {
		head = nullptr;
	}
	void push(T const & data){
		Node* const new_node = mem.node_malloc();
		if (new_node == nullptr) {
			std::cout << "no memory" << std::endl;
		}
		new_node->data = std::make_shared<T>(data);
		new_node->next = head.load();
		while (!head.compare_exchange_weak(new_node->next, new_node));
}
	void pop(){
		Node* old_head = head.load();
		while (old_head &&
			!head.compare_exchange_weak(old_head, old_head->next));
		if (old_head)
			mem.node_free(old_head);
	}
};

