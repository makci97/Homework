#include <stdio.h>
#include <stdlib.h>

struct Stack_Node
{
	char value;
	Stack_Node *next;
};

struct Stack
{
	Stack_Node *first;
	Stack();
	~Stack();
	void add(char c);
	char del();
	int pyst();
};

Stack :: Stack()
{
	first = 0;
}

Stack :: ~Stack()
{
	while(first != 0)
	{
		Stack_Node *del = first;
		first = first -> next;
		free(del);
	}
}

void Stack :: add(char c)
{
	Stack_Node *new_node = (Stack_Node*) malloc(sizeof(Stack_Node));
	new_node -> value = c;
	new_node -> next = first;
	first = new_node;
}

char Stack :: del()
{
	if(first != 0)
	{
		Stack_Node *del = first;
		first = first -> next;
		char c = del -> value;
		free(del);
		return c;
	}
	return '-1';
}

int Stack :: pyst()
{
	if(first == 0)
		return 1;

	return 0;
}


int main()
{
	Stack s;
	char c ='\0', buf;
	int set = 1, i = 0;

	for (int i = 0; (c = getchar()) != '\n'; i++)
	{
		if(c == '(' || c == '{')
		{
			s.add(c);
		}
		else if(c == ')' || c == '}')
		{
			buf = s.del();
			if( !((c == ')' && buf == '(') || (c == '}' && buf == '{')) )
			{
				set = 0;
				break;
			}
		}
		else
		{
			set = 0;
			break;
		}
	}	

	if(set == 1 && s.pyst())
		printf("YES");
	else
		printf("NO");
	return 0;
}