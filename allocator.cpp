#include <iostream>


using namespace std;

struct node {
	int ID;
	int size;
	int index;
	node * next;


	//constructor
	node (int i,int s, int idx, node *n)
		:ID(i), size(s), index(idx),next(n)
	{}	
};

class HeapManager
{
	private:
		node * head;
    pthread_mutex_t mtx;
	public:

		int initHeap(int size)
		{		
			head= new node(-1,size,0,NULL);
			cout<<"Memort initialized"<<endl;
      print();
      pthread_mutex_init(&mtx, NULL);
			return 1;

		}

		int myFree(int ID, int incomingIndex)
		{
			pthread_mutex_lock(&mtx);
			node *iterator = head;		
			node *itrPrev = head;
			bool found = false;
			while (iterator != NULL)
			{
				if (iterator->index == incomingIndex && iterator->ID==ID && head == iterator) //head düzenlenmeli
				{
					while (itrPrev->next != iterator) //iteratordan öncekini bul
					{
						itrPrev=itrPrev->next;
					}
					
					if (itrPrev->ID == -1 && iterator->next->ID == -1) //sað sol
					{
						itrPrev->next = iterator->next;
						itrPrev->size = itrPrev->size + iterator->size;
						
						if (itrPrev->next->next != NULL && itrPrev->next->ID==-1)
						{
							itrPrev->next = iterator->next->next;
						}
						else
						{
							itrPrev->size = itrPrev->size + itrPrev->next->size;
						}

						iterator->next->size += iterator->size;
						iterator->next->index -= iterator->size;
						head=iterator->next;
						delete iterator;
						
					}		
					
					cout<<"Freed for thread "<< ID <<endl;
					found=true;
					print();
					delete iterator;
					pthread_mutex_unlock(&mtx);
					return 1;
				}
				else if(iterator->index == incomingIndex && iterator->ID==ID && head != iterator)
				{
					while (itrPrev->next != iterator) //iteratordan öncekini bul
					{
						itrPrev=itrPrev->next;
					}

					//itrPrev->next=iterator->next;
					
					if (itrPrev->ID == -1 && iterator->next->ID == -1) //sað sol
					{
						if (iterator->next->next == NULL) //ÝTRpREV'Ý BÝR ÞEYE BAÐLAMA
						{
							itrPrev->size += (iterator->size+iterator->next->size);
						}
						else //itrPrev itr->next->next'ine baðlanmalý
						{
							itrPrev->size += (iterator->size+iterator->next->size);
							itrPrev->next=iterator->next->next;
						}

						//iterator->next->size += iterator->size;
						//iterator->next->index -= iterator->size;
					}
					else if (itrPrev->ID == -1) //sol
					{
						itrPrev->size += iterator->size;
						itrPrev->next=iterator->next;
						//iterator->next->index=itrPrev->index+itrPrev->size;

					}
					else if (iterator->next->ID == -1) //sað
					{
						itrPrev->next = iterator->next;
						iterator->next->size += iterator->size;
						iterator->next->index -=iterator->size;
					}
					else
					{
						itrPrev->next = iterator->next;

					}

					cout<<"Freed for thread "<< ID <<endl;
					found=true;
					print();
					delete iterator;
					pthread_mutex_unlock(&mtx);
					return 1;
				}

				iterator=iterator->next;
			}

			if (!found)
			{
				cout<<"can not freed"<<endl;
        
			}
			pthread_mutex_unlock(&mtx);		
		}


		int myMalloc(int ID, int incomingSize)
		{
			pthread_mutex_lock(&mtx);
			node *iterator = head;
			bool found = false;

			if (head->next == NULL) //ONLY INITHEAP executed
			{
				if (iterator->size > incomingSize) 
				{
					found=true;
					node * newNode = new node(ID,incomingSize, 0, NULL);
					iterator->size -= incomingSize;
					iterator->index=incomingSize;

					newNode->next = iterator;
					head=newNode;
					
					cout<<"Allocated for thread "<< ID <<endl;
					print();
					pthread_mutex_unlock(&mtx);
					return newNode->index;
				}
				else
				{
					cout<<"Can not allocate, requested size "<<incomingSize<< " for thread "<<ID<<" is bigger than remaining size"<<endl;
					print();
					pthread_mutex_unlock(&mtx);
					return -1;				
				}
			}

			else
			{
				node *itrPrev = head;
				while (iterator != NULL)
				{
					if (iterator->size > incomingSize)
					{
						while (itrPrev->next != iterator) //iteratordan öncekini bul
						{
							itrPrev=itrPrev->next;
						}
						found=true;
						node * newNode = new node(ID,incomingSize, itrPrev->index+itrPrev->size, NULL);
						itrPrev->next=newNode;
						newNode->next=iterator;


						iterator->index=newNode->index+incomingSize;
						iterator->size -= incomingSize;
						
						cout<<"Allocated for thread "<< ID <<endl;
						print();
            pthread_mutex_unlock(&mtx);
						return newNode->index;						
					}			
					iterator=iterator->next;					
				}
				if (!found)
				{
					cout<<"Can not allocate, requested size "<<incomingSize<< " for thread "<<ID<<" is bigger than remaining size"<<endl;
					print();
          pthread_mutex_unlock(&mtx);
					return -1;
				}
			}
		}

		void print()
		{
			node *itr=head;
			string printed="";

			if (itr->next == NULL)
			{
				cout<<"["<<itr->ID<<"]"<<" ["<<itr->size<<"]"<<" ["<<itr->index<<"]"<<endl;
			}

			else
			{
				while (itr != NULL)
				{
					cout<<"["<<itr->ID<<"]"<<" ["<<itr->size<<"]"<<" ["<<itr->index<<"]";
					
					itr=itr->next;
					cout<<"---";
				}
			}
	
		
		}

};

