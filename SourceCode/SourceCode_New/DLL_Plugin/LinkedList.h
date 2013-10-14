#include <string.h>
class node
{
public:
	char label[256];
	node* next;
	
	node(char* p)
	{
		next=NULL;
		strcpy(label,p);
	}
};

class linklist
{
public:
	node* first;
	
	linklist()
	{
		first=NULL;
	}
	bool isempty()
	{
		return first==NULL;
	}
	void insert(char* p)
	{
		if(first==NULL)
			first=new node(p);
		else
		{
			node* q=first;
			while(q->next!=NULL)
				q=q->next;
			q->next=new node(p);
		}
	}
	void DeleteFromFirst()
	{
		node* q=first;
		first=first->next;
		delete q;
	}
	void DeleteSpecificNode(char* p)
	{
		node* q=first;
		node* q_prev=NULL;
		while(strcmp(q->label,p)!=0)
		{
			q_prev=q;
			q=q->next;
		}
		if(q==first)
		{
			first=first->next;
			delete q;
		}
		else if(q->next=NULL)
		{
			delete q;
			q_prev->next=NULL;
		}
		else
		{
			q_prev->next=q->next;
			delete q;
		}
	}
	int element_count()
	{
		int count=0;
		node* q=first;
		while(q!=NULL)
		{
			count++;
			q=q->next;
		}
		return count;
	}

	void sortElements()
	{
		int count=element_count();
		for(int i=0;i<count;i++)
			for(int j=i;j<count;j++)
			{
				char str_1[256],str_2[256],temp[256];
				GetNodeData(i,str_1);
				GetNodeData(i,str_2);
				if(strcmp(str_1,str_2)<0)
				{
					strcpy(temp,str_1);
					strcpy(str_1,str_2);
					strcpy(str_2,temp);
				}
			}
	}

	bool GetNodeData(int number,char* returned_value)
	{
		if(first==NULL)
			return false;
		node* p=first;
		int counter=0;
		while(counter<number)
		{
			if(p->next==NULL)
				return false;
			p=p->next;
			counter++;
		}
		strcpy(returned_value,p->label);
		return true;
	}
	void clear()//Clear All Linked List
	{
		node* p;
		node* q=first;
		while(q!=NULL)
		{
			p=q->next;
			delete q;
			q=p;
		}
		first=NULL;
	}
	bool find(char* p)
	{
		node* q=first;
		while(q!=NULL)
		{
			if(strcmp(q->label,p)==0)
				return true;
			q=q->next;
		}
		return false;
	}
};