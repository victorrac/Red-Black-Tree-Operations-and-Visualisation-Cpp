#ifndef RBNODE_H_
#define RBNODE_H_

#include <string>
#include <iostream>
#include <sstream>

using namespace std;


struct RBNode {
	int key;       // key
	RBNode *p;     // pointer to parent
	RBNode *left;  // pointer to left child
	RBNode *right; // pointer to right child
	enum color { RED, BLACK };
	color col;

	static RBNode* Nil; // sentinel leaf node

	RBNode(int k, RBNode *l = Nil, RBNode *r = Nil, RBNode *parent = Nil, color c = RBNode::BLACK) :
		key(k), p(parent), left(l), right(r), col(c) { }

    string toString() {
        ostringstream os;
        if (col == RBNode::RED) {
            os << key << "\033[31m:r\033[0m";  // Red for ":r"
        } else {
            os << key << "\033[30m:b\033[0m";  // Black for ":b"
        }
        return os.str();
    }


    ~RBNode() {
        if (this != Nil) {
            if (left != Nil) delete left;  // Delete left child only if it's not Nil
            if (right != Nil) delete right;  // Delete right child only if it's not Nil
        }
    }
};

RBNode* RBNode::Nil = new RBNode(0);

struct RBTree {
	RBNode* root;

	void LeftRotate(RBNode* x) {
		// Set y
		RBNode* y = x->right;
		// Turn y's left subtree into x's right subtree
		x->right = y->left;
		if (y->left != RBNode::Nil)
			y->left->p = x;
		// link x's parent to y
		y->p = x->p;
		if (x->p == RBNode::Nil)
			root = y;
		else if (x == x->p->left)
			x->p->left = y;
		else
			x->p->right = y;
		// Put x on y's left
		y->left = x;
		x->p = y;
	}

	/**
	 * RightRotate(y) assumes that y is the right child of a RBNode x
	 */

	void RightRotate(RBNode* y) {
		// Set x
		RBNode* x = y->left;
		// Turn x's right subtree into y's left subtree
		y->left = x->right;
		if (x->right != RBNode::Nil)
			x->right->p = y;
		// link y's parent to x
		x->p = y->p;
		if (y->p == RBNode::Nil)
			root = x;
		else if (y == y->p->left)
			y->p->left = x;
		else
			y->p->right = x;
		// Put y on x's right
		x->right = y;
		y->p = x;
	}

	RBTree() { root = RBNode::Nil; }
    ~RBTree() {
        if (root != RBNode::Nil) {
            delete root;
        }
    }

    RBNode* createNode(int key) { return new RBNode(key); }

	bool isNil(RBNode* n) { return (n == RBNode::Nil); }

	RBNode* search(RBNode* w, int key) {
		if (isNil(w) || w->key == key)
			return w;
		return search( (key < w->key) ? w->left : w->right, key);
	}

    RBNode* maximum(RBNode* w) {
        if (isNil(w)) {
            cout << "\033[31mThe tree is empty. No maximum node exists.\033[0m" << endl;
            return RBNode::Nil;  // Return Nil if tree is empty
        }
        RBNode* x = w;
        while (!isNil(x->right))
            x = x->right;
        return x;
    }

    RBNode* minimum(RBNode* w) {
        if (isNil(w)) {
            cout << "\033[31mThe tree is empty. No minimum node exists.\033[0m" << endl;
            return RBNode::Nil;  // Return Nil if tree is empty
        }
        RBNode* x = w;
        while (!isNil(x->left))
            x = x->left;
        return x;
    }

	RBNode* successor(RBNode* w) {
		if(isNil(w)) return w;
		RBNode* x = w;
		if (!isNil(x->right))
			return minimum(x->right);
		RBNode* y = x->p;
		while (!isNil(y) && x == y->right) {
			x = y;
			y = x->p;
		}
		return y;
	}

	RBNode* predecessor(RBNode* w) {
		if(isNil(w)) return w;
		RBNode* x = w;
		if (!isNil(x->left))
			return maximum(x->left);
		RBNode* y = x->p;
		while (!isNil(y) && x == y->left) {
			x = y;
			y = x->p;
		}
		return y;
	}

    void RBInsert(RBNode* z) {
        // Check if the key already exists in the tree
        RBNode* existingNode = search(root, z->key);
        if (!isNil(existingNode)) {
            cout << "\033[1;31m❌ Error: key already exists in the tree.\033[0m" << endl;
            delete z;  // Free the memory for the duplicate node
            return;
        }

        // Insert like in a bs tree
        RBNode* y = RBNode::Nil;
        RBNode* x = root;
        while (!isNil(x)) {
            y = x;
            x = (z->key < x->key) ? x->left : x->right;
        }
        z->p = y;
        if (isNil(y))
            root = z;  // If tree is empty, make z the root
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;

        z->left = z->right = RBNode::Nil;  // Initialize left and right to Nil
        z->col = RBNode::RED;             // Set new node to red

        cout << "\033[1;32m✔️ Node added successfully!\033[0m" << endl;
        RBInsertFixup(z);
    }

