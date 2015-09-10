#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

const double E = 0.000001;
const int N = 10;

class BigInt
{
	public:
		BigInt()
		{
			_numb.resize(0);
			_numb.push_back(0);
			_znak = 1;
		}

		BigInt(int a)
		{
			_numb.resize(0);
			if(a < 0)
			{
				_znak = -1;
				a *= -1;
			}
			else
				_znak = 1;

			if(a == 0)
				_numb.push_back(a);
			else
			{
				while(a >= 1)
				{
					_numb.push_back(a%N);
					a /= N;
				}
			}
		}

		BigInt(const BigInt& a)
		{
			int a_size = a._numb.size();
			_numb.resize(a_size);
			_znak = a._znak;
			for(int i = 0; i < a_size; ++i)
			{
				_numb[i] = a._numb[i];
			}
		}

		/*vector<unsigned int> GetNumb() const
		{
			return _numb;
		}*/

		void ClearNumb()
		{
			_numb.resize(0);
			_znak = 1;
		}

		void PushBack(short int const &a)
		{
			_numb.push_back(a);
		}

		void PopBack()
		{
			if(this -> _numb.size() != 0)
				_numb.pop_back();
		}

		void Invert()
		{
			int size = _numb.size();

			for(int i = 0; 2 * i < size - 1; ++i)
			{
				swap(_numb[i], _numb[size - i - 1]);
			}
		}

		void SetNumb(BigInt const &a)
		{
			if(&a == this)
				return;
			int a_size = a._numb.size();
			_numb.resize(a_size);
			_znak = a._znak;
			for(int i = 0; i < a_size; ++i)
			{
				_numb[i] = a._numb[i];
			}
		}

		void SetZnak(short int const &a)
		{
			_znak = a;
		}

		short int GetZnak()const
		{
			return _znak;
		}

		int GetSize()const
		{
			return _numb.size();
		}

		int GetINumb(int i)const
		{
			if(i > int(_numb.size()))
				return (-1);

			return _numb[i];
		}

		BigInt max_BigInt(BigInt const &a, BigInt const &b)
		{
			if(a._znak > b._znak)
				return a;
			else if(b._znak > a._znak)
				return b;
			else
			{
				int size_a = a._numb.size(), size_b = b._numb.size();
				if(a._znak < 0)
					swap(size_a, size_b);

				if(size_a > size_b)
					return a;
				else if(size_a < size_b)
					return b;
				else
				{
					int i = 0;
					bool test = 0;
					while(i < size_a)
					{
						if(a._numb[i] < b._numb[i])
						{
							if(a._znak < 0)
								test = 1;
							break;
						}
						else if(a._numb[i] > b._numb[i])
						{
							if(a._znak > 0)
								test = 1;
							break;
						}
						++i;
					}
					return (test)? a: b;
				}

			}
			
		}

		BigInt& operator=(BigInt const &other)
		{
			if(this == &other)
				return *this;
			SetNumb(other);
			return *this;
		}

		bool operator==(BigInt const &other)const
		{
			if(_znak != other._znak)
				return false;

			int size = _numb.size();
			if(size != other._numb.size())
				return false;

			for(int i = 0; i < size; ++i)
			{
				if(_numb[i] != other._numb[i])
					return false;
			}

			return true;
		}

		bool operator<(BigInt const &other)const
		{
			if(_znak != other._znak)
			{
				if(_znak == -1)
					return true;
				else
					return false;
			}
			else
			{
				int size_this = _numb.size(), size_other = other._numb.size();

				if(size_this != size_other)
				{
					if(size_this <  size_other)
						return (_znak == -1)? false: true;
					else
						return (_znak == -1)? true: false;
				}
				else
				{
					while(--size_this >= 0)
					{
						if(_numb[size_this] != other._numb[size_this])
							return (_numb[size_this] < other._numb[size_this])? true: false;
					}

					return false;
				}
			}
		}

