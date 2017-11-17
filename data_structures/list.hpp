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
class List {
private:
	Node<T> header;
	Node<T>* current;
	int size;

public:
	List() : current(&this->header), size(0) {};
	~List() { this->clear(); }

	bool toFirst(); // retorna verdadeiro se a lista estiver vazia.
	bool toNext(); // retorna verdadeiro se passar para o último elemento da lista.

	bool isEmpty() const { return (this->header.next == &this->header) ? true : false; };

	void insert(T);
	bool find(T);
	bool remove(T);
	void getCurrent(T& element) { element = this->current->value; };
	void removeCurrent();
	int getSize() const { return this->size; };

	void clear();

	T operator[](int) const;

};

template<class T>
bool List<T>::toFirst() {
	this->current = this->header.next;
	return this->current == &this->header;
}

template<class T>
bool List<T>::toNext() {
	if (!this->isEmpty()) {
		this->current = this->current->next;
		if (this->current == &this->header)
			this->current = this->current->next;
	}
	return this->current->next == &this->header;
}

template<class T>
void List<T>::insert(T element) {
	Node<T>* newNode = new Node<T>;
	newNode->value = element;
	newNode->next = this->current->next;
	newNode->previous = this->current;
	newNode->next->previous = newNode;
	this->current->next = newNode;
	this->toNext();
	this->size++;
}

template<class T>
void List<T>::removeCurrent() {
	if (!this->isEmpty()) {
		Node<T>* aux = this->current;
		aux->previous->next = aux->next;
		aux->next->previous = aux->previous;
		this->current = aux->previous;
		delete aux;
		this->size--;
	}
}

template<class T>
bool List<T>::find(T element) {
	this->toFirst();
	while (this->current->value != element) this->toNext();
	if (this->current->value == element)
		return true;
	return false;
}

template<class T>
bool List<T>::remove(T element) {
  Node<T>* temp = this->current;
		if(this->find(element)){
			this->removeCurrent();
			this->current = temp;
			return true;
		}
	return false;
}

template<class T>
void List<T>::clear() {
	this->toFirst();
	while (!this->toNext())
		this->removeCurrent();
}

template<class T>
T List<T>::operator[](int index) const {
	this->toFirst();
	while (index > 0 && !this->toNext())
		index--;
	return this->current.value;
}


