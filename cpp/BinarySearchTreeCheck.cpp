// Check if a binary tree is a binary search tree

#include <iostream>
#include <climits>

template <typename T>
class BinaryTree {

public:
	BinaryTree(T const& val, BinaryTree *left = NULL, BinaryTree *right = NULL)
		: mVal(val), mLeft(left), mRight(right) {
		if (contains(left, right)) {
			throw std::runtime_error("Left is in right subtree");
		}

		if (contains(right, left)) {
			throw std::runtime_error("Left is in right subtree");
		}
	}

	void setVal(T const& val) {
		mVal = val;
	}

	bool setLeftChild(BinaryTree *left) {
		if (contains(left, this)) {
			return false;
		}

		mLeft = left;

		return true;
	}

	bool setRightChild(BinaryTree *right) {
		if (contains(right, this)) {
			return false;
		}

		mRight = right;

		return true;
	}

	T const& getVal() const {
		return mVal;
	}

	BinaryTree* getLeft() const {
		return mLeft;
	}

	BinaryTree* getRight() const {
		return mRight;
	}

	~BinaryTree() {
		// std::cout << "Deleting tree node with val: " << mVal << std::endl;

		delete mLeft;
		delete mRight;
	}

private:

	bool contains(BinaryTree *subTree, BinaryTree *root) {
		if (root == NULL) {
			return false;
		}

		if (root == subTree) {
			return true;
		}

		return contains(subTree, root->mLeft) || contains(subTree, root->mRight);
	}

	T mVal;
	BinaryTree *mLeft;
	BinaryTree *mRight;
};

bool isBinarySearchTree(BinaryTree<int> *root, int min, int max) {
	if (root == NULL) {
		return true;
	}

	int const& rootV = root->getVal();

	if (rootV <= min || rootV > max) {
		// std::cout << rootV << " <= " << min << " || " << rootV << " > " << max << std::endl;
		return false;
	}

	return isBinarySearchTree(root->getLeft(), min, rootV)
	    && isBinarySearchTree(root->getRight(), rootV, max);
}

/*
                  20
             /          \
           18            22
       /        \    /        \
      16        19  21        23

*/
int main(int argc, char **argv) {

	BinaryTree<int> *left = new BinaryTree<int>(16);
	BinaryTree<int> *right = new BinaryTree<int>(19);

	BinaryTree<int> *rootLeft = new BinaryTree<int>(18, left, right);

	left = new BinaryTree<int>(21);
	right = new BinaryTree<int>(23);
	BinaryTree<int> *rootRight = new BinaryTree<int>(22, left, right);

	left = rootLeft;
	right = rootRight;
	BinaryTree<int> *root = new BinaryTree<int>(20, left, right);

	std::cout << "Is binary search tree: " << isBinarySearchTree(root, INT_MIN, INT_MAX) << std::endl;

	delete root;

	return 0;
}
