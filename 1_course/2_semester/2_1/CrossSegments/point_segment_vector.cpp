#include <iostream>
#include <cmath>
#include "point_segment_vector.h"

using namespace std;


//**********************************************************************************************************
//CLASS Point
//**********************************************************************************************************
Point::Point(const Point& p)
{
        _x = p._x;
        _y = p._y;
}

double Point::GetX() const
{ 
	return _x;
}

double Point::GetY()const
{
	return _y;
}
		
void Point::SetX(const double& x)
{
	_x = x;
}

void Point::SetY(const double& y)
{
	_y = y;
}

Point& Point::operator=(const Point& p)
{
	if(this != &p)
	{
		_x = p._x;
		_y = p._y;
	}
	return *this;
}

Point& Point::operator+=(const Point& p)
{
	_x += p._x;
	_y += p._y;
	return *this;
}

Point& Point::operator-=(const Point& p)
{
	_x -= p._x;
	_y -= p._y;
	return *this;
}

//**********************************************************************************************************

bool operator==(const Point& first, const Point& second)
{
	if(abs(first.GetX() - second.GetX()) <= 0.0000001 && abs(first.GetY() - second.GetY()) <= 0.0000001)
		return true;
	return false;
}

Point operator+(const Point& first, const Point& second)
{
	Point result = first;
	result += second;
	return result;
}

Point operator-(const Point& first, const Point& second)
{
	Point result = first;
	result -= second;
	return result;
}

void swap(Point& first, Point& second)
{
	Point buf = first;
	first = second;
	second = buf;
}

double DistanceBetweenPoints(const Point& first, const Point& second)
{
	return sqrt(pow(first.GetX() - second.GetX(), 2) + pow(first.GetY() - second.GetY(), 2) );
}

ostream& operator << (ostream &stream, const Point& point)
{
	stream << "("<< point.GetX()<< "; "<< point.GetY()<< ")";
	return stream;
}

//**********************************************************************************************************
//CLASS Segment
//**********************************************************************************************************

Segment::Segment(double x1, double y1, double x2, double y2)
{
	_p1.SetX(x1);
	_p1.SetY(y1);
	_p2.SetX(x2);
        _p2.SetY(y2);
}

Segment::Segment(const Segment& s)
{
	_p1 = s.GetFirstPoint();
	_p2 = s.GetSecondPoint();
}

Point Segment::GetFirstPoint()const
{
	return _p1; 
}

Point Segment::GetSecondPoint()const
{
	return _p2; 
}
		
void Segment::SetFirstPoint(const Point& p)
{
	_p1 = p; 
}

void Segment::SetSecondPoint(const Point& p)
{
	_p2 = p; 
}

bool Segment::IntersectionOfSegment(const Segment& s) const
{
	double x1 = min(_p1.GetX(), _p2.GetX()), x2 = max(_p1.GetX(), _p2.GetX()), y1 = min(_p1.GetY(), _p2.GetY()), y2 = max(_p1.GetY(), _p2.GetY());
	double x3 = min(s._p1.GetX(), s._p2.GetX()), x4 = max(s._p1.GetX(), s._p2.GetX()), y3 = min(s._p1.GetY(), s._p2.GetY()), y4 = max(s._p1.GetY(), s._p2.GetY()); 
	if( !( (x2 >= x3) && (x4 >= x1) && (y2 >= y3) && (y4 >= y1) ) )
		return false;
			
	Vector buf1(_p1, s._p1), buf2(_p1, _p2), buf3(_p1, s._p2);
        if( !( buf1.VectorProduct(buf2) * buf3.VectorProduct(buf2) <= 0) )  // or <=
		return false;
			
	Vector buff1(s._p1, _p1), buff2(s._p1, s._p2), buff3(s._p1, _p2);
        if( !( buff1.VectorProduct(buff2) * buff3.VectorProduct(buff2) <= 0) )  // or <=
		return false;

	return true;
}

