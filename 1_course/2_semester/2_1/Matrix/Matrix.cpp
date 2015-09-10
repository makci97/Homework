#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

using namespace std;

template <typename T>
class Matrix
{

	public:
		Matrix();
		Matrix(int a, int b);
		Matrix(const vector< vector<T> >& m);
		Matrix(const Matrix<T>& m);
		int GetA();
		int GetB();
		vector< vector<T> >& GetM();
		Matrix<T>& operator=(const Matrix<T>& m);
		Matrix<T>& operator+=(const Matrix<T>& m);
		Matrix<T>& operator-=(const Matrix<T>& m);
		Matrix<T>& operator*=(const Matrix<T>&  m);
		T Trace() const;
		Matrix<T>& Triangular();
		double Det() const;
		Matrix<T>& Transposing();
		Matrix<T>& Turn();

	private:
		int _a, _b;
		vector< vector<T> > _m; 
};

template <typename T>
void swap_vec(vector< vector<T> > v, int i, int j)
{
	int size = v[i].size();
	T buf;

	for(int ii = 0; ii < size; ++ii)
	{
		buf = v[i][ii];
		v[i][ii] = v[j][ii];
		v[j][ii] = buf;
	}

}

template <typename T>
Matrix<T>:: Matrix()
{
	_a = 0;
	_b = 0;
	_m.resize(0);
}

template <typename T>
Matrix<T>:: Matrix(int a, int b)
{
	_a = a;
	_b = b;

	for(int i = 0; i < _a; ++i)
		_m.push_back(vector<T> (_b,0));
}


template <typename T>
Matrix<T>:: Matrix(const vector< vector<T> >& m)
{
	int a = m.size();
	int b = m[0].size();
	
	for(int i = 0; i < a; ++i)
		if(b != m[i].size())
			throw logic_error("It isn't Matrix");

	_a = a;
	_b = b;

	for(int i = 0; i < _a; ++i)
		_m.push_back(m[i]);
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& m)
{
	_a = m._a;
	_b = m._b;
	_m.clear();

	for(int i = 0; i < _a; ++i)
		_m.push_back(m._m[i]);
}


template <typename T>
int Matrix<T>::GetA()
{
	return _a;
}

template <typename T>
int Matrix<T>::GetB()
{
	return _b;
}

template <typename T>
vector< vector<T> >& Matrix<T>::GetM()
{
	return _m;
}


template <typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& m)
{
	if(this == &m)
		return *this;

	_a = m._a;
	_b = m._b;
	 
	_m.clear();

	for(int i = 0; i < _a; ++i)
	{
		_m.push_back(m._m[i]);
	}

	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& m)
{
	if(_a != m._a || _b != m._b)
		throw logic_error("The sizes of matrixes don't coincide");

	for(int i = 0; i < _a; ++i)
		for(int j = 0; j < _b; ++j)
			_m[i][j] += m._m[i][j];

	return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& m)
{
	if(_a != m._a || _b != m._b)
		throw logic_error("The sizes of matrixes don't coincide");

	for(int i = 0; i < _a; ++i)
		for(int j = 0; j < _b; ++j)
			_m[i][j] -= m._m[i][j];

	return *this;
}


template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& m)
{
	if(_b != m._a)
		throw logic_error("The sizes of matrixes don't correspond");

	Matrix<T> result(_a, m._b);
	for(int i = 0; i < _a; ++i)
		for(int j = 0; j < _b; ++j)
			for(int k = 0; k < _b; ++k)
				result._m[i][j] += _m[i][k] * m._m[k][j];
	*this = result;
	return *this;
}


template <typename T>
T Matrix<T>::Trace() const
{
	int min = (_a < _b)? _a: _b;
	T result = 0;
	for(int i = 0; i < min; ++i)
		result += _m[i][i];
	return result;
}

template <typename T>
Matrix<T>& Matrix<T>::Triangular()
{
	Matrix<T> buf = *this;

	int i = 0;
	while(i < _a)
	{
		int j = i - 1;
		while(++j < _a && buf._m[j][i] == 0 );
		
		if(j < _a)
		{
			if(i != j)
				swap_vec<T>(buf._m, i, j);

			for(int ii = i + 1; ii < _a; ++ii)
			{
				for(int jj = i + 1; jj < _a; ++jj)
				{
					buf._m[ii][jj] -= buf._m[i][jj] * buf._m[ii][i] / buf._m[i][i];
				}

				buf._m[ii][i] = 0;
			}

		}

		++i;
	}

	*this = buf;
	return *this;
}

