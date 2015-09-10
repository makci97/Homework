#include <iostream>
#include "Quaternion.h"

using namespace std;

int main()
{
        Quaternion a(1,1,1,1), b(-2,3,2,-1), c(0,0,1,0), d(3,0,-1,2), res;
        res = a + c;
        cout << "\na = " << a << ";\tb = " << b << ";\tc = " << c << ";\td = " << d << ";\n";
        cout << "res = a + c = "<< res << endl;
        cout << "b.VectorProduct(d) = " << b.VectorProduct(d) << endl;
        cout << "c.ScalarProduct(a) = " << c.ScalarProduct(a) << endl;
        cout << "d.Interfaced() = " << d.Interfaced() << endl;
        cout << "a.Mod() = " << a.Mod() << "\nVvedite res. res = (a, i, j, k) .\tres = ";
        cin  >> res;
        cout << "\nres == a ? " << (res == a) << endl;
        cout << "res * a = " << res * a << endl;
	return 0;
}
