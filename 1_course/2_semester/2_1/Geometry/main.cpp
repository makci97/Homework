#include <iostream>
#include "Shapes.h"
#include "point_segment_vector.h"

int main()
{	
        Circle c(1, 1, 3);
        Point pp1(1.0, 5.000001);

        cout << c.Area() << endl;
        cout << c.PointOnBorder(pp1) << endl;
        cout << c.PointInShape(pp1) << endl;

        Segment s1(0, 0, 0, 1), s2(1, -1, -1, 2), ss(1, 1, 7, 7);
        try
        {
                pp1 = c.PointOfIntersectionWithSegment(ss);
                cout<< pp1 << endl;
                cout << c.PointOnBorder(pp1) << endl;
	}
	catch(IntersectionIsEmpty()){ }
	

	
	Point p1(0,0), p2(0,2), p3(1,1), p4(1,0);
	vector<Point> p;
	
	p.push_back(p1);
	p.push_back(p2);
	p.push_back(p3);
	p.push_back(p4);
	
	Polygon pp(p);

	Point point(0.25, 0.5);
        cout << pp.Area() << endl;
        cout << pp.PointInShape(point) << endl;
	try
        {
		Segment s(1, 0, 2, 1);
                cout << pp.PointOfIntersectionWithSegment(s) << endl;
	}
        catch(IntersectionIsEmpty())
	{
        }
        cout << pp.PolygonIsConvex()<<endl;

	Convex_Polygon cp(p);

	cout << cp.Area() <<endl;
        cout << pp.Area() << endl;
	return 0;
}
