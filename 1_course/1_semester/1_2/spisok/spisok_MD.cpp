#include <stdio.h>
#include <stdlib.h>

struct spisok_elem
{
	int value;
	spisok_elem *left, *right;
};

struct spisok
{
	spisok_elem *first, *last;
	spisok();										//конструктор
	~spisok();										//деструктор
};

/*
	spisok *list_new();								//создать новый пустой список
	void list_delete(spisok *l);					//удалить список и все его элементы
	void push(spisok *l, const int *a);				//добавить новый элемент a в конец списка
	void pop(spisok *l, int *x);					//извлечь последний элемент списка
	void unshift(spisok *l, const int *a);			//добавить новый элемент a в начало
	void shift(spisok *l, int *x);					//извлечь первый элемент списка
	void reverse(spisok *l);						//инвертирование списка
*/

spisok::spisok()									//конструктор
{
	first = 0;
	last = 0;
}


spisok::~spisok()									//деструктор
{
	while(first -> right != last)
	{
		first = first->right;
		free(first->left);
	}

	free(first);
	free(last);
}

/*
spisok * list_new()							//создать новый пустой список
{
	spisok *new_spisok;
	new_spisok -> first = 0;
	new_spisok -> last = 0;
	return new_spisok;
}
*/

void push(spisok *l, const int *a)				//добавить новый элемент a в конец списка
{
	spisok_elem *new_elem = (spisok_elem*) malloc(sizeof(spisok_elem));
	new_elem -> value = *a;

	if(l -> last != 0)
	{
		new_elem -> left = l -> last;
		l -> last -> right = new_elem;
		l -> last = new_elem;
		l -> last -> right = 0;
	}
	else
	{
		l -> first = new_elem;
		l -> last = new_elem;
		new_elem -> left = 0;
		new_elem -> right = 0;
	}

	return;
}


void unshift(spisok *l, const int *a)			//добавить новый элемент a в начало
{
	spisok_elem *new_elem = (spisok_elem*) malloc(sizeof(spisok_elem));
	new_elem -> value = *a;

	if(l -> first != 0)
	{
		new_elem -> right = l -> first;
		l -> first -> left = new_elem;
		l -> first = new_elem;
		l -> first -> left = 0;
	}
	else
	{
		l -> first = new_elem;
		l -> last = new_elem;
		new_elem -> left = 0;
		new_elem -> right = 0;
	}

	return;
}


void pop(spisok *l, int *x)					//извлечь последний элемент списка
{
	if(l -> last == 0)
		*x = -1;

	else
	{
		*x = l -> last -> value;
		if(l -> last == l -> first)
		{
			l -> last -> left = 0;			
			l -> last -> right = 0;
			free(l -> first);
			l -> last = 0;
			l -> first = l -> last;
		}
		else
		{
			l -> last = l -> last -> left;
			free(l -> last -> right);
			l -> last -> right = 0;
		}
	}

	return;
}


void shift(spisok *l, int *x)					//извлечь первый элемент списка
{
	if(l -> first == 0)
		*x = -1;

	else
	{
		*x = l -> first -> value;
		if(l -> last == l -> first)
		{
			l -> last -> left = 0;			
			l -> last -> right = 0;
			free(l -> first);
			l -> last = 0;
			l -> first = l -> last;
		}
		else
		{
			l -> first = l -> first -> right;
			free(l -> first -> left);
			l -> first -> left = 0;
		}
	}

	return;
}


void list_delete(spisok *l)				//удалить список и все его элементы
{
	if(l -> first == 0)
		return;


	while(l -> first -> right != 0)
	{
		spisok_elem *cur = l -> first;
		l -> first = l -> first -> right;
		l -> first -> left = 0;
		free(cur);
	}

	free(l -> first);
	l -> first = 0;
	l -> last = 0;
	printf("Spisok PUST!\n");
	return;
}


void reverse(spisok *l)						//инвертирование списка
{
	if(l -> first == 0)
		return;

	l -> first = l -> last;
	spisok_elem *cur = l -> last;


	while(l -> last -> left != 0)
	{
		l -> last = l -> last -> left;
		cur -> left = cur -> right; 
		cur -> right = l -> last;
		cur = l -> last;
	}

	l -> last -> left = l -> last -> right;
	l -> last -> right = 0;


	return;
}

void print_spisok(spisok *l)
{
	spisok_elem *cur = l -> first;
	if(cur != 0)
	{
		while(cur != 0)
		{
			printf("%i\t", cur -> value);
			cur = cur -> right;
		}
		printf("\n");
	}

	return;
}

int main()
{
	spisok l;
	char p = 23;
	int a;
	while(p != 27)
	{
		p = getchar();
		
		if( p == '1')				//добавить новый элемент a в конец списка
		{
			scanf("%i", &a);
			push(&l, &a);	
			print_spisok(&l);
		}

		if( p == '2')				//добавить новый элемент a в начало
		{
			scanf("%i", &a);
			unshift(&l, &a);
			print_spisok(&l);
		}

		if( p == '3')				//извлечь последний элемент списка
		{
			pop(&l, &a);
			printf("%i\n", a);
			print_spisok(&l);
		}

		if( p == '4')				//извлечь первый элемент списка
		{
			shift(&l, &a);
			printf("%i\n", a);
			print_spisok(&l);
		}

		if( p == '5')				//инвертирование списка
		{
			reverse(&l);
			print_spisok(&l);						
		}

		if( p == '6')				//удалить список и все его элементы
		{
			list_delete(&l);
			print_spisok(&l);
		}
	}
	return 0;
}