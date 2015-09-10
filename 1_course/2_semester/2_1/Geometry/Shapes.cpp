#include "Shapes.h"
#include "point_segment_vector.h"
#include <iostream>
#include <vector>
#include <new>
#include <cstdlib>
#include <cmath>

using namespace std;

//**********************************************************************************************************
//CLASS Circle
//**********************************************************************************************************

double Circle::Area()const
{
	return 3.14159265358 * _radius * _radius;
}

bool Circle::PointOnBorder(const Point& point) const
{
	Point p = _center;
        return (abs(DistanceBetweenPoints(point, p) - _radius) <= 0.000000001)? true: false;
}

bool Circle::PointInShape(const Point& point)const
{
        return (DistanceBetweenPoints(point,_center) - _radius) <= 0.000000001? true: false;
}

Point Circle::PointOfIntersectionWithSegment(const Segment& segment) const
{
        Point p1 = segment.GetFirstPoint(), p2 = segment.GetSecondPoint(), p = _center, result;
        p1 -= p;
	p2 -= p;
	double k = p2.GetX() - p1.GetX(), b = p2.GetY() - p1.GetY();
        if(k != 0)
	{
                k = b/k;
		b = p1.GetY() - k*p1.GetX();
                double buf1 = -k*b/(k*k + 1), buf2 = _radius*_radius * (k*k + 1) - b*b;
                if(buf2 < 0)
                    throw IntersectionIsEmpty();

                buf2 = ( sqrt(buf2) ) / (k*k + 1);

                double x = buf1 + buf2, y = k*x + b;
                result.SetX(x);
                result.SetY(y);
                result += p;
                if( segment.PointBelongSegment(result) )
                {
                        return result;
                }

		x = buf1 - buf2;
		y = k*x + b;
                result.SetX(x);
                result.SetY(y);
                result += p;
                if( segment.PointBelongSegment(result) )
                {
			return result;
		}
		else
			throw IntersectionIsEmpty();
	}
	else
	{
		double x = p1.GetX(), y =_radius*_radius - x*x;
		if(y >= 0)
		{
			y = sqrt(y);
			result.SetX(x);
			result.SetY(y);
			result += p;
			return result;
		}
		else
			throw IntersectionIsEmpty();
	}
}

//**********************************************************************************************************
//CLASS Polygon
//**********************************************************************************************************

Polygon::Polygon(vector<Point>& points)
{
	_n = points.size();

	for(int i = 0; i < _n; ++i)
	{
		_points.push_back(points[i]);
	}
}

Polygon::Polygon(const Polygon& p)
{
	_n = p._points.size();

	for(int i = 0; i < _n; ++i)
	{
		_points.push_back(p._points[i]);
	}
}

/*vector <Point>& Polygon::ReturnVector()
{
	return _points;
}*/

double Polygon::Area() const
{
	double result = 0, t, c = 0, y , buf;
        Point p_zero(0,0);
	Vector buf1, buf2;
	for(int i = 0; i < _n; ++i)
	{
		if(i != _n - 1)
                {
                        buf1.GetVec().SetFirstPoint(p_zero);
                        buf1.GetVec().SetSecondPoint(_points[i]);
                        buf2.GetVec().SetFirstPoint(p_zero);
                        buf2.GetVec().SetSecondPoint(_points[i+1]);
		}
		else
                {
                        buf1.GetVec().SetFirstPoint(p_zero);
                        buf1.GetVec().SetSecondPoint(_points[_n-1]);
                        buf2.GetVec().SetFirstPoint(p_zero);
                        buf2.GetVec().SetSecondPoint(_points[0]);
		}
       	buf = buf1.VectorProduct(buf2);
		y = buf - c;
        t = result + y;
        c = (t - result) - y;
        result = t;
	}
    result -= c;
    return abs(result / 2);
}

bool Polygon::PointOnBorder(const Point& point) const
{
        Segment buf;

                for(int i = 0; i < _n - 1; ++i)
                {
                        buf.SetFirstPoint(_points[i]);
                        buf.SetSecondPoint(_points[i + 1]);
                        if(buf.PointBelongSegment(point))
                                return true;
                }
                buf.SetFirstPoint(_points[_n - 1]);
                buf.SetSecondPoint(_points[0]);
                if(buf.PointBelongSegment(point))
                        return true;
                return false;
}

