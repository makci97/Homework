#pragma once
#include <iostream>

using namespace std;

class Quaternion
{
	public:
		Quaternion(): _a(0), _b(0), _c(0), _d(0) { }
		Quaternion(double a, double b, double c, double d): _a(a), _b(b), _c(c), _d(d) { }
                Quaternion(const Quaternion& q);
                Quaternion& operator=(const Quaternion& q);
                Quaternion& operator+=(const Quaternion& q);
                Quaternion& operator*=(const Quaternion& q);
                Quaternion& operator/=(Quaternion q);
                bool operator==(const Quaternion& q) const;
                bool operator!=(const Quaternion& q) const;
		Quaternion& Interfaced();
		double Mod();
                Quaternion VectorProduct(const Quaternion& q);
                double ScalarProduct(const Quaternion& q);
		double GetA() const;
		double GetB() const;
		double GetC() const;
		double GetD() const;
		void SetA(double a);
		void SetB(double a);
		void SetC(double a);
		void SetD(double a);
	private:
		double _a, _b, _c, _d;
};

Quaternion operator+(const Quaternion& first, const Quaternion& second);
Quaternion operator*(const Quaternion& first, const Quaternion& second);
Quaternion operator/(const Quaternion& first, const Quaternion& second);

ostream& operator << (ostream &stream, const Quaternion& number);
istream& operator >> (istream &stream, Quaternion& number);
