#include <iostream>
#include <vector>
#include <stdexcept>
#include "String.h"

using namespace std;

//*****************************************************************************
//Class String
//*****************************************************************************

String::String(char* const s)
{
	long int i = 0;

	while(s[i] != '\0' && s[i] != '\n')
		_s.push_back(s[i++]);
}

String::String(const String& s)
{
	long int size = s._s.size();

	for(long int i = 0; i < size; ++i)
		_s.push_back(s._s[i]);
}

char& String::operator[](long int i)
{
	return _s[i];
}

String& String::operator=(const String& s)
{
	_s.clear();
	
	long int size = s._s.size();
	for(long int i = 0; i < size; ++i)
		_s.push_back(s._s[i]);

	return *this; 
}

bool String::operator==(const String& s) const
{
	if(this == &s)
		return true;

	long int size = _s.size();
	
	if(size != s._s.size())
		return false;
	
	while(--size >= 0)
		if(_s[size] != s._s[size])
			return false;

	return true;
}

bool String::operator!=(const String& s) const
{
	return !(*this == s);
}

bool String::operator<(const String& s) const
{
	if(*this == s)
		return false;

	long int size_this = _s.size(), size_s = s._s.size();

	if(size_this < size_s)
		return true;
	else if(size_this > size_s)
		return false;
	
	for(long int i = 0; i < size_this; ++i)
	{
		if(_s[i] < s._s[i])
			return true;
		else if(_s[i] > s._s[i])
			return false;
	}
		
	return false;
}

bool String::operator<=(const String& s) const
{
	if(*this < s || *this == s)
		return true;
	return false;
}

bool String::operator>(const String& s) const
{
	if(*this <= s)
		return false;
	return true;
}

bool String::operator>=(const String& s) const
{
	if(*this < s)
		return false;
	return true;
}

String& String::operator+=(const String& s)
{
	long int size = s._s.size();
	for(long int i = 0; i < size; ++i)
		_s.push_back(s._s[i]);

	return *this;
}

long int String::Size() const
{
	return _s.size();
}

String& String::Resize(long int size)
{
	_s.resize(size);
	return *this;
}

String& String::PushBack(char const c)
{
	_s.push_back(c);
	return *this;
}

String& String::InsertAfterIElement(long int i, String s)
{
	long int size_this = _s.size();
	--i;

	if(i > size_this - 1)
		i = size_this - 1;
	if(i < 0)
		i = -1;

	long int size_s = s._s.size(), size = size_this + size_s;
	_s.resize(size);

	for(long int j = size_this - 1; j > i; --j)
	{
		_s[j + size_s] = _s[j];
	}

	for(long int j = i + 1; j < i + size_s + 1; ++j)
	{
		_s[j] = s._s[j - i - 1];
	}

	return *this;

}

String& String::RemoveFromItoJ(long int i, long int j)
{
	long int size = _s.size();
	--i;
	--j;

	if(i > size - 1)
		throw logic_error("First index > size - 1");
	else if(j <= i)
		throw logic_error("First index =< second index");
	else if(i <= 0)
		throw logic_error("First index <= 0");

	if(j > size - 1)
	{
		_s.resize(i);
		return *this;
	}

	for(long int k = 0; k < size - j; ++k)
	{
		_s[i + k] = _s[j + k];
	}
	
	_s.resize(size - j + i);
	return *this;
}

String& String::InsertAfterLastElement(String s)
{
	long int size = s._s.size();

	for(long int i = 0; i < size; ++i)
		_s.push_back(s._s[i]);

	return *this;
}

String& String::DeleteLastElement()
{
	_s.size();
	_s.resize(_s.size() - 1);
	_s.size();
	return *this;
}

String operator+(const String& first, const String& second)
{
	String result(first);
	result += second;
	return result;
}

ostream& operator << (ostream &stream, String &s)
{
	long int size = s.Size();
	for(long int i = 0; i < size; ++i)
		stream << s[i];
	return stream;
}

istream& operator >> (istream &stream, String &s)
{
	char buf = 0;
	s.Resize(0);
	buf = stream.get();
	while(buf != '\0' && buf != '\n')
	{
		s.PushBack(buf);
		buf = stream.get();
	}
	return stream;
}
