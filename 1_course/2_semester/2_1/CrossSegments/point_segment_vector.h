#pragma once
#include <iostream>

using namespace std;

class IntersectionIsEmpty { };
//**********************************************************************************************************
class Point
{
	public:
		Point(){}

		Point(double x, double y): _x(x), _y(y){}
		
		Point(const Point& p);

		double GetX() const;

		double GetY() const;
		
		void SetX(const double& x);

		void SetY(const double& y);

                Point& operator=(const Point& p);

                Point& operator+=(const Point& p);

                Point& operator-=(const Point& p);

	private:
		double _x, _y;
};

bool operator==(const Point& first, const Point& second);

Point operator+(const Point& first, const Point& second);

Point operator-(const Point& first, const Point& second);

void swap(Point& first, Point& second);

double DistanceBetweenPoints(const Point& first, const Point& second);

ostream& operator << (ostream &stream, const Point& point);

//**********************************************************************************************************
class Vector;

class Segment
{
	public:
		Segment(){}

                Segment(const Point& p1, const Point& p2): _p1(p1), _p2(p2){}

		Segment(double x1, double y1, double x2, double y2);

		Segment(const Segment& s);
		
		Point GetFirstPoint() const;

		Point GetSecondPoint() const;
		
		void SetFirstPoint(const Point& p);

		void SetSecondPoint(const Point& p);

		bool IntersectionOfSegment(const Segment& s) const;

		Point PointOfIntersectionOfSegments(const Segment& s) const;	//this method have exeption

                Vector GetVector();

		bool PointBelongSegment(const Point& p) const;			//the point belongs to the segment?

                Segment& operator=(const Segment& s);

	private:
		Point _p1, _p2;
};

ostream& operator << (ostream &stream, const Segment& segment);

//**********************************************************************************************************
class Vector
{
	public:
		Vector(){}
		
                Vector(const Segment& s): _vec(s){}

                Vector(const Point& p1, const Point& p2);

		Vector(const Vector& v): _vec(v._vec){}

		Vector(double x1, double y1, double x2, double y2);

		double GetX() const;

		double GetY() const;

                Segment& GetVec();

                void SetVec(const Segment& s);

		double VectorProduct(const Vector& v) const;

		double ScalarProduct(const Vector& v) const;

		void Invert();

		bool SideOfVectorForPoint(const Point& p) const;				//true is right or point belongs to the straight line, false is left

		bool IntersectionOfVector(const Vector& v) const;

                Vector& operator=(const Vector& v);

	private:
		Segment _vec;				// Second point of segment is end of vector
};

ostream& operator << (ostream &stream, const Vector& vector);
