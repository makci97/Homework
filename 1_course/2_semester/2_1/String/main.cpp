#include <iostream>
#include "String.h"

using namespace std;

int main()
{
        char c[] = "sgs 23r dfhds", cc[] = "1s0fd", c1[] = "trt", c2[] = "ghjb";
        String s(c), ss(cc),s2;
        cin >> s2;
        cout << "\n" << s2 << endl;
        cout << s << s.Size() << endl;
        cout << s.InsertAfterIElement(5, c1);
        cout << s.Size()<< endl;
        cout << s.InsertAfterLastElement(c2) << endl;
	cout << s.RemoveFromItoJ(4,6) << endl;
	s += ss;
        cout << s << endl;
	return 0;
}