    RBNode* del(RBNode* z) {
        RBNode* y = (isNil(z->left) || isNil(z->right)) ? z : successor(z);
        RBNode* x = !isNil(y->left) ? y->left : y->right;

        if (!isNil(x)) {  // Avoid accessing Nil node
            x->p = y->p;
        }

        if (isNil(y->p)) {
            root = x;
        } else {
            if (y == y->p->left)
                y->p->left = x;
            else
                y->p->right = x;
        }

        if (y != z) {
            z->key = y->key;  // Copy y's key to z
        }

        if (y->col == RBNode::BLACK)
            RBDeleteFixup(x);

        return y;  // Return the deleted node
    }

    void RBDeleteFixup(RBNode* x) {
        RBNode* w;
        while ((x != root) && (x->col == RBNode::BLACK)) {
            if (x == x->p->left) {
                w = x->p->right;
                if (w->col == RBNode::RED) {
                    w->col = RBNode::BLACK;
                    x->p->col = RBNode::RED;
                    LeftRotate(x->p);
                    w = x->p->right;
                }
                if ((w->left->col == RBNode::BLACK) && (w->right->col == RBNode::BLACK)) {
                    w->col = RBNode::RED;
                    x = x->p;
                } else {
                    if (w->right->col == RBNode::BLACK) {
                        w->left->col = RBNode::BLACK;
                        w->col = RBNode::RED;
                        RightRotate(w);
                        w = x->p->right;
                    }
                    w->col = x->p->col;
                    x->p->col = RBNode::BLACK;
                    w->right->col = RBNode::BLACK;
                    LeftRotate(x->p);
                    x = root;
                }
            } else {
                w = x->p->left;
                if (w->col == RBNode::RED) {
                    w->col = RBNode::BLACK;
                    x->p->col = RBNode::RED;
                    RightRotate(x->p);
                    w = x->p->left;
                }
                if ((w->left->col == RBNode::BLACK) && (w->right->col == RBNode::BLACK)) {
                    w->col = RBNode::RED;
                    x = x->p;
                } else {
                    if (w->left->col == RBNode::BLACK) {
                        w->right->col = RBNode::BLACK;
                        w->col = RBNode::RED;
                        LeftRotate(w);
                        w = x->p->left;
                    }
                    w->col = x->p->col;
                    x->p->col = RBNode::BLACK;
                    w->left->col = RBNode::BLACK;
                    RightRotate(x->p);
                    x = root;
                }
            }
        }
        x->col = RBNode::BLACK;  // Set root to black after fixing
    }

	void RBInsertFixup(RBNode* z) {
		while(z->p->col == RBNode::RED)
			if (z->p == z->p->p->left) {
				RBNode* y = z->p->p->right;
				if (y->col == RBNode::RED) {
					z->p->col = RBNode::BLACK;
					y->col = RBNode::BLACK;
					z->p->p->col = RBNode::RED;
                                          z = z->p->p;
				} else {
					if (z==z->p->right) {
						z = z->p;
						LeftRotate(z);
					}
					z->p->col = RBNode::BLACK;
					z->p->p->col = RBNode::RED;
					RightRotate(z->p->p);
				}
			} else {
				RBNode* y = z->p->p->left;
				if (y->col == RBNode::RED) {
					z->p->col = RBNode::BLACK;
					y->col = RBNode::BLACK;
					z->p->p->col = RBNode::RED;
                    z = z->p->p;
				} else {
					if (z==z->p->left) {
						z = z->p;
						RightRotate(z);
					}
					z->p->col = RBNode::BLACK;
					z->p->p->col = RBNode::RED;
					LeftRotate(z->p->p);
				}
			}
		root->col = RBNode::BLACK;
	}

	void inorder(RBNode* T) {
		if (!isNil(T)) {
			inorder(T->left);
			cout << T->toString() << ' ';
			inorder(T->right);
		}
	}

    void inorder() {
        if (isNil(root)) {
            cout << "\033[31mempty\033[0m";
        } else {
            inorder(root);
        }
    }

	void display(RBNode* w, int indent) {
		if (!isNil(w)) {
			display(w->right, indent + 2);
			for (int i = 0; i < indent; i++) cout << ' ';
			cout << w->toString() << '\n';
			display(w->left, indent + 2);
		}
	}

	int bh(RBNode* x) {
		return (isNil(x) ? 0 : countBlacks(x->left));
	}

	int countBlacks(RBNode* x) {
		if(isNil(x)) return 1;
		int n = countBlacks(x->left);
		return ((x->col == RBNode::RED)? n : n+1);
	}

	int bh() {
        return bh(root);
	}

    int depth(RBNode* x) {
        if (isNil(x)) {
            return 0;
        } else {
            int a = depth(x->left);
            int b = depth(x->right);
            return 1 + (a<b ? b : a);
        }
    }

    int depth() {
        return depth(root);
    }

    int maxBlackKey(RBNode* x) {
        if (isNil(x))
            return -1000;
        int leftMax = maxBlackKey(x->left);
        int rightMax = maxBlackKey(x->right);

        int currentMax = (x->col == RBNode::BLACK) ? x->key : -1000;
        return max(currentMax, max(leftMax, rightMax));
    }

    int maxRedKey(RBNode* x) {
        if (isNil(x)) {
            return -1000;
        }
        int leftMax = maxRedKey(x->left);
        int rightMax = maxRedKey(x->right);

        int currentKey = (x->col == RBNode::RED) ? x->key : -1000;

        return max(currentKey, max(leftMax, rightMax));
    }
};

#endif