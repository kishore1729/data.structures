#include <stdio.h>
#include <algorithm>
#include <cstring>

using namespace std;

class avl{
private:
    class avl_node {
	public:
	   int key;
	   int data;
	   int balance;
	   avl_node* left;
	   avl_node* right;
	};

	avl_node* create_node(int key, int data)
	{
		avl_node* nNode = new avl_node;

		if(NULL!=nNode)
		{
			nNode->key = key;
			nNode->data = data;
			nNode->balance = 0;
			nNode->left = nNode->right = NULL;
		}
		return nNode;
	}

	avl_node* avl_rotate (avl_node *root, bool direction )
	{
		avl_node *temp =0;
		if(root == NULL)return NULL;
		switch(direction){
		case true://direction is right
			temp = root->left;
			root->left = temp->right;
			temp->right = root;
			break;
		case false: // direction is left
			temp = root->right;
			root->right = temp->left;
			temp->left = root;
			break;
		}
		return temp;
	}

	avl_node* avl_balance(avl_node* tree, int key)
	{
		avl_node* faultChild;
		avl_node* faultGrandChild;
		bool doubleRotate=false;
		if(tree == NULL) return NULL;
		if(key<tree->key)
		{
			//Rx rotation needed
			faultChild = tree->left;
			if(key>faultChild->key)
			{
				//RL Roataion
				faultGrandChild = faultChild->right;
				//Fixing Balance values for the tree after double rotation (RL)
				if(faultGrandChild->left == NULL && faultGrandChild->right == NULL) // RL Case1 : C is the new node
					tree->balance = faultChild->balance = faultGrandChild->balance =0;
				else if(key<faultGrandChild->key) //RL Case2: new entry was at Left sub tree of C (C is the Grand Child of A, the unbalanced node)
				{
					tree->balance = -1;
					faultChild->balance = faultGrandChild->balance = 0;
				}
				else  //RL Case3: new entry was at Right sub tree of C
				{
					faultChild->balance = 1;
					tree->balance = faultGrandChild->balance = 0;
				}
				tree->left = avl_rotate(faultChild, false);// rotate fault child to the left
				tree = avl_rotate(tree, true);//rotate right about root
			}
			else
			{
				//RR Rotation
				tree = avl_rotate(tree, true);//rotate right about root
				//fix balance factors
				tree->balance = tree->right->balance =0;
			}
		}
		else
		{
			//Lx roatation needed
			faultChild = tree->right;
			if(key<faultChild->key)
			{
				//LR rotation
				faultGrandChild = faultChild->left;
				//Fixing Balance values for the tree after double rotation (LR)
				if(faultGrandChild->left == NULL && faultGrandChild->right == NULL) // LR Case1 : C is the new node
					tree->balance = faultChild->balance = faultGrandChild->balance =0;
				else if(key>faultGrandChild->key) //LR Case2: new entry was at Right sub tree of C (C is the Grand Child of A, the unbalanced node)
				{
					tree->balance = 1;
					faultChild->balance = faultGrandChild->balance = 0;
				}
				else //LR Case3: new entry was at left sub tree of C
				{
					faultChild->balance = -1;
					tree->balance = faultGrandChild->balance = 0;
				}
				tree->right = avl_rotate(faultChild, true);// rotate fault child to the right
				tree = avl_rotate(tree, false);//rotate left about root
			
			}
			else
			{
				//LL Rotation
				tree = avl_rotate(tree, false);//rotate left about root
				//fix balance factors
				tree->balance = tree->left->balance =0;
			}
		}
		return tree;
	}

	avl_node* avl_insert_worker (avl_node *tree, int key, int data, bool* balanceFlag)
	{
		if(tree == NULL)
			tree = create_node (key,data);
		else if(key == tree->key)
			return tree;//only distinct keys, duplicates are ignored. Lists to contain datta can be created as add-on feature.
		else if( key< tree->key)
			tree->left = avl_insert_worker (tree->left, key, data, balanceFlag);
		else
			tree->right = avl_insert_worker (tree->right, key, data, balanceFlag);
		if (*balanceFlag == false) 
		{
			if(key<tree->key)
			{
				tree->balance += 1;
				if (tree->balance == 0)
					*balanceFlag = true;
				if (tree->balance > 1) 
				{
					tree = avl_balance(tree, key);
					*balanceFlag = true;
				}
			}
			else if(key>tree->key)
			{
				tree->balance -= 1;
				if (tree->balance == 0)
					*balanceFlag = true;
				if (tree->balance < -1) 
				{
					tree = avl_balance(tree, key);
					*balanceFlag = true;
				}
			}
		}
		return tree;
	}

	int avl_search_worker (avl_node* tree, int key)
	{
		if(tree == NULL)
			return 0;
		else if (key == tree->key)
			return tree->data;
		else if(key< tree->key)
			return avl_search_worker(tree->left, key);
		else
			return avl_search_worker(tree->right, key);
	}
	void postOrder_worker(avl_node *root)
	{
		if(root != NULL)
		{
			postOrder_worker(root->left);
			postOrder_worker(root->right);
			printf("%d,",root->data);
		}
	}
	void inOrder_worker(avl_node *root)
	{
		if(root != NULL)
		{
			inOrder_worker(root->left);
			printf("%d,",root->data);
			inOrder_worker(root->right);
		}
	}
	avl_node* tree_root;

public:
	int avl_search(int key)
	{
		return avl_search_worker(tree_root, key);
	}
	void avl_insert (int key, int data)
	{
		bool flag = false;
		tree_root = avl_insert_worker(tree_root, key, data, &flag);
	}
	avl()
	{
		tree_root = NULL;
	}
};