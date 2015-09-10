#pragma once
#include <iostream>
#include <vector>
#include "point_segment_vector.h"

using namespace std;

class Shape
{
	public:
		virtual double Area() const = 0;
		virtual bool PointOnBorder(const Point& point) const = 0;
		virtual bool PointInShape(const Point& point) const = 0;
		virtual Point PointOfIntersectionWithSegment(const Segment& segment) const = 0;	//this method have exeption
};

class Circle: public Shape
{
	public:
		Circle() { }
		Circle(Point center, double radius): _center(center), _radius(radius) { }
		Circle(double x, double y, double radius): _center(Point(x, y)), _radius(radius) { }
		virtual double Area() const;
		virtual bool PointOnBorder(const Point& point) const;
		virtual bool PointInShape(const Point& point) const;
		virtual Point PointOfIntersectionWithSegment(const Segment& segment) const;	//this method have exeption
	private:
		Point _center;
		double _radius;
};

class Polygon: public Shape 
{
	public:
		Polygon() { }
		Polygon(vector<Point>& points);
		Polygon(const Polygon& p);
		//vector <Point>& ReturnVector();
		virtual double Area() const;
		virtual bool PointOnBorder(const Point& point) const;
		virtual bool PointInShape(const Point& point) const;
		virtual Point PointOfIntersectionWithSegment(const Segment& segment) const;	//this method have exeption
		bool PolygonIsConvex() const;
		bool PolygonIsRectangle() const;
		bool PolygonIsSquare() const;
                bool PolygonIsTriangle() const;
                Polygon& operator=(const Polygon& other);
	protected:
		int _n;
		vector <Point> _points;
};

class Convex_Polygon: public Polygon
{
	public:
		Convex_Polygon() {}
		Convex_Polygon(vector<Point>& points);
                Convex_Polygon(const Polygon& p);
};

class Rectangle: public Polygon
{
	public:		
		Rectangle() {}
		Rectangle(vector<Point>& points);
                Rectangle(const Polygon& p);
};

class Square: public Polygon
{
	public:
		Square() {}
		Square(vector<Point>& points);
                Square(const Polygon& p);
};

class Triangle: public Polygon
{
	public:
		Triangle() {}
		Triangle(vector<Point>& points);
                Triangle(const Polygon& p);
};
