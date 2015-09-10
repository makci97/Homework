#pragma once
#include <iostream>
#include <vector>

using namespace std;

class String
{
	public:
		String() { }
		String(char* const s);
                String(const String& s);
		char& operator[](long int i);
                String& operator=(const String& s);
                bool operator==(const String& s) const;
                bool operator!=(const String& s) const;
                bool operator<(const String& s) const;
                bool operator<=(const String& s) const;
                bool operator>(const String& s) const;
                bool operator>=(const String& s) const;
                String& operator+=(const String& s);
		long int Size() const;
		String& Resize(long int size);
		String& PushBack(char const c);
		String& InsertAfterIElement(long int i, String s);
		String& RemoveFromItoJ(long int i, long int j);
		String& InsertAfterLastElement(String s);
		String& DeleteLastElement();

	private:
		vector<char> _s;		
};


String operator+(const String& irst, const String& second);
ostream& operator << (ostream &stream, String &s);
istream& operator >> (istream &stream, String &s);