bool Polygon::PointInShape(const Point& point) const
{
        Vector buf;

                for(int i = 0; i < _n - 1; ++i)
                {
                        buf.GetVec().SetFirstPoint(_points[i]);
                        buf.GetVec().SetSecondPoint(_points[i + 1]);

                        if(!buf.SideOfVectorForPoint(point))
                                return false;
                }
                buf.GetVec().SetFirstPoint(_points[_n - 1]);
                buf.GetVec().SetSecondPoint(_points[0]);
                if(!buf.SideOfVectorForPoint(point))
                        return false;
                return true;
}

Point Polygon::PointOfIntersectionWithSegment(const Segment& segment) const	//this method have exeption
{
         Segment buf;
                for(int i = 0; i < _n - 1; ++i)
                {
                        buf.SetFirstPoint(_points[i]);
                        buf.SetSecondPoint(_points[i + 1]);
                        try
                        {
                                return buf.PointOfIntersectionOfSegments(segment);
                        }
                        catch(...) { }
                }

                buf.SetFirstPoint(_points[_n - 1]);
                buf.SetSecondPoint(_points[0]);
                try
                {
                        return buf.PointOfIntersectionOfSegments(segment);
                }
                catch(...)
                {
                        throw IntersectionIsEmpty();
                }
}


bool Polygon::PolygonIsConvex() const
{
	for(int i = 0; i < _n - 1; ++i)
	{
		Vector v(_points[i], _points[i + 1]);

		for(int j = 0; j < _n; ++j)
		{
			if(j != i && j != i + 1)
			{
                                static bool check = v.SideOfVectorForPoint(_points[j]);
                                if(check != v.SideOfVectorForPoint(_points[j]))
					return false;
			}
		}
	}
	
	Vector v(_points[_n - 1], _points[0]);

	for(int j = 1; j < _n - 1; ++j)
		{
                        static bool check = v.SideOfVectorForPoint(_points[j]);
                                if(check != v.SideOfVectorForPoint(_points[j]))
					return false;
		}

	return true;
}

bool Polygon::PolygonIsRectangle() const
{
        Vector buf1, buf2;
	if( !(this -> PolygonIsConvex()) || _n != 4)
		return false;
	for(int i = 0; i < 4; ++i)
	{
		for(int j = i + 1; j < 4; ++j)
		{
			if(_points[i] == _points[j])
				return false;
		}
		if(i == 3)
                {
                        buf1.GetVec().SetFirstPoint(_points[3]);
                        buf1.GetVec().SetSecondPoint(_points[0]);
                        buf2.GetVec().SetFirstPoint(_points[0]);
                        buf2.GetVec().SetSecondPoint(_points[1]);
                        if(abs( buf1.ScalarProduct(buf2) ) >= 0.000000001)
				return false;
		}
		else if(i == 2)
                {
                        buf1.GetVec().SetFirstPoint(_points[3]);
                        buf1.GetVec().SetSecondPoint(_points[0]);
                        buf2.GetVec().SetFirstPoint(_points[2]);
                        buf2.GetVec().SetSecondPoint(_points[3]);
                        if(abs( buf1.ScalarProduct(buf2) ) >= 0.000000001)
				return false;
                }
		else
                {
                        buf1.GetVec().SetFirstPoint(_points[i]);
                        buf1.GetVec().SetSecondPoint(_points[i + 1]);
                        buf2.GetVec().SetFirstPoint(_points[i + 1]);
                        buf2.GetVec().SetSecondPoint(_points[i + 2]);
                        if(abs( buf1.ScalarProduct(buf2) ) >= 0.000000001)
				return false;
                }
	}

	return true;
}


bool Polygon::PolygonIsSquare() const
{
	if( !(this -> PolygonIsRectangle()))
		return false;
        double dist = DistanceBetweenPoints(_points[0], _points[3]);
	for(int i = 0; i < 3; ++i)
	{
                if(abs(dist - DistanceBetweenPoints(_points[i], _points[i + 1]) ) >= 0.000000001)
			return false;
	}

	return true;
}

