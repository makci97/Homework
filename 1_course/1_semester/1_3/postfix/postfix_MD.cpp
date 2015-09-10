#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int len_symbol = 10; // длина поля значения стэка

struct Stack_Node
{
	char *value;
	Stack_Node *next;
};

struct Stack
{
	Stack_Node *first;
	Stack();
	~Stack();
	void add(char *c);
	char *del();
	int pyst();
	void print_n();
	void print_s();
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

void Stack :: add(char *c)
{
	Stack_Node *new_node = (Stack_Node*) malloc(sizeof(Stack_Node));
	new_node -> value = (char*) malloc(strlen(c) * sizeof(char));
	strcpy(new_node -> value, c);
	new_node -> next = first;
	first = new_node;
}

char *Stack :: del()
{
	if(first != 0)
	{
		Stack_Node *del = first;
		first = first -> next;
		char *c = (char*) malloc(sizeof(char) * strlen(del -> value));
		strcpy(c, del -> value);
		free(del);
		return c;
	}
	char *c = (char*) malloc(sizeof(char));
	c[0] = '!';
	c[1] = '\0';
	return c;
}

int Stack :: pyst()
{
	if(first == 0)
		return 1;

	return 0;
}

void Stack :: print_n()
{
	Stack_Node *cur, *last = 0;

	while(last != first)
	{
		cur = first;
		while(cur -> next != last)
		{
			cur = cur -> next;
		}
		printf("%s ", cur -> value);
		last = cur;
	}

	return;
}

void Stack :: print_s()
{
	Stack_Node *cur = first;

	while(cur != 0)
	{
		printf("%s ", cur -> value);
		cur = cur -> next;
	}
	return;
}



int main()
{
	Stack stack, numbers;
	char c ='\0', *buf = (char*) malloc(len_symbol * sizeof(char));
	int i = 0;

	for (int i = 0; (c = getchar()) != '\n'; i++)
	{

		if(c >= '0' && c <= '9')
		{
			int j = 0;
			do
			{
				buf[j++] = c;
			}
			while((c = getchar()) >= '0' && c <= '9' && j < len_symbol);

			buf[j] = '\0';
			numbers.add(buf);
		}
		//если считываем цифры

		if(c == '(' || c == ')')
		{
			if(c == '(')
			{
				buf[0] = c;
				buf[1] = '\0';
				stack.add(buf);
			}
			else
			{
				buf = stack.del();
				while(buf[0] != '(')
				{
					numbers.add(buf);
					buf = stack.del();
				}
			}
		}


		if(c == '+' || c == '-')
		{
			buf = stack.del();
			if(buf[0] != '!' && (buf[0] == '(' || buf[0] == '+' || buf[0] == '-'))
				stack.add(buf);
			else
			{
				while(buf[0] != '!' && buf[0] != '(' && buf[0] != '+' && buf[0] != '-')
				{
					numbers.add(buf);
					buf = stack.del();
				}
				if(buf[0] != '!' && (buf[0] == '(' || buf[0] == '+' || buf[0] == '-'))
					stack.add(buf);
			}

			buf[0] = c;
			buf[1] = '\0';
			stack.add(buf);

		}
		//если считываем знаки низкого приоритета '+', '-'


		if(c == '*' || c == '/')
		{
			buf = stack.del();

			if(buf[0] != '!' && (buf[0] == '(' || buf[0] == '+' || buf[0] == '-'))
				stack.add(buf);
			else
			{
				while(buf[0] != '!' && buf[0] != '(' && buf[0] != '+' && buf[0] != '-')
				{
					numbers.add(buf);
					buf = stack.del();
				}

				if(buf[0] != '!' && (buf[0] == '(' || buf[0] == '+' || buf[0] == '-'))
					stack.add(buf);
			}


			buf[0] = c;
			buf[1] = '\0';
			stack.add(buf);

		}
		//если считываем знаки высокого приоритета '*', '/'


	}	

	numbers.print_n();
	stack.print_s();

	return 0;
}