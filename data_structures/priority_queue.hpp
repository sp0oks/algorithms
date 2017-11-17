template<class T>
class Node {
public:
	T value;
	Node* next;
	Node* previous;

	Node() :next(this), previous(this) {};
	Node(const T val) : value(val), next(this), previous(next) {};
	~Node() { next = nullptr; previous = nullptr; }
};

template<class T>
class PriorityQueue{
private:
	Node<T> header;
	int size;

public:
	PriorityQueue() :size(0) {};
	~PriorityQueue() { this->clear(); };

	void enque(const T);
	bool deque(T&);

	bool isEmpty() const { bool check = (this->header.next == &this->header) ? true : false; return check; };
	int getSize() const { return this->size; };
	Node<T>* getFront() const { return this->header.next; };
	Node<T>* getBack() const { return this->header.previous; };

	void clear();

};

template<class T>
void PriorityQueue<T>::enque(const T element) {
	Node<T>* newNode = new Node<T>;
	if(this->isEmpty()){
		newNode->value = element;
		newNode->next = &this->header;
		newNode->previous = this->header.previous;
		this->header.previous->next = newNode;
		this->header.previous = newNode;
		size++;
	}
	else{
		Node<T>* iter = this->header.next;
		if (element > iter->value){
			newNode->value = element;
			newNode->next = iter;
			newNode->previous = this->header;
			iter->previous = newNode;
			size++;
		}
		else{
			while((iter->value > element) && (iter->next != this->header)) iter = iter->next;
			newNode->value = element;
			newNode->next = iter->next;
			newNode->previous = iter;
			iter->next = newNode;
			size++;
		}
	}
}


template<class T>
bool PriorityQueue<T>::deque(T& element) {
	if (!this->isEmpty()) {
		element = this->header.next->value;
		Node<T>* aux = this->header.next;
		this->header.next = aux->next;
		aux->next->previous = &this->header;
		delete aux;
		size--;
		return true;
	}
	return false;
}

template<class T>
void PriorityQueue<T>::clear() {
	T temp;
	while (this->deque(temp));
}

