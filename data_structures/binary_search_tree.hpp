template<class T>
class Node {
public:
	int key;
	T data;
	Node* right;
	Node* left;

	Node() :right(this), left(this) {};
	Node(const T val, int keyVal) : data(val), key(keyVal), right(this), left(right) {};
	Node(const Node<T>&);
	~Node() { right = nullptr; left = nullptr; }
};

template<class T>
Node<T>::Node(const Node<T>& copyNode) {
	this->key = copyNode->key;
	this->data = copyNode->data;
	this->right = copyNode->right;
	this->left = copyNode->left;
}

template<class T>
class BinSearchTree {
private:
	Node<T>* root;

	void insert(Node<T>*, int, T);
	Node<T>* find(Node<T>*, int) const;
	void remove(Node<T>*, int);

public:
	BinSearchTree() : root(nullptr) {};
	~BinSearchTree() { this->purge(root); };

	void insert(int, T);
	Node<T>* find(int) const;
	void remove(int);

	bool isEmpty() const { return (this->root == nullptr) ? true : false; };
	Node<T>* getMax(Node<T>*) const;
	Node<T>* getMin(Node<T>*) const;

	void purge(Node<T>*);

};

template<class T>
void BinSearchTree<T>::insert(int key, T data) {
	insert(this->root, key, data);
}

template<class T>
void BinSearchTree<T>::insert(Node<T>* leaf, int key, T data) {
	if (leaf == nullptr) {
		leaf = new Node<T>;
		leaf->key = key;
		leaf->data = data;
		leaf->right = nullptr;
		leaf->left = nullptr;
	}
	else if (key > leaf->key)
		insert(leaf->right, key, data);
	else if (key < leaf->key)
		insert(leaf->left, key, data);
}

template<class T>
Node<T>* BinSearchTree<T>::find(int key) const {
	return find(this->root, key);
}

template<class T>
Node<T>* BinSearchTree<T>::find(Node<T>* leaf, int key) const {
	if (leaf == nullptr)
		return nullptr;
	else if (key == leaf->key)
		return leaf;
	else if (key < leaf->key)
		return find(leaf->left, key);
	else if (key > leaf->key)
		return find(leaf->right, key);
}

template<class T>
Node<T>* BinSearchTree<T>::getMax(Node<T>* leaf) const {
	if (leaf->right == nullptr)
		return leaf;
	else
		return getMax(leaf->right);
}

template<class T>
Node<T>* BinSearchTree<T>::getMin(Node<T>* leaf) const {
	if (leaf->left == nullptr)
		return leaf;
	else
		return getMin(leaf->left);
}

template<class T>
void BinSearchTree<T>::remove(int key) {
	if(!this->isEmpty())
		this->remove(this->root, key);
}

template<class T>
void BinSearchTree<T>::remove(Node<T>* leaf, int key) {
	//Case 0: tree is empty
	if (leaf != nullptr) {
		//Case 1: found node to be removed
		if (key == leaf->key) {
			//Case 1.1: leaf node
			if (leaf->right == nullptr && leaf->left == nullptr)
				delete leaf;
			//Case 1.2: branch node with 1 leaf node
			else if (leaf->right == nullptr) {
				Node<T>* aux = leaf;
				leaf = leaf->left;
				delete aux;
			}
			else if (leaf->left == nullptr) {
				Node<T>* aux = leaf;
				leaf = leaf->right;
				delete aux;
			}
			//Case 1.3: branch node with 2 leaf nodes
			else {
				Node<T>* aux = getMax(leaf->left);
				leaf->key = aux->key;
				leaf->data = aux->data;
				this->remove(aux, aux->key);
			}
		}
		//Case 2: the key value we're looking for is smaller than leaf's key value, go left
		else if (key < leaf->key)
			this->remove(leaf->left, key);
		//Case 3: the key value we're looking for is greater than leaf's key value, go right
		else if (key > leaf->key)
			this->remove(leaf->right, key);
	}
}

template<class T>
void BinSearchTree<T>::purge(Node<T>* leaf) {
	if(!this->isEmpty()){
		if (leaf->left != nullptr)
			purge(leaf->left);
		if (leaf->right != nullptr)
			purge(leaf->right);
		delete leaf;
	}
}
