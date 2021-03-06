#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int W_LEN = 100;
const int AM_W = 10;


int cmp(const void *a, const void *b)
{
	return strcmp(*(char**)a, *(char**)b);
}

struct handler
{
    char **all;	//store all words
    char *word;	//store current word

    long int amount_ch; //current amount chars in "word"
    long int amount_w;  //current amount words in "all"
    long int cap_all;	//capasity of "all"

};

void add_word(struct handler *h)
{
    if(h == NULL)
    {
        perror("Bad alloc. h == NULL\n");
        exit(1);
    }
    if(h->amount_ch > 0)
    {
        if(h->cap_all <= h->amount_w)
        {
            char **checker = (char**)realloc(h->all, 1.2 * h->cap_all * sizeof(char*) );
            if(checker != NULL)
            {
                h->all = checker;
            }
            else
            {
                perror("Memory is end");
                exit(1);
            }

            h->cap_all *= 1.2;
        }
        h->word[h->amount_ch] = '\0';
        h->all[h->amount_w] = h->word;

        h->amount_ch = 0;
        h->word = (char*)malloc(W_LEN*sizeof(char) );
        ++h->amount_w;
    }
}

void add_char(struct handler *h, char ch)
{
    if(h == NULL)
    {
        perror("Bad alloc. h == NULL\n");
        exit(1);
    }
    if(!isspace(ch) && ch != '\n' && ch != '\t')
    {
        if(W_LEN <= h->amount_ch + 1 && strlen(h->word) <= h->amount_ch + 1)
        {
            char* checker = (char*)realloc(h->word, 1.2 * strlen(h->word) * sizeof(char) );
            if(checker != NULL)
            {
                h->word = checker;
            }
            else
            {
                perror("Memory is end");
                exit(1);
            }
        }

        h->word[h->amount_ch] = ch;
        ++h->amount_ch;
    }
}


int main()
{
    struct handler h;
    h.all = (char**)malloc(AM_W*sizeof(char*) );	//store all words
    h.word = (char*)malloc(W_LEN*sizeof(char) );	//store current word
    char ch;                                        //store current char

    if(h.all == NULL || h.word == NULL)
    {
        perror("Bad alloc\n");
        exit(1);
    }

    h.amount_ch = 0;    //current amount chars in "word"
    h.amount_w = 0;     //current amount words in "all"
    h.cap_all = AM_W;	//capasity of "all"
	
	//read all words
	while((ch = getchar() ) != EOF)
	{
        //look for special characters
        if(ch == ';')
        {
            add_word(&h);       //add current word in all
            add_char(&h, ch);   //add ';' in word
            add_word(&h);       //add word, which store ';', in all
        }
        else if(ch == '&' || ch == '|')
        {
            add_word(&h);       //add current word in all

            char second_ch = getchar(); //read second char

            //if '&&' or '||'
            if(second_ch == ch)
            {
                add_char(&h, ch);           //add ch in word
                add_char(&h, second_ch);    //add second char in word
                add_word(&h);               //add word in all
            }
            //if ch is last char in stream
            else if(second_ch == EOF)
            {
                add_char(&h, ch);           //add ch in word
                break;
            }
            //if ch != second_ch
            else
            {
                add_char(&h, ch);           //add ch in word
                add_word(&h);               //add word, which store ch, in all
                add_char(&h, second_ch);    //add second char in word
            }

        }
		//add "ch" in "word"
        else if(!isspace(ch) && ch != '\n' && ch != '\t')
		{
            add_char(&h, ch);
		}
		//add "word" in "all"
        else
		{
            add_word(&h);
		}
	}

    //add last word
    add_word(&h);
	
	//sorting words in "all"
    qsort(h.all, h.amount_w, sizeof(char*), cmp);

	//write words in stdout
    int i;
    for(i = 0; i < h.amount_w; ++i)
    {
        printf("'%s'\n", h.all[i]);
    }

	return 0;
}
