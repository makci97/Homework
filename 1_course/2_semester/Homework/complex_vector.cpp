#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

const double E = 0.000001;

class Complex
{
	public:
		Complex(){}

		Complex(double re): re_(re), im_(0.0){}

		Complex(double re, double im): re_(re), im_(im){}

		Complex& operator+=(Complex const &other)
		{
			re_ += other.GetRe();
			im_ += other.GetIm();
			return *this;
		}

		Complex& operator-=(Complex const &other)
		{
			re_ -= other.GetRe();
			im_ -= other.GetIm();
			return *this;
		}

		Complex& operator*=(Complex const &other)
		{
			double buf_re = re_;
			re_ = other.GetRe()*re_ - other.GetIm()*im_;
			im_ = other.GetIm()*buf_re + other.GetRe()*im_;
			return *this;
		}

		Complex& operator=(Complex const &other)
		{
			if(this == &other)
				return *this;
			re_ = other.GetRe();
			im_ = other.GetIm();
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

bool operator==(Complex const &a, Complex const &b)
{
    return (abs(a.GetRe() - b.GetRe()) <= E) && (abs(a.GetIm() - b.GetIm()) <= E);
}

bool operator!=(Complex const &a, Complex const &b)
{
	return !(a == b);
}

Complex operator+(Complex const &first, Complex const &second)
{
	Complex result(first);
	result += second;
	return result;
}

Complex operator-(Complex const &first, Complex const &second)
{
	Complex result(first);
	result -= second;
	return result;
}


Complex operator*(Complex const &first, Complex const &second)
{
	Complex result(first);
	result *= second;
	return result;
}

ostream& operator << (ostream &stream, Complex const &number)
{
	stream << number.GetRe() << " + " << number.GetIm() << "i";
	return stream;
}

istream& operator >> (istream &stream, Complex &number)
{
	double buf_re, buf_im;
	char buf = 0;
	stream >> buf_re;
	while(buf != '+' && buf != '-')
		stream >> buf;

	stream >> buf_im ;
	if(buf == '-')
		buf_im *= -1;
	stream >> buf;

	number.SetRe(buf_re);
	number.SetIm(buf_im);
	return stream;
}

bool CheckSymmetry(const vector<Complex> &Array)
{
	int n = Array.size(), n_2 = (n%2 == 0)? n/2: (n+1)/2;
	for(int i = 0; i < n_2; ++i)
		if(Array[i] != Array[n-i-1])
			return 0;
	return 1;
}

int main()
{
	int n;
	cin >> n;
	vector<Complex> first_array(n);
	for(int i = 0; i < n; ++i)
	{
		cin >> first_array[i];
	}

	vector<Complex> second_array(n);

	for(int i = 0; i < n; ++i)
	{
		second_array[i] = first_array[n-i-1];
		cout << second_array[i] << " ";
	}
	cout << endl;

	vector<Complex> result_array(n);

	for(int i = 0; i < n; ++i)
	{
		result_array[i] = first_array[i] + second_array[i];
		cout << result_array[i] << " ";
	}
	cout << endl;

	cout << CheckSymmetry(result_array);

	return 0;
}