		bool operator<=(BigInt const &other)const
		{
			if(*this < other || *this == other)
				return true;
			else
				return false;
		}

		bool operator>(BigInt const &other)const
		{
			if(*this <= other)
				return false;
			else
				return true;
		}

		bool operator>=(BigInt const &other)const
		{
			if(*this < other)
				return false;
			else
				return true;
		}

		bool operator!=(BigInt const &other)const
		{
			return !(*this == other);
		}

		BigInt& operator+=(BigInt const &other)
		{
			int size_this = _numb.size(), size_other = other._numb.size(), size_max = max(size_this, size_other), size_min = min(size_this, size_other);

			if(other._znak != _znak)
			{
				if(_znak > other._znak)
				{
					BigInt buf(other);
					buf._znak = 1;
					return (*this -= buf);
				}
				else
				{
					BigInt buf1(other), buf2(*this);
					buf2._znak = 1;
					buf1 -= buf2;
					*this = buf1;
					return *this;
				}
			}
			else
			{
				int i = 0, buf = 0;
				while(i < size_min)
				{
					buf += other._numb[i] + _numb[i];
					_numb[i] = buf % N;
					buf /= N;
					++i;
				}

				if(i < size_max + 1)
				{
					if(i < size_this)
					{
						while(i < size_this)
						{
							buf += _numb[i];
							_numb[i] = buf % N;
							buf /= N;
							++i;
						}
					}
					else
					{
						while(i < size_other)
						{
							buf += other._numb[i];
							_numb.push_back(buf % N);
							buf /= N;
							++i;
						}
					}
					if(buf != 0)
						_numb.push_back(buf);
				}
				return *this;
			}
		}

		BigInt& operator-=(BigInt const &other)
		{
			if(*this == other)
			{
				_numb.resize(0);
				_numb.push_back(0);
				_znak = 1;
				return *this;
			}

			if(other._znak != _znak)
			{
				if(_znak > other._znak)
				{
					BigInt buf(other);
					buf._znak = 1;
					return (*this += buf);
				}
				else
				{
					BigInt buf1(other), buf2(*this);
					buf2._znak = 1;
					buf1 += buf2;
					*this = buf1;
					return *this;
				}
			}
			else
			{
				int size_this = _numb.size(), size_other = other._numb.size(), size_max = max(size_this, size_other), size_min = min(size_this, size_other), i = 0;
				
				if(max_BigInt(*this, other) == other)
				{
					 BigInt buf_BigInt(other);
					 buf_BigInt -= *this;
					 buf_BigInt._znak = -1;
					 *this = buf_BigInt;
					 return *this;
				}

				while(i < size_other)
				{
					_numb[i] -= other._numb[i];
					if(_numb[i] < 0)
					{
						_numb[i] += N;
						--_numb[i + 1];
					}
					++i;
				}
				while(i < size_this - 1 && _numb[i] < 0)
				{
					_numb[i] += N;
					--_numb[++i];
				}
				while(_numb[size_this - 1] == 0)
				{
					_numb.pop_back();
					--size_this;
				}
				return *this;
			}
		}

		BigInt CrutchForMultiplication(BigInt const &min, BigInt const &max, int size_min, int size_max)
		{
			BigInt buf, buf2 = min, result(0);
			int i = 0, j;

			buf2.Invert();
			while(i < size_max)
			{
				j = max._numb[i];

				if(j != 0)
				{
					if(i != 0)
					{
						buf2.PushBack(0);
					}
					buf = buf2;
					buf.Invert();
				}

				while(j-- > 0)
				{
					result += buf;
				}
				++i;
			}	
			
			if(min._znak != max._znak)
				result._znak = -1;
			else
				result._znak = 1;

			return result;
		}
		
