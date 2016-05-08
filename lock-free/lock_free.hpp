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

template <typename T>
class memory;

unsigned const max_hazard_pointers = 100;


struct hazard_pointer
{
	std::atomic<std::thread::id> id;
	std::atomic<void*> pointer;
};

hazard_pointer hazard_pointers[max_hazard_pointers];

class hp_owner
{
	hazard_pointer* hp;

public:
	hp_owner(hp_owner const&) = delete;

	hp_owner operator=(hp_owner const&) = delete;

	hp_owner() : hp(nullptr) {
		for (unsigned i = 0; i < max_hazard_pointers; ++i) {
			std::thread::id old_id;
			if (hazard_pointers[i].id.compare_exchange_strong(
				old_id, std::this_thread::get_id())) {
				hp = &hazard_pointers[i];
				break;
			}
		}
		if (!hp) {
			throw std::runtime_error("No hazard pointers available");
		}
	}
	std::atomic<void*>& get_pointer(){
		return hp->pointer;
	}
	~hp_owner() {
		hp->pointer.store(nullptr);
		hp->id.store(std::thread::id());
	}
};

std::atomic<void*>& get_hazard_pointer_for_current_thread()
{
	thread_local static hp_owner hazard;
	return hazard.get_pointer();
}

template<typename T>
struct node;

bool outstanding_hazard_pointers_for(void* p) {
	for (unsigned i = 0; i < max_hazard_pointers; ++i) {
		if (hazard_pointers[i].pointer.load() == p) {
			return true;
		}
	}
	return false;
}


template<typename T>
void do_delete(void* p)
{
	delete static_cast<T*>(p);
}
struct data_to_reclaim
{
	void* data;
	std::function<void(void*)> deleter;
	data_to_reclaim* next;
	template<typename T>
	data_to_reclaim(T* p) :
		data(p),
		deleter(&do_delete<T>),
		next(0) {}
	~data_to_reclaim() {
		deleter(data);
	}
};

std::atomic<data_to_reclaim*> nodes_to_reclaim;

void add_to_reclaim_list(data_to_reclaim* node) {
	node->next = nodes_to_reclaim.load();
	while (!nodes_to_reclaim.compare_exchange_weak(node->next, node));
}

template<typename T>
void reclaim_later(T* data) {
	add_to_reclaim_list(new data_to_reclaim(data));
}

void delete_nodes_with_no_hazards() {
	data_to_reclaim* current = nodes_to_reclaim.exchange(nullptr);
	while (current) {
		data_to_reclaim* const next = current->next;
		if (!outstanding_hazard_pointers_for(current->data)) {
			delete current;
		}
		else {
			add_to_reclaim_list(current);
		}
		current = next;
	}
}

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
	node(T const& data_) : data(std::make_shared<T>(data_)) {}
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
		Node* const new_node = new Node(data);
		if (new_node == nullptr) {
			std::cout << "no memory" << std::endl;
		}
		new_node->data = std::make_shared<T>(data);
		new_node->next = head.load();
		while (!head.compare_exchange_weak(new_node->next, new_node));
}
	void pop(){
		std::atomic<void*>& hp = get_hazard_pointer_for_current_thread();
		Node* old_head = head.load();
		do {
			node* temp;
			do {
				temp = old_head;
				hp.store(old_head);
				old_head = head.load();
			} while (old_head != temp);
		} while (old_head &&
			!head.compare_exchange_strong(old_head, old_head->next));

		hp.store(nullptr);
		std::shared_ptr<T> res;
		if (old_head) {
			res.swap(old_head->data);
			if (outstanding_hazard_pointers_for(old_head)) {
				reclaim_later(old_head);
			}
			else {
				delete old_head;
			}
			delete_nodes_with_no_hazards();
		}
		return res;
	}
};

