#include <iostream>
#include <math.h>
using namespace std;


class Complex
{
	public:
		Complex(){}

		Complex(double re, double im): re_(re), im_(im){}

		Complex& operator+=(Complex &other)
		{
			re_ += other.GetRe();
			im_ += other.GetIm();
			return *this;
		}

		Complex& operator-=(Complex &other)
		{
			re_ -= other.GetRe();
			im_ -= other.GetIm();
			return *this;
		}

		Complex operator*=(Complex &other)
		{
			double buf_re = re_;
			re_ = other.GetRe()*re_ - other.GetIm()*im_;
			im_ = other.GetIm()*buf_re + other.GetRe()*im_;
			return *this;
		}
		
		double GetRe() const
		{
			return re_;
		}

		void SetRe(double re)
		{
			re_ = re;
		}

		double GetIm() const
		{
			return im_;
		}

		void SetIm(double im)
		{
			im_ = im;
		}

		double Moudle() const
		{
			return sqrt(re_*re_ + im_*im_);
		}

	private:
		double re_, im_;
};

Complex operator+(Complex &first, Complex &second)
{
	Complex result(first);
	result += second;
	return result;
}

Complex operator-(Complex &first, Complex &second)
{
	Complex result(first);
	result -= second;
	return result;
}


Complex operator*(Complex &first, Complex &second)
{
	Complex result(first);
	result *= second;
	return result;
}

ostream& operator << (ostream &stream, Complex &number)
{
	stream << number.GetRe() << " + " << number.GetIm() << "i";
	return stream;
}


int main()
{
	Complex a(1, 2), b(3, 4), c;
	c = a + b;
	cout << c << endl;

	c = a - b;
	cout << c << endl;

	c = a * b;
	cout << c << endl;

	return 0;
}