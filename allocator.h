#include <iostream>




struct node {
	int ID;
	int size;
	int index;
	node * next;

	//default constructor
	node::node (int i,int s, int idx, node *n)
		:ID(-1), size(s), index(idx),next(NULL)
	{}

	//constructor
	node::node (int i,int s, int idx, node *n)
		:id(i), size(s), index(idx),next(n)
	{}	
};

class HeapManager
{
	private:
		node * head;
	public:

		int initHeap(int size)
		{		
			head= new node(-1,size,0,NULL);
			return 1;

		}

		int myFree(int ID, int incomingIndex)
		{
			node *iterator = head;

			while (iterator != NULL)
			{
				if (iterator->index = incomingIndex)
				{

				}




				iterator=iterator->next;
			}
		
		}

		int myMalloc(int ID, int incomingSize)
		{
			node *iterator = head;

			if (head->next == NULL) //ONLY INITHEAP executed
			{
				if (iterator->size > incomingSize)
				{
					node * newNode = new node(ID,incomingSize, iterator->index-incomingSize, NULL);
					iterator->size-=incomingSize;
					iterator->next = newNode;

					return newNode->index;
				}
				else
				{
					//initheap does not have enoguh space
					return -1;				
				}
			}

			else
			{
				while (iterator->next != NULL)
				{
					if (iterator->size > incomingSize)
					{
						node * iteratorRepl = iterator->next;
						node * newNode = new node(ID,incomingSize, iterator->index-incomingSize, NULL);

						iterator->size -= incomingSize;
						if (iterator->size < newNode->size )
						{
							iterator->next = newNode;
							newNode->next=iteratorRepl;
							head = iterator;
							return newNode->index;
						}
						else
						{
							iterator->next = iteratorRepl;
							newNode->next = iterator;
							head = newNode;
							return newNode->index;
						}				
						
					}
					iterator=iterator->next;
				}
			}
		}

		void print()
		{
			while (head->next != NULL)
			{
				cout>>"[">>
			}
	
		
		}

};

#endif