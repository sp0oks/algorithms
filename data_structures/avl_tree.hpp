#include <iostream>
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
class AVLTree {
private:
	Node<T>* root;

	void rebalance_ins(Node<T>*);
	void rebalance_rem(Node<T>*);

	void rotateLL(Node<T>*);
	void rotateRR(Node<T>*);
	void rotateRL(Node<T>*);
	void rotateLR(Node<T>*);

	Node<T>* find(Node<T>*, int) const;
	void insert(Node<T>*, int, T, bool&);
	void remove(Node<T>*, int,bool&);

public:
	AVLTree() : root(nullptr) {};
	~AVLTree() { this->purge(root); };

	int getHeight(Node<T>*) const;
	int getBF(Node<T>* subtree) const { return getHeight(subtree->right) - getHeight(subtree->left); }

	Node<T>* getParentNode(Node<T>*, int) const;
	Node<T>* getMax(Node<T>*) const;
	Node<T>* getMin(Node<T>*) const;

	bool isBalanced(Node<T>*) const;
	bool isEmpty() const { return (this->root == nullptr) ? true : false; };

	Node<T>* find(int) const;
	void insert(int, T, bool&);
	void remove(int, bool&);

	void purge(Node<T>*);

	void preorder(Node<T>*);

};

template<class T>
int AVLTree<T>::getHeight(Node<T>* subtree) const {
	// Case 1: empty subtree
	if (subtree == nullptr)
		return 0;
	//Case 2: height equals to 1 + greatest value between left and right branches
	int maxHeight = (getHeight(subtree->left) >= getHeight(subtree->right)) ? getHeight(subtree->left) : getHeight(subtree->right);
	return 1 + maxHeight;
}

template<class T>
Node<T>* AVLTree<T>::getParentNode(Node<T>* leaf, int childkey) const{
	//Searches for leaf node's parent, considering the leaf node is already in the tree

	if (childkey != this->root->key) {
		if (childkey == leaf->left->key || childkey == leaf->right->key)
			return leaf;
		else{
			if (childkey > leaf->key)
				return getParentNode(leaf->right, childkey);

			else if (childkey < leaf->key)
				return getParentNode(leaf->left, childkey);
		}
	}
	return nullptr;
}

template<class T>
Node<T>* AVLTree<T>::getMax(Node<T>* leaf) const {
	if (leaf->right == nullptr)
		return leaf;
	else
		return getMax(leaf->right);
}

template<class T>
Node<T>* AVLTree<T>::getMin(Node<T>* leaf) const {
	if (leaf->left == nullptr)
		return leaf;
	else
		return getMin(leaf->left);
}

template<class T>
bool AVLTree<T>::isBalanced(Node<T>* subtree) const{
	if (subtree == nullptr)
		return true;
	else{
		if (getBF(subtree) < -1 || getBF(subtree) > 1)
			return false;
		return isBalanced(subtree->right) && isBalanced(subtree->left);
	}
}

template<class T>
void AVLTree<T>::rotateLL(Node<T>* leaf) {
	Node<T>* child;
	child = leaf->left;
	leaf->left = child->right;
	child->right = leaf;
	leaf = child;
}

template<class T>
void AVLTree<T>::rotateRR(Node<T>* leaf) {
	Node<T>* child;
	child = leaf->right;
	leaf->right = child->left;
	child->left = leaf;
	leaf = child;
}

template<class T>
void AVLTree<T>::rotateLR(Node<T>* leaf) {
	Node<T>* child, grandchild;
	child = leaf->left;
	grandchild = child->right;
	//RotateRR child and grandchild nodes
	child->right = grandchild->left;
	grandchild->left = child;
	//RotateLL root and grandchild nodes
	leaf->left = grandchild->right;
	grandchild->right = leaf;
	//Make grandchild node the new root
	leaf = grandchild;
}

template<class T>
void AVLTree<T>::rotateRL(Node<T>* leaf) {
	Node<T>* child, grandchild;
	child = leaf->right;
	grandchild = child->left;
	//RotateLL child and grandchild nodes
	child->left = grandchild->right;
	grandchild->right = child;
	//RotateRR root and grandchild nodes
	leaf->right = grandchild->left;
	grandchild->left = leaf;
	//Make grandchild node the new root
	leaf = grandchild;
}

template<class T>
void AVLTree<T>::rebalance_ins(Node<T>* leaf) {
	//Proceed to check if the tree has been unbalanced
	if (!isBalanced(this->root)) {
		//If it is, search for the first unbalanced node from the bottom node "leaf"
		Node<T>* BalNode = getParentNode(this->root, leaf->key);
		Node<T>* tempX = leaf;
		Node<T>* tempY;

		//The other 2 temp nodes will be used in the coming rotation cases
		while (isBalanced(BalNode) && getParentNode(this->root, BalNode->key) != nullptr) {
			tempY = tempX;
			tempX = BalNode;
			BalNode = getParentNode(this->root, BalNode->key);
		}
		//Check which of the rotation cases will be needed and rotate the nodes
			//Case 1: Left-Left Rotation
			if (BalNode->left == tempX && tempX->left == tempY)
				this->rotateLL(BalNode);

			//Case 2: Right-Right Rotation
			else if (BalNode->right == tempX && tempX->right == tempY)
				this->rotateRR(BalNode);

			//Case 3: Left-Right Rotation
			else if (BalNode->left == tempX && tempX->right == tempY)
				this->rotateLR(BalNode);

			//Case 4: Right-Left Rotation
			else if (BalNode->right == tempX && tempX->left == tempY)
				this->rotateRL(BalNode);
	}
}

