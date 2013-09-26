#include <stdio.h>
#include <algorithm>
#include <cstring>
#include <vector>
#include <queue>

#define NULL 0

using namespace std;

class btree
{
private:
    class bt_node
	{
	public:
		int height;
		vector<int> key;
		vector<int> data;
		vector<long int> child;
	};
	bt_node* create_newnode(void)
	{
		bt_node* nNode = new bt_node;
		if(NULL!=nNode)
		{
			nNode->height =0;
			nNode->key.empty();
			nNode->data.empty();
			nNode->child.empty();
		}
		return nNode;
	}
	bt_node* bt_split(bt_node* node)
	{
		bt_node* nParent = create_newnode();
		bt_node* nChild = create_newnode();
		const unsigned int pMid = (bOrder+1)/2;
		unsigned int count=0;
		vector<int>::iterator itk,itd;
		vector<long int>::iterator itc = node->child.begin();	itc++; //incrementing child array pointer
		for(itk = node->key.begin(),itd= node->data.begin(); itk < node->key.end(); itk++, itd++, itc++, count++)
		{
			if(count == pMid)
			{
				// copy new parent details
				nParent->key.push_back(*itk);
				nParent->data.push_back(*itd);
				nParent->child.push_back((long int)node); //copy left pointer
				nParent->child.push_back((long int)nChild); // copy right pointer
				nChild->child.push_back(*itc);
				//node->child.erase(itc);
				//set height parameter
				nChild->height = node->height;
				nParent->height = node->height + 1;
			}
			else if(count > pMid)
			{
				//copy second half of overfull node to new child node
				nChild->key.push_back(*itk);
				nChild->data.push_back(*itd);
				nChild->child.push_back(*itc);
			}
		}
		//for(itk = node->key.begin(),itd= node->data.begin(), itc = node->child.begin(), count =0; count>pMid; itk++, itd++, itc++, count++);
		count=node->key.size();
		while (count>pMid)
		{
			node->key.pop_back();
			node->data.pop_back();
			node->child.pop_back();
			count--;
		}
		return nParent;
	}
	bt_node* bt_merge(bt_node* tree, bt_node* temp)
	{
		vector<int>::iterator itk,itd;
		vector<long int>::iterator itc = tree->child.begin(); itc++;
		for(itk = tree->key.begin(),itd= tree->data.begin(); itk < tree->key.end(); itk++, itd++, itc++)
		{
			if(temp->key.front() < (*itk))
			{
				//Insert and check for overflow.
				tree->key.insert(itk, temp->key.front());// key is inserted at appropriate place
				tree->data.insert(itd, temp->data.front());// data is inserted at appropriate place
				//left child of temp will be the same as pointer sitting in between a and b, where a is largest key less than temp's key
				// and b is smallest key greater than temp's key
				tree->child.insert(itc,temp->child.back());
				break;
			}
			else
			{
				if(itk==(tree->key.end()-1))
				{
					tree->key.push_back(temp->key.front());
					tree->data.push_back(temp->data.front());
					tree->child.push_back(temp->child.back());
					break;
				}
			}
		}
		delete temp;
		return tree;
	}
	bt_node* bt_insert_in_node(bt_node* tree, int key, int data)
	{
		vector<int>::iterator itk,itd;
		unsigned int i=0;
		bt_node* temp = tree;
		for(itk = tree->key.begin(),itd= tree->data.begin(); itk < tree->key.end(); itk++, itd++, i++)
		{
			if((*itk)==key)
			{
				 return tree; // no duplicates allowed, ignoring input.
			}
		
			if(key < (*itk))
			{
				if(tree->child.at(i)!= NULL)
					{temp = bt_insert_in_node((bt_node*)tree->child.at(i), key, data);break;}
				else
				{
					//Insert and check for overflow.
					tree->key.insert(itk, key);
					tree->data.insert(itd, data);
					tree->child.push_back(NULL);
					if(tree->key.size()>bOrder)
						temp = bt_split(tree);
					break;
				}
			}
			else
			{
				if(itk==(tree->key.end()-1))
				{
					if (tree->height != 0)
						{temp = bt_insert_in_node((bt_node*)tree->child.at(++i), key, data);break;}
					else
					{
					tree->key.push_back(key);
					tree->data.push_back(data);
					tree->child.push_back(NULL);
					if(tree->key.size()>bOrder)
						temp = bt_split(tree);
					break;
					}
				}
			}
		}
		if(temp != tree)
		{
			if(temp->height == tree->height)
			{
				//merge the two nodes
				tree = bt_merge(tree, temp);
				//if this node overflows, split and return new parent to this node's parent
				if(tree->key.size()>bOrder)
				{
					tree = bt_split(tree);
				}
			}
			else if(temp->height > tree->height)
				tree = temp; // new root is created
			else
				return tree;// insert happened on lower node, pass tree back
		}
		return tree;
	}
	bt_node* bt_insert_worker (bt_node* tree, int key, int data)
	{
		bt_node* temp=0;
		if(key<tree->key.front()&& tree->child.front() != NULL)
			temp = bt_insert_worker((bt_node*)tree->child.front(), key, data);
		else if(tree->key.size()>=2 && key>tree->key.back() && tree->child.back() != NULL)
			temp = bt_insert_worker((bt_node*)tree->child.back(), key,data);
		else if(tree->key.size()<2 && key>tree->key.front() && tree->child.back() != NULL)
			temp = bt_insert_worker((bt_node*)tree->child.back(), key,data);
		else
			temp = bt_insert_in_node(tree, key, data);
		if(temp != tree)
		{
			if(temp->height == tree->height)
			{
				//merge the two nodes
				tree = bt_merge(tree, temp);
				//if this node overflows, split and return new parent to this node's parent
				if(tree->key.size()>bOrder)
				{
					tree = bt_split(tree);
				}
			}
			else if(temp->height > tree->height)
				tree = temp; // new root is created
			else
				return tree;// insert happened on lower node, pass tree back
		}
		return tree;
	}
	int bt_search_in_node(bt_node* tree, int key)
	{
		unsigned int it;
		for(it = 0; it < tree->key.size(); it++)
		{
			if(tree->key.at(it)==key)
			{
				 return tree->data.at((unsigned int)it);
			}
			if(key < tree->key.at(it))
			{
				if(tree->child.at(it)!= NULL)
					return bt_search_in_node((bt_node*)tree->child.at(it), key);
				else
					return 0;
			}
		}
		return 0;
	}
	int bt_search_worker(bt_node* tree, int key)
	{
		if(tree == NULL)
			return 0;
		else if(key<tree->key.front()&& tree->child.front() != NULL)
			return bt_search_worker((bt_node*)tree->child.front(), key);
		else if(key>tree->key.back() && tree->key.back() != NULL)
			return bt_search_worker((bt_node*)tree->child.back(), key);
		else
			return bt_search_in_node(tree, key);
	}
	void bt_inOrder_worker (bt_node*root)
	{
		if (root != NULL)
		{
			int i;
			for ( i = 0 ; i < root->key.size() ; i++ )
			{
				bt_inOrder_worker ((bt_node*)root->child.at(i)) ;
				printf("%d,",root->data.at(i));//error had not changed the printf into file output
			}
			bt_inOrder_worker ((bt_node*) root->child.at(i));
		}
	}
	void bt_level_worker(bt_node* root)
	{
		queue<long int> q;
		q.push((long int)tree_root);
		while (!q.empty())
		{
			bt_node* currentNode = (bt_node*)q.front();
			if ( currentNode->child.front() != NULL )
				q.push(currentNode->child.front());
			vector<int>::iterator it;
			vector<int>::iterator itd;
			unsigned int itc = 0;	itc++;
			itd = currentNode->data.begin();
			for(it= currentNode->key.begin(); it<currentNode->key.end(); it++, itc++, itd++)
			{
				printf("%d,", *itd);
				if ( currentNode->child.at(itc) != NULL )
					q.push(currentNode->child.at(itc));
			}
			q.pop();
		}
	}
	int bOrder;
	bt_node* tree_root;

public:
	int bt_search(int key)
	{
		return bt_search_worker(tree_root, key);
	}
	void bt_insert(int key, int data)
	{
		tree_root = bt_insert_worker(tree_root, key, data);
	}
	btree(int inbOrder, int key, int data)
	{
		bOrder = inbOrder;
		tree_root = create_newnode();
		tree_root->key.push_back(key);
		tree_root->data.push_back(data);
		tree_root->child.push_back(NULL);
		tree_root->child.push_back(NULL);
	}
};