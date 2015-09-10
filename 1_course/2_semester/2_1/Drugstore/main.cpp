#include <iostream>
#include <vector>
#include <string>
#include "Drugstore.h"

using namespace std;

int main()
{
        string name = "Pendel", producer = "Director", ingredient = "Magic";
        Drug a(name, producer, ingredient, 1, tincture);
	cout << a;
	return 0;
}
