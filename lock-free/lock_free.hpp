#include <cstdlib>
#include <cstdio>
#include <thread>
#include <memory>
#include <atomic>


template<typename T>
struct node
{
	std::shared_ptr<T> data;
	node* next;
	node(T const& data_) :
		data(std::make_shared<T>(data_))
	{}
	node(){data = nullptr}
	~node(){}
};



template<typename T>
class lock_free_stack{
	typedef node<T> Node;
private:
	std::atomic<node<T>*> head;
public:
	lock_free_stack() {
		head = nullptr;
	}
	void push(T const& data){
		  Node* const new_node = new Node(data);
		  new_node->next = head.load();
		  while (!head.compare_exchange_weak(new_node->next, new_node));
}
	std::shared_ptr<T> pop(){
		Node* old_head = head.load();
		while (old_head &&
			!head.compare_exchange_weak(old_head, old_head->next));
		return old_head ? old_head->data : std::shared_ptr<T>();
	}
};

