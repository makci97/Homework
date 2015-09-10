

#include <iostream>
#include <vector>
#include "point_segment_vector.h"

using namespace std;

void CrossSegments(vector<Segment> s)
{
	int size = s.size();
	for(int i = 0; i < size; ++i)
	{
                for(int j = i + 1; j < size; ++j)
		{
                        try
                        {
                                cout << s[i].PointOfIntersectionOfSegments(s[j]) << " " << s[i] << " " << s[j] << endl;
                        }
                        catch(...) { }
		}
	}
}

int main()
{
    Point p1(0,0), p2(2,2), p3(2,0), p4(0,2), p5(2,1), p6(1,1), p7(2,-1), p8(1,2);
    Segment s1(p1, p2), s2(p3, p4), s3(p5, p6), s4(p7, p8);
    Vector v1(s1), v2(s2);
    cout << "Point of intersection of segments " << s1 << " and " << s2 << ":\n" << s1.PointOfIntersectionOfSegments(s2) << "\n\n";
    cout << "Vector: " << s1.GetVector() << " corresponds to a segment: " << s1 << "\n\n";
    cout << s1.PointBelongSegment(p5) << " " << s1.PointBelongSegment(p6) << "\n\n";

    cout << "Vector: " << v1;
    v1.Invert();
    cout << "\treverse vector: " << v1 << "\n\n";

    cout << "Side of vector: " << v1 << " for points: " << p3 << " and " << p4 << "\n";
    cout << v1.SideOfVectorForPoint(p3) << " " << v1.SideOfVectorForPoint(p4) << "\n\n";

    vector<Segment> v;
    v.push_back(s1);
    v.push_back(s2);
    v.push_back(s3);
    v.push_back(s4);
    cout << "Cross of segments:" << s1 << ", " << s2 << ", " << s3 << ", " << s4 << "\n\n";
    CrossSegments(v);
    cout << "\n\n";

    return 0;
}
