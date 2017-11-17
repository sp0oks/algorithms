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
class Queue {
private:
	Node<T> header;
	int size;

public:
	Queue() :size(0) {};
	~Queue() { this->clear(); };

	void enque(const T);
	bool deque(T&);

	bool isEmpty() const { bool check = (this->header.next == &this->header) ? true : false; return check; };
	int getSize() const { return this->size; };
	Node<T>* getFront() const { return this->header.next; };
	Node<T>* getBack() const { return this->header.previous; };

	void clear();

};

template<class T>
void Queue<T>::enque(const T element) {
	Node<T> *newNode = new Node<T>;
	newNode->value = element;
	newNode->next = &this->header;
	newNode->previous = this->header.previous;
	this->header.previous->next = newNode;
	this->header.previous = newNode;
	size++;
}


template<class T>
bool Queue<T>::deque(T& element) {
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
void Queue<T>::clear() {
	T temp;
	while (this->deque(temp));
}
