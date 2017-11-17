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
class Stack {
private:
	Node<T> header;
	int size;

public:
	Stack() :size(0) {};
	~Stack() { this->clear(); };

	void push(const T);
	bool pop(T&);

	bool isEmpty() const { return (this->header.next == &this->header) ? true : false; };
	int getSize() const { return this->size; };
	Node<T>* peek() const { return this->header.next; };

	void clear();

};

template<class T>
void Stack<T>::push(const T element) {
	Node<T>* newNode = new Node<T>;
	newNode->value = element;
	newNode->next = &this->header;
	newNode->previous = this->header.previous;
	this->header.previous->next = newNode;
	this->header.previous = newNode;
	size++;
}

template<class T>
bool Stack<T>::pop(T& element) {
	if (!this->isEmpty()) {
		element = this->header.previous->value;
		Node<T> *aux = this->header.previous;
		this->header.previous = aux->previous;
		aux->previous = &this->header;
		delete(aux);
		size--;
		return true;
	}
	return false;
}

template<class T>
inline void Stack<T>::clear() {
	T temp;
	while (this->pop(temp));
}