template <typename T>
double Matrix<T>::Det() const
{
	if(_a != _b)
		throw logic_error("The determinant isn't defined");

	Matrix<T> buf = *this;

	int i = 0;
	while(i < _a)
	{
		int j = i - 1;
		while(++j < _a && buf._m[j][i] == 0 );
		
		if(j < _a)
		{
			if(i != j)
				swap_vec<T>(buf._m, i, j);

			for(int ii = i + 1; ii < _a; ++ii)
			{
				for(int jj = _a - 1; jj >= i; --jj)
				{
					buf._m[ii][jj] -= buf._m[i][jj] * buf._m[ii][i] / buf._m[i][i];
				}
			}

		}
		else if(j >= _a)
			return 0;

		++i;
	}

	double result = 1;
	for(int i = 0; i < _a; ++i)
		result *= buf._m[i][i];

	return result;
}


template <typename T>
Matrix<T>& Matrix<T>::Transposing()
{
	Matrix<T> buf(_b, _a);

	for(int i = 0; i < _a; ++i)
	{
		for(int j = 0; j < _b; ++j)
		{
			buf._m[j][i] = _m[i][j];
		}
	}

	*this = buf;
	return *this;
}

template <typename T>
void PushStrokeForTurnMatrix(Matrix<T>& first, Matrix<T>& second)
{	
	int size = first._a;
	if(size != second._a || first._b != second._b || size != second._a)
		throw logic_error("Atata");

	int i = 0;
	while(i < size)
	{
		int j = i - 1;
		while(++j < size && first._m[j][i] == 0 );
		
		if(j < size)
		{
			if(i != j)
			{
				swap_vec<T>(first._m, i, j);
				swap_vec<T>(second._m, i, j);
			}

			for(int ii = size - 1; ii >= i; --ii)
			{
				second._m[i][ii] /= first._m[i][i];
				first._m[i][ii] /= first._m[i][i];
			}

			for(int ii = i + 1; ii < size; ++ii)
			{
				for(int jj = size - 1; jj >= i; --jj)
				{
					second._m[ii][jj] -= second._m[i][jj] * first._m[ii][i];
					first._m[ii][jj] -= first._m[i][jj] * first._m[ii][i];
				}
			}

		}

		++i;
	}
}

template <typename T>
Matrix<T>& Matrix<T>::Turn()
{
	if(_a != _b || this -> Det() == 0)
		throw logic_error("Atata");
	if(_a == 0)
		return *this;

	Matrix<T> buf = *this;
	Matrix<T> ed;
	ed._a = _a;
	ed._b = _b;
	
	vector<T> v(_a, 0);
	v[0] = 1;
	ed._m.push_back(v);

	for(int i = 1; i < _a; ++i)
	{
		v[i - 1] = 0;
		v[i] = 1;
		ed._m.push_back(v);
	}
	

	int i = 0;
	while(i < _a)
	{
		int j = i - 1;
		while(++j < _a && buf._m[j][i] == 0 );
		
		if(j < _a)
		{
			if(i != j)
			{
				swap_vec<T>(buf._m, i, j);
				swap_vec<T>(ed._m, i, j);
			}

			double buf1 = buf._m[i][i];
			for(int ii = _a - 1; ii >= 0; --ii)
			{
				ed._m[i][ii] /= buf1;
				buf._m[i][ii] /= buf1;
			}

			for(int ii = i + 1; ii < _a; ++ii)
			{
				buf1 = buf._m[ii][i];
				for(int jj = _a - 1; jj >= 0; --jj)
				{
					ed._m[ii][jj] -= ed._m[i][jj] * buf1;
					buf._m[ii][jj] -= buf._m[i][jj] * buf1;
				}
			}

		}

		++i;
	}

	i = _a - 1;
	while(i >= 0)
	{
		for(int ii = i - 1; ii >= 0; --ii)
		{
			double buf1 = buf._m[ii][i];
			for(int jj = 0; jj <= _a - 1; ++jj)
			{
				ed._m[ii][jj] -= ed._m[i][jj] * buf1;
				buf._m[ii][jj] -= buf._m[i][jj] * buf1;
			}
		}

		--i;
	}

	*this = ed;
	return *this;
}


template <typename T>
Matrix<T> operator+(const Matrix<T>& first, const Matrix<T>& second)
{
	Matrix<T> result(first);
	result += second;
	return result;
}

template <typename T>
Matrix<T> operator-(const Matrix<T>& first, const Matrix<T>& second)
{
	Matrix<T> result(first);
	result -= second;
	return result;
}

template <typename T>
Matrix<T> operator*(const Matrix<T>& first, const Matrix<T>& second)
{
	Matrix<T> result(first);
	result *= second;
	return result;
}


int main()
{
	srand (time(NULL));
	vector< vector<double> > d(3);
	int ii = 0;
	for(int i = 0; i < 3; ++i)
		for(int j = 0; j < 3; ++j)
			d[i].push_back(rand() % 10);

	Matrix<double> m(d), mm, mmm(2, 3);
	mm = m;
	mmm = mm * m;
	cout << m.Det();
	//m.Transposing();
	m.Turn();

	mmm = m * mm;
	return 0;
}
