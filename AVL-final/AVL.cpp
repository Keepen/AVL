#include <iostream>
using namespace std;

//AVL结点
template <class T>
struct AVLNode {
	T _data;
	AVLNode<T>* _left;
	AVLNode<T>* _right;
	AVLNode<T>* _parent;
	int _bf;	//平衡因子

	AVLNode(const T& data = T())
		:_data(data),
		_left(nullptr),
		_right(nullptr),
		_parent(nullptr),
		_bf(0){}
};

template <class T>
class AVL {
public:
	typedef AVLNode<T> Node;
	typedef Node* pNode;
	AVL()
		:_root(nullptr){}

	//左边的左边高——右旋
	void RotateRight(pNode parent) {
		pNode subL = parent->_left;
		pNode subLR = subL->_right;

		subL->_right = parent;
		parent->_left = subLR;

		if (subLR) {
			subLR->_parent = parent;
		}

		//判断是否是根节点
		if (parent != _root) {
			pNode gParent = parent->_parent;
			if (gParent->_left == parent) {
				gParent->_left = subL;
			}
			else {
				gParent->_right = subL;
			}
			subL->_parent = gParent;
		}
		//是根节点
		else {
			_root = subL;
			subL->_parent = nullptr;
		}
		parent->_parent = subL;
		//更新平衡因子
		subL->_bf = parent->_bf = 0;
	}

	//右边的右边高——左旋
	void RotateLeft(pNode parent) {
		pNode subR = parent->_right;
		pNode subRL = subR->_left;

		subR->_left = parent;
		parent->_right = subRL;

		if (subRL) {
			subRL->_parent = parent;
		}

		//判断是否是根节点
		if (parent != _root) {
			pNode gParent = parent->_parent;
			if (gParent->_left == parent) {
				gParent->_left = subR;
			}
			else {
				gParent->_right = subR;
			}
			subR->_parent = gParent;
		}
		//是根节点
		else {
			_root = subR;
			subR->_parent = nullptr;
		}
		parent->_parent = subR;
		subR->_bf = parent->_bf = 0;
	}


	bool Insert(const T& data) {
		//判断树是否为空
		if (_root == nullptr) {
			_root = new Node(data);
			return true;
		}
		//树非空，进行搜索，找出合适的位置进行插入
		pNode cur = _root;
		pNode parent = nullptr;			
		while (cur) {
			parent = cur;
			//不能有相同的值
			if (cur->_data == data) {
				return false;
			}
			
			if (cur->_data > data) {
				cur = cur->_left;
			}
			else {
				cur = cur->_right;
			}
		}
		cur = new Node(data);
		if (parent->_data>data) {
			parent->_left = cur;
		}
		else {
			parent->_right = cur;
		}
		cur->_parent = parent;
		//结点插入完毕，开始调整		
		while (parent) {
			//先更新平衡因子
			if (parent->_left == cur) {
				--parent->_bf;
			}
			else {
				++parent->_bf;
			}		
			 if (parent->_bf == -1 || parent->_bf == 1) {
				//需要进行向上回溯，再次检查
				cur = parent;
				parent = parent->_parent;
			}
			 else if (parent->_bf == 0) {
				 break;
			 }
			//剩余的情况就是平衡因子>2    或者    <-2，需要进行旋转调整
			else {
				//左边的左边高——右旋调整
				if (parent->_bf == -2 && cur->_bf == -1) {
					RotateRight(parent);
					//break;
				}
				//右边的右边高——左旋调整
				else if (parent->_bf == 2 && cur->_bf == 1) {
					RotateLeft(parent);
					//break;
				}
				//右边的左边高——先右旋再左旋
				else if (parent->_bf == 2 && cur->_bf == -1) {
					pNode subR = parent->_right;
					pNode subRL = subR->_left;
					int bf = subRL->_bf;
					RotateRight(cur);
					RotateLeft(parent);
					//由于可能会错误的更改了某些节点的平衡因子
					//故需更改
					if (bf == 1) {
						subR->_bf = 0;
						parent->_bf = -1;
					}
					else if (bf == -1) {
						subR->_bf = 1;
						parent->_bf = 0;
					}
					//break;
				}
				//左边的右边高——先左旋再右旋
				else if (parent->_bf == -2 && cur->_bf == 1) {
					pNode subL = parent->_left;
					pNode subLR = subL->_right;
					int bf = subLR->_bf;
					RotateLeft(cur);
					RotateRight(parent);
					if (bf == -1) {
						parent->_bf = 1;
						subL->_bf = 0;
					}
					else if (cur->_right->_bf == 1) {
						parent->_bf = 0;
						subL->_bf = -1;
					}
					//break;
				}
				break;
			}
		}
	}

	void InOrder() {
		inorder(_root);
		cout << endl;
	}
	void inorder(pNode root) {
		if (root == nullptr) {
			return;
		}
		inorder(root->_left);
		cout << root->_data << " ";
		inorder(root->_right);
	}
	
	int Height(pNode root) {
		if (root == nullptr) {
			return 0;
		}
		int left = Height(root->_left);
		int right = Height(root->_right);
		return left > right ? left + 1 : right + 1;
	}

	bool _isBalance(pNode root) {
		if (root == nullptr) {
			return true;
		}
		int leftHeight = Height(root->_left);
		int rightHeight = Height(root->_right);
		if (root->_bf != rightHeight - leftHeight) {
			cout << root->_data << "不平衡" << endl;
			cout << "平衡因子：" << root->_bf << "高度差：" << rightHeight - leftHeight << endl;
			return false;
		}
		return abs(root->_bf) < 2 
			&& _isBalance(root->_left) 
			&& _isBalance(root->_right);
	}

	bool isBalance() {
		return _isBalance(_root);
	}


private:
	pNode _root;
};

void test() {
	AVL<int> avl;
	//int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16,14 };

	for (int i = 0; i < sizeof(a) / sizeof(a[0]); ++i) {
		avl.Insert(a[i]);
		//cout << avl.isBalance() << endl;
	}

	avl.InOrder();
	cout << avl.isBalance() << endl;

}

int main() {
	test();
	system("pause");
	return 0;
}