bool Polygon::PolygonIsTriangle() const
{
        Vector buf1, buf2;
	if(_n != 3)
		return false;

        buf1.GetVec().SetFirstPoint(_points[0]);
        buf1.GetVec().SetSecondPoint(_points[1]);
        buf2.GetVec().SetFirstPoint(_points[1]);
        buf2.GetVec().SetSecondPoint(_points[2]);
        if(abs( buf1.VectorProduct(buf2) ) <= 0.000000001)
		return false;

        buf1.GetVec().SetFirstPoint(_points[2]);
        buf1.GetVec().SetSecondPoint(_points[0]);
        if(abs( buf1.VectorProduct(buf2) ) <= 0.000000001)
		return false;

        buf2.GetVec().SetFirstPoint(_points[0]);
        buf2.GetVec().SetSecondPoint(_points[1]);
        if(abs( buf1.VectorProduct(buf2) ) <= 0.000000001)
		return false;

	return true;
}


Polygon& Polygon::operator=(const Polygon& other)
{
    if(this != &other)
    {
        _n = other._n;
        this -> _points.resize(_n);
        for(int i = _n - 1; i >= 0; --i)
        {
            _points[i] = other._points[i];
        }
    }

    return *this;
}

//**********************************************************************************************************
//CLASS Convex_Polygon
//**********************************************************************************************************

Convex_Polygon::Convex_Polygon(vector<Point>& points)
{
	Point min = points[0];
	int min_y = min.GetY(), size = points.size();

	_n = 0;

	for(int i = 1; i < size; ++i)
	{
		if(min_y > points[i].GetY())
		{
			min = points[i];
			min_y = points[i].GetY();
		}
		else if(min_y == points[i].GetY())
		{
			if(min.GetX() > points[i].GetX())
				min = points[i];
		}
	}

	Point buf = min;
        Vector buf_v, kal;
	int buf_i;
	if(size > 1)
	{
		do
		{
			_points.push_back(buf);
			++_n;
			do
			{
				buf_i = rand() % size;
			}
			while(points[buf_i] == buf);

                        buf_v.GetVec().SetFirstPoint(buf);
                        buf_v.GetVec().SetSecondPoint(points[buf_i]);
			for(int i = 0; i < size; ++i)
			{
                                if(i != buf_i && !buf_v.SideOfVectorForPoint(points[i]) )
				{
                                        kal.GetVec().SetFirstPoint(buf);
                                        kal.GetVec().SetSecondPoint(points[i]);
                                        if(buf_v.VectorProduct(kal) == 0 && DistanceBetweenPoints(buf, points[buf_i]) > DistanceBetweenPoints(buf, points[i])) { }
					else
                                        {
                                                buf_i = i;
                                                buf_v.GetVec().SetFirstPoint(buf);
                                                buf_v.GetVec().SetSecondPoint(points[buf_i]);
                                        }
				}
			}

			buf = points[buf_i];
		}
                while(!(buf == min));
	}
}

Convex_Polygon::Convex_Polygon(const Polygon& p)
{
	if(p.PolygonIsConvex())
                *this = p;
        else
                throw bad_alloc();
}

//**********************************************************************************************************
//CLASS Rectangle
//**********************************************************************************************************

Rectangle::Rectangle(vector<Point>& points)
{
	Polygon p(points);

	if(p.PolygonIsRectangle())
	{
		vector<Point> _points(points);
		_n = points.size();
	}
}

Rectangle::Rectangle(const Polygon& p)
{
	if(p.PolygonIsRectangle())
                *this = p;
        else
                throw bad_alloc();
}

//**********************************************************************************************************
//CLASS Square
//**********************************************************************************************************

Square::Square(vector<Point>& points)
{
	Polygon p(points);

	if(p.PolygonIsSquare())
	{
		vector<Point> _points(points);
		_n = points.size();
	}
}

Square::Square(const Polygon& p)
{
	if(p.PolygonIsSquare())
                *this = p;
        else
                throw bad_alloc();
}

//**********************************************************************************************************
//CLASS Triangle
//**********************************************************************************************************

Triangle::Triangle(vector<Point>& points)
{
	Polygon p(points);

	if(p.PolygonIsTriangle())
	{
		vector<Point> _points(points);
		_n = points.size();
	}
}

Triangle::Triangle(const Polygon& p)
{
	if(p.PolygonIsTriangle())
                *this = p;
        else
                throw bad_alloc();
}
