#include <iostream>
#include <cmath>
#include "Quaternion.h"

using namespace std;

//********************************************************************************************
//Class Quaternion
//********************************************************************************************

Quaternion:: Quaternion(const Quaternion& q)
{
	_a = q._a;
	_b = q._b;
	_c = q._c;
	_d = q._d;
}

Quaternion& Quaternion::operator=(const Quaternion& q)
{
	_a = q._a;
	_b = q._b;
	_c = q._c;
	_d = q._d;

	return *this;
}

Quaternion& Quaternion::operator+=(const Quaternion& q)
{
	_a += q._a;
	_b += q._b;
	_c += q._c;
	_d += q._d;

	return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& q)
{
	double a, b, c, d;
	a = _a*q._a - _b*q._b - _c*q._c - _d*q._d;
	b = _b*q._a + _a*q._b - _d*q._c + _c*q._d;
	c = _c*q._a + _d*q._b + _a*q._c - _b*q._d;
	d = _d*q._a - _c*q._b + _b*q._c - _a*q._d;
	_a = a;
	_b = b;
	_c = c;
	_d = d;

	return *this;
}

Quaternion& Quaternion::operator/=(Quaternion q)
{
	double mod = q._a*q._a + q._b*q._b + q._c*q._c + q._d*q._d;
	q.Interfaced();
	
	q._a /= mod;
	q._b /= mod;
	q._c /= mod;
	q._d /= mod;
	*this *= q;

	return *this;
}

bool Quaternion::operator==(const Quaternion& q) const
{
	if(_a == q._a && _b == q._b && _c == q._c && _d == q._d)
		return true;
	
	return false;
}

bool Quaternion::operator!=(const Quaternion& q) const
{
	if(!(*this == q))
		return true;
	
	return false;
}


Quaternion& Quaternion::Interfaced()
{
	_b *= -1;
	_c *= -1;
	_d *= -1;

	return *this;
}

double Quaternion::Mod()
{
	return sqrt(_a*_a + _b*_b + _c*_c + _d*_d);
}

Quaternion Quaternion::VectorProduct(const Quaternion& q)
{
	Quaternion buf1 = *this, buf2 = q;
	buf1.Interfaced();
	buf2.Interfaced();
	buf1 = buf2*(*this) + buf1*q;
	buf1._a /= 2;
	buf1._b /= 2;
	buf1._c /= 2;
	buf1._d /= 2;

	return buf1;
}

double Quaternion::ScalarProduct(const Quaternion& q)
{
	Quaternion buf1 = *this, buf2 = q;
	buf1.Interfaced();
	buf2.Interfaced();
	return (buf2*(*this) + buf1*q)._a / 2;
}


double Quaternion::GetA() const
{
	return _a;
}

double Quaternion::GetB() const
{
	return _b;
}

double Quaternion::GetC() const
{
	return _c;
}

double Quaternion::GetD() const
{
	return _d;
}

void Quaternion::SetA(double a)
{
	_a = a;
}

void Quaternion::SetB(double a)
{
	_b = a;
}

void Quaternion::SetC(double a)
{
	_c = a;
}

void Quaternion::SetD(double a)
{
	_d = a;
}


Quaternion operator+(const Quaternion& first, const Quaternion& second)
{
	Quaternion result(first);
	result += second;
	return result;
}

Quaternion operator*(const Quaternion& first, const Quaternion& second)
{
	Quaternion result(first);
	result *= second;
	return result;
}

Quaternion operator/(const Quaternion& first, const Quaternion& second)
{
	Quaternion result(first);
	result /= second;
	return result;
}

ostream& operator << (ostream &stream, const Quaternion& number)
{
	stream << "(" << number.GetA() << "; " << number.GetB() << "; " << number.GetC() << "; " << number.GetD() << ")";
	return stream;
}

istream& operator >> (istream &stream, Quaternion& number)		// (_a, _b, _c, _d)
{
	double buf_numb;
	char buf = 0;
	stream >> buf;
	while(buf != '(')
		stream >> buf;
	
	stream >> buf_numb ;
	number.SetA(buf_numb);
	stream >> buf;

	stream >> buf_numb ;
	number.SetB(buf_numb);
	stream >> buf;

	stream >> buf_numb ;
	number.SetC(buf_numb);
	stream >> buf;
	
	stream >> buf_numb ;
	number.SetD(buf_numb);
	stream >> buf;

	return stream;
}
