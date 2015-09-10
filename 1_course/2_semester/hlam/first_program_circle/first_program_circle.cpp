#include <iostream>
using namespace std;

struct Point
{
	int x, y;
};

class Circle
{
	public:
		Circle()
		{
			c.x = 0;
			c.y = 0;
			r = 0;
		}

		Circle(int new_x, int new_y, int new_r)
		{
			c.x = new_x;
			c.y = new_y;
			r = new_r;
		}

		bool Inside(Point &p)
		{
			int buf = (p.x - c.x)*(p.x - c.x) + (p.y - c.y)*(p.y - c.y); 
			return (buf <= r * r)? true: false;
		}

		void plos()
		{
			cout << 3.14 * r * r << "\n";
		}

		void len()
		{
			cout << 6.28 * r << "\n";
		}

		void mirror(Point &p)
		{
			p.x = 2 * c.x - p.x;
			p.y = 2 * c.y - p.y;
		}

	private:
		Point c;
		int r;
};


int main()
{
	Point p;
	p.x = 1;
	p.y = -2;
	Circle c(1, 2, 3);
	c.len();
	c.plos();
	cout << c.Inside(p) << "\n";
	c.mirror(p);
	cout <<"p.x = "<< p.x << " p.y = " << p.y <<"\n";
	return 0;
}