#include <stdio.h>
#include <algorithm>
#include <cstring>
#include <map> //for RB tree

using namespace std;

class rbt
{
private:
	map<int, int>* rbtree;

public:
	void rbt_insert(int key, int data)
	{
		rbtree->insert(make_pair(key, data));// make key and data into a pair and insert
	}
	int rbt_search(int key)
	{
		map<int, int>::iterator it = rbtree->find(key); //find iterator of key
		if(it!= rbtree->end())
			return it->second;
		else
			return 0; //value not preesnt in tree
	}
	rbt()
	{
		rbtree = new map<int,int>;
	}
};