Point Segment::PointOfIntersectionOfSegments(const Segment& s) const
{
	if(this -> IntersectionOfSegment(s))
	{
		Point p11 = this -> GetFirstPoint(), p12 = this -> GetSecondPoint(), result;
		double k1 = p12.GetX() - p11.GetX(), b1 = p12.GetY() - p11.GetY();
		Point p21 = s.GetFirstPoint(), p22 = s.GetSecondPoint();
		double k2 = p22.GetX() - p21.GetX(), b2 = p22.GetY() - p21.GetY();
		
		if(abs(k1) >= 0.000000001 && abs(k2) >= 0.000000001)
		{
			k1 = b1/k1;
			b1 = p11.GetY() - k1*p11.GetX();
			k2 = b2/k2;
			b2 = p21.GetY() - k2*p21.GetX();

			if(abs(k1 - k2) <= 0.000000001)
			{
				double x1_max = max(p11.GetX(), p12.GetX()), x2_max = max(p21.GetX(), p22.GetX()), x_max = max(x1_max, x2_max);
				double x1_min = min(p11.GetX(), p12.GetX()), x2_min = min(p21.GetX(), p22.GetX()), x_min = min(x1_min, x2_min);
			
				if(abs(x1_max - x_max) <= 0.000000001)
				{
					if(abs(x1_min - x_min) >= 0.000000001)
					{
						result.SetX(x1_min);
						result.SetY(x1_min*k1 + b1);
					}
					else
						result = p21;
				}

				else
				{
					if(abs(x2_min - x_min) >= 0.000000001)
					{
						result.SetX(x2_min);
						result.SetY(x2_min*k2 + b2);
					}
					else
						result = p11;
				}
				return result;
			}

			double x = (b2 - b1)/(k1 - k2);
			result.SetX(x);
			result.SetY(k1*x + b1);
			return result;
		}

		else if((abs(k1) <= 0.000000001 || abs(k2) <= 0.000000001) && abs(k1 - k2) >= 0.000000001)
		{
			if(k1 == 0)
			{
				k2 = b2/k2;
				b2 = p21.GetY() - k2*p21.GetX();
                                double x = p11.GetX();
                                result.SetX(x);
                                result.SetY(k2*x + b2);
			}
			else
			{
				k1 = b1/k1;
				b1 = p11.GetY() - k1*p11.GetX();
                                double x = p21.GetX();
                                result.SetX(x);
                                result.SetY(k1*x + b1);
			}
			return result;
		}

		else
		{
			double y1_max = max(p11.GetY(), p12.GetY()), y2_max = max(p21.GetY(), p22.GetY()), y_max = max(y1_max, y2_max);
			double y1_min = min(p11.GetY(), p12.GetY()), y2_min = min(p21.GetY(), p22.GetY()), y_min = min(y1_min, y2_min);
			
			if(abs(y1_max - y_max) <= 0.000000001)
			{
				if(abs(y1_min - y_min) >= 0.000000001)
				{
					result.SetX(p11.GetX());
					result.SetY(y1_min);
				}
				else
					result = p21;
			}

			else
			{
				if(abs(y2_min - y_min) >= 0.000000001)
				{
					result.SetX(p21.GetX());
					result.SetY(y2_min);
				}
				else
					result = p11;
			}
			return result;
		}
	}

	else
		throw IntersectionIsEmpty();
}

Segment& Segment::operator=(const Segment& s)
{
	if(this != &s)
	{
		_p1 = s._p1;
		_p2 = s._p2;
	}
	return *this;
}

Vector Segment::GetVector()
{
	Vector result;
        result.SetVec(*this);
	return result;
}

bool Segment::PointBelongSegment(const Point& p) const				//the point belongs to the segment?
{
	return (this -> IntersectionOfSegment(Segment(p, p)));
}

ostream& operator << (ostream &stream, const Segment& segment)
{
	stream << "( "<< segment.GetFirstPoint()<< "; "<< segment.GetSecondPoint()<< "}";
	return stream;
}

//**********************************************************************************************************
//CLASS Vector
//**********************************************************************************************************
Vector::Vector(const Point& p1, const Point& p2)
{
        _vec.SetFirstPoint(p1);
        _vec.SetSecondPoint(p2);
}

Vector::Vector(double x1, double y1, double x2, double y2)
{
	_vec.GetFirstPoint().SetX(x1);
	_vec.GetFirstPoint().SetY(y1);
	_vec.GetSecondPoint().SetX(x2);
	_vec.GetSecondPoint().SetY(y2);
}

double Vector::GetX() const
{
	return (_vec.GetSecondPoint().GetX() - _vec.GetFirstPoint().GetX());
}

double Vector::GetY() const
{
	return (_vec.GetSecondPoint().GetY() - _vec.GetFirstPoint().GetY());
}

Segment& Vector::GetVec()
{
	return _vec;
}

void Vector::SetVec(const Segment& s)
{
    _vec = s;
}

double Vector::VectorProduct(const Vector& v) const
{
        //double b = this -> Vector::GetX();
	return (this -> Vector::GetX() * v.Vector::GetY() - this -> Vector::GetY() * v.Vector::GetX());
}

double Vector::ScalarProduct(const Vector& v) const
{
        //double b = this -> Vector::GetX();
	return (this -> Vector::GetX() * v.Vector::GetX() + this -> Vector::GetY() * v.Vector::GetY());
}

void Vector::Invert()
{
	Point buf = _vec.GetFirstPoint();
	_vec.SetFirstPoint(_vec.GetSecondPoint());
	_vec.SetSecondPoint(buf);
}

bool Vector::SideOfVectorForPoint(const Point& p) const					//true is right or point belongs to the straight line, false is left
{
	Vector buf(*this);
	buf._vec.SetSecondPoint(p);
	if(this -> VectorProduct(buf) <= 0)
		return true;
	return false;
}

bool Vector::IntersectionOfVector(const Vector& v) const
{
	return (_vec.IntersectionOfSegment(v._vec));
}

Vector& Vector::operator=(const Vector& v)
{
	if(this != &v)
	{
		_vec = v._vec;
	}	
	return *this;
}

ostream& operator << (ostream &stream, const Vector& vector)
{
        Vector v = vector;
        stream << v.GetVec();
        return stream;
}