template<class T>
void AVLTree<T>::rebalance_rem(Node<T>* BalNode){
	//Check if the tree is unbalanced
	if (!isBalanced(this->root)) {
		//If it is, search for the first unbalanced node from the parent of the one that was removed
		while (isBalanced(BalNode) && getParentNode(this->root, BalNode->key) != nullptr)
			BalNode = getParentNode(this->root, BalNode->key);
		//Now find another 2 nodes from the tallest subtree
		Node<T>* tempX = (getHeight(BalNode->left) > getHeight(BalNode->right)) ? BalNode->left : BalNode->right;
		Node<T>* tempY = (getHeight(tempX->left) > getHeight(tempX->right)) ? tempX->left : tempX->right;
		//Check which of the rotation cases will be needed and rotate the nodes
			//Case 1: Left-Left Rotation
			if (BalNode->left == tempX && tempX->left == tempY)
				this->rotateLL(BalNode);

			//Case 2: Right-Right Rotation
			else if (BalNode->right == tempX && tempX->right == tempY)
				this->rotateRR(BalNode);

			//Case 3: Left-Right Rotation
			else if (BalNode->left == tempX && tempX->right == tempY)
				this->rotateLR(BalNode);

			//Case 4: Right-Left Rotation
			else if (BalNode->right == tempX && tempX->left == tempY)
				this->rotateRL(BalNode);

	}
}

template<class T>
Node<T>* AVLTree<T>::find(int key) const {
	return find(this->root, key);
}

template<class T>
Node<T>* AVLTree<T>::find(Node<T>* leaf, int key) const {
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
void AVLTree<T>::insert(int key, T data, bool& need_rebalance) {
	insert(this->root, key, data, need_rebalance);
}

template<class T>
void AVLTree<T>::insert(Node<T>* leaf, int key, T data, bool& need_rebalance){
	//Same insertion procedure as the unbalanced version
	if (leaf == nullptr) {
		leaf = new Node<T>;
		leaf->key = key;
		leaf->data = data;
		leaf->right = nullptr;
		leaf->left = nullptr;
		need_rebalance = true;
	}
	else if (key == leaf->key)
		need_rebalance = false;
	else if (key > leaf->key)
		insert(leaf->right, key, data, need_rebalance);
	else if (key < leaf->key)
		insert(leaf->left, key, data, need_rebalance);

	//If insertion is successful, check if tree needs balancing
	if (need_rebalance)
		this->rebalance_ins(leaf);
}

template<class T>
void AVLTree<T>::remove(int key, bool& need_rebalance) {
	need_rebalance = false;
	if(!this->isEmpty())
		this->remove(this->root, key, need_rebalance);
}

template<class T>
void AVLTree<T>::remove(Node<T>* leaf, int key,bool& need_rebalance) {
	Node<T>* pNode;
	//Case 0: tree is empty
	if (leaf != nullptr) {
		//Case 1: found node to be removed
		if (key == leaf->key) {
			//Case 1.1: leaf node
			if (leaf->right == nullptr && leaf->left == nullptr) {
				pNode = getParentNode(this->root, leaf->key);
				delete leaf;
				need_rebalance = true;
			}
			//Case 1.2: branch node with 1 leaf node
			else if (leaf->right == nullptr) {
				Node<T>* aux = leaf;
				leaf = leaf->left;
				pNode = getParentNode(this->root, aux->key);
				delete aux;
				need_rebalance = true;
			}
			else if (leaf->left == nullptr) {
				Node<T>* aux = leaf;
				leaf = leaf->right;
				pNode = getParentNode(this->root, aux->key);
				delete aux;
				need_rebalance = true;
			}
			//Case 1.3: branch node with 2 leaf nodes
			else {
				Node<T>* aux = getMax(leaf->left);
				leaf->key = aux->key;
				leaf->data = aux->data;
				this->remove(aux, aux->key, need_rebalance);
			}
		}
		//Case 2: the key value we're looking for is smaller than leaf's key value, go left
		else if (key < leaf->key)
			this->remove(leaf->left, key, need_rebalance);
		//Case 3: the key value we're looking for is greater than leaf's key value, go right
		else if (key > leaf->key)
			this->remove(leaf->right, key, need_rebalance);
	}
	//After removal, rebalance the tree if needed using the removed node's parent reference
	if (need_rebalance)
		if (!this->isEmpty() && pNode != nullptr){
			while (!isBalanced(this->root)){
				this->rebalance_rem(pNode);
				pNode = getParentNode(pNode);
			}
		}
}

template<class T>
void AVLTree<T>::purge(Node<T>* leaf) {
	if (!this->isEmpty()) {
		if (leaf->left != nullptr)
			purge(leaf->left);

		if (leaf->right != nullptr)
			purge(leaf->right);

		delete leaf;
	}
}

template<class T>
void AVLTree<T>::preorder(Node<T>* leaf) {
	if (leaf != nullptr) {
		preorder(leaf->left);
		preorder(leaf->right);
		std::cout << leaf->key;
	}
}