		BigInt& operator*=(BigInt const &other)
		{
			int size_numb = _numb.size(), size_other = other._numb.size(), size_min = min(size_numb, size_other), size_max = max(size_numb, size_other);

			if(size_numb == size_min)
			{
				*this = CrutchForMultiplication(*this, other, size_min, size_max);
			}
			else
			{
				*this = CrutchForMultiplication(other, *this, size_min, size_max);
			}

			return *this;
		}

		bool CrutchForDivide(BigInt const &a, BigInt const &b, int size_a, int size_b)
		{

			if(size_a < size_b)
				return 1;
			else if(size_a > size_b)
				return 0;
			else
			{
				while(--size_a >= 0)
				{
					if(a._numb.at(size_a) < b._numb.at(size_a))
						return 1;
					else if(a._numb.at(size_a) > b._numb.at(size_a))
						return 0;
				}
				return 0;
			}
			
		}

		BigInt& operator/=(BigInt const &other)
		{
			if(other == 0)
			{
				cout << "Atata!!!";
				return *this;
			}

			int size_max = _numb.size(), size_min = other._numb.size();

			if( CrutchForDivide(*this, other, size_max, size_min))					
			{
				*this = 0;
				return *this;
			}
			else
			{
				int i = size_max - size_min;
				BigInt number = *this, buf, buf2 = other, result, sum_res;
				number._znak = 1;
				buf2._znak = 1;
				
				buf2.Invert();
				if(i != 0)
				{
					for(int j = 0; j <= i; ++j)
					{
						buf2.PushBack(0);
						sum_res.PushBack(0);
					}
				}
				sum_res.PopBack();
				sum_res.PushBack(1);

				while(number >= other && i >= 0)
				{
					buf = buf2;
					buf.Invert();
					while(number >= buf)
					{
						number -= buf;
						result += sum_res;
					}
					buf2.PopBack();
					sum_res.PopBack();
					sum_res.PopBack();
					sum_res.PushBack(1);
					--i;
				}
				int znak;
				if(_znak != other._znak)
					znak  = -1;
				else
					znak = 1;

				*this = result;
				this -> _znak = znak;

				return *this;
			}


		}

	private:
		vector<short int> _numb;
		short int _znak;									// '+' or '0' == '1', '-' == '-1'
};


ostream& operator << (ostream &stream, BigInt const &a)
{
	if(a.GetZnak() < 0)
		stream << "-";
	for(int i = a.GetSize() - 1; i >= 0; --i)
	{
		stream << a.GetINumb(i);
	}
	return stream;
}

istream& operator >> (istream &stream, BigInt &number)
{
	short int znak;
	char buf = ' ';
	while(buf == ' ')
		stream >> buf;


	if(buf == '-')
	{
		znak = -1;
	}
	else if(buf == '+')
	{
		znak = 1;
	}
	else if(buf  >= '0' && buf <= '9')
	{
		znak = 1;
	}
	else
	{
		return stream;
	}


	number.ClearNumb();
	number.SetZnak(znak);

	if(buf >= '0' && buf <= '9')
	{
		number.PushBack(buf - '0');
	}
	
	buf = stream.get();

	while(buf >= '0' && buf <= '9')
	{
		number.PushBack(buf - '0');
		buf = stream.get();
	}

	number.Invert();
	return stream;
	
}

BigInt operator+ (BigInt const &first, BigInt const &second)
{
	BigInt result(first);
	result += second;
	return result;
}

BigInt operator- (BigInt const &first, BigInt const &second)
{
	BigInt result(first);
	result -= second;
	return result;
}

BigInt operator* (BigInt const &first, BigInt const &second)
{
	BigInt result(first);
	result *= second;
	return result;
}

BigInt operator/ (BigInt const &first, BigInt const &second)
{
	BigInt result(first);
	result /= second;
	return result;
}

BigInt operator% (BigInt const &first, BigInt const &second)
{
	BigInt result(first);
	result /= second;
	result = first - result * second;
	return result;
}


int main()
{
	BigInt a, b;
	cin >> a >> b;
	a += b;
	cout << a;
	return 0;
}