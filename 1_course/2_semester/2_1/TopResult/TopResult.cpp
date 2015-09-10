#include <iostream>
#include <utility>
#include <deque>
#include <vector>
#include <stdexcept>

using namespace std;

template <typename K, typename V>
class TopResult
{
	public:
		TopResult<K, V>() { }
		TopResult<K, V>(int k): _k(k) { }
		TopResult<K, V>(vector< pair<K, V> >& v, int k);
		void SetK(int k);
		void Put(pair<K, V> p);
		pair<K, V> GetFirst();
		pair<K, V> GetLast();

        class iterator
        {
                public:
                        iterator() { }
                        iterator(const TopResult<K,V>::iterator& it): _it(it._it) { }
                        iterator& operator=(const TopResult<K,V>::iterator& it);
                        iterator& operator+=(int i);
                        iterator& operator-=(int i);
                        iterator& operator++();
                        iterator& operator--();

                        iterator GetIterator() const;
                        void SetIterator(const typename deque<pair<K,V> >::iterator& it);

                        bool operator==(const TopResult<K,V>::iterator& it);
                        bool operator!=(const TopResult<K,V>::iterator& it);
                        pair<K, V>& operator*();

                        template <typename KK, typename VV>
                        friend iterator operator+(const typename TopResult<KK,VV>::iterator& it, int i);

                        template <typename KK, typename VV>
                        friend iterator operator-(const typename TopResult<KK,VV>::iterator& it, int i);
                private:
                         typename deque<pair<K,V> >::iterator _it;
        };

        iterator Begin();
        iterator End();

        class reverse_iterator
        {
                public:
                        reverse_iterator() { }
                        reverse_iterator(const TopResult<K,V>::reverse_iterator& rit): _rit(rit._rit) { }
                        reverse_iterator& operator=(const TopResult<K,V>::reverse_iterator& rit);
                        reverse_iterator& operator+=(int i);
                        reverse_iterator& operator-=(int i);
                        reverse_iterator& operator++();
                        reverse_iterator& operator--();

                        reverse_iterator GetIterator() const;
                        void SetIterator(const typename deque<pair<K,V> >::reverse_iterator& rit);

                        bool operator==(const TopResult<K,V>::reverse_iterator& rit);
                        bool operator!=(const TopResult<K,V>::reverse_iterator& rit);
                        pair<K, V>& operator*();

                        template <typename KK, typename VV>
                        friend reverse_iterator operator+(const typename TopResult<KK,VV>::reverse_iterator& rit, int i);

                        template <typename KK, typename VV>
                        friend reverse_iterator operator-(const typename TopResult<KK,VV>::reverse_iterator& rit, int i);
                private:
                         typename deque<pair<K,V> >::reverse_iterator _rit;
        };

        reverse_iterator RBegin();
        reverse_iterator REnd();

        private:
		int _k;
		deque<pair<K, V> > _l;
};


//***************************************************************
//Class TopResult<K,V>
//***************************************************************

template <typename K, typename V>
TopResult<K, V>::TopResult(vector< pair<K, V> >& v, int k)
{
        _k = k;
        for(auto it = v.begin(), end = v.end(); it != end; ++it)
                this -> Put(*it);
}

template <typename K, typename V>
void TopResult<K, V>::SetK(int k)
{
	_k = k;
}

template <typename K, typename V>
void TopResult<K, V>::Put(pair<K, V> p)
{
	_l.resize(_l.size() + 1);
        auto it = _l.begin();
        for(auto end = _l.end(); (it + 1) != end; ++it)
	{
		if(it ->first < p.first)
			break;
	}

        auto r_it = _l.rbegin();
	if(_l.size() != 1 && &(*r_it) != &(*it))
	{
		do
		{
			r_it -> first = (r_it + 1) -> first;
			r_it -> second = (r_it + 1) -> second;
			++r_it;
		}
		while(&(*r_it) != &(*it));

	}
	r_it -> first = p.first;
	r_it -> second = p.second;

    if(_l.size() > _k - 1)
        _l.resize(_k - 1);
		
}

template <typename K, typename V>
pair<K, V> TopResult<K, V>::GetFirst()
{
	if(_l.empty())
		throw logic_error("TopResult is empty!");
	return *(_l.begin());
}

template <typename K, typename V>
pair<K, V> TopResult<K, V>::GetLast()
{
	if(_l.empty())
		throw logic_error("TopResult is empty!");
	return *(--_l.end());
}


template <typename K, typename V>
typename TopResult<K,V>::iterator TopResult<K,V>::Begin()
{
        TopResult<K,V>::iterator result;
        result.SetIterator(_l.begin());
        return result;
}

template <typename K, typename V>
typename TopResult<K,V>::iterator TopResult<K,V>::End()
{
        TopResult<K,V>::iterator result;
        result.SetIterator(_l.end());
        return result;
}

template <typename K, typename V>
typename TopResult<K,V>::reverse_iterator TopResult<K,V>::RBegin()
{
        TopResult<K,V>::reverse_iterator result;
        result.SetIterator(_l.rbegin());
        return result;
}

template <typename K, typename V>
typename TopResult<K,V>::reverse_iterator TopResult<K,V>::REnd()
{
        TopResult<K,V>::reverse_iterator result;
        result.SetIterator(_l.rend());
        return result;
}


//***************************************************************
//Class TopResult<K,V>::iterator
//***************************************************************

template <typename K, typename V>
typename TopResult<K, V>::iterator& TopResult<K, V>::iterator::operator=(const TopResult<K,V>::iterator& it)
{
        _it = it._it;
        return *this;
}

template <typename K, typename V>
typename TopResult<K, V>::iterator& TopResult<K, V>::iterator::operator+=(int i)
{
        _it += i;
        return *this;
}

template <typename K, typename V>
typename TopResult<K, V>::iterator& TopResult<K, V>::iterator::operator-=(int i)
{
        _it -= i;
        return *this;
}

template <typename K, typename V>
typename TopResult<K, V>::iterator& TopResult<K, V>::iterator::operator++()
{
        ++_it;
        return *this;
}

template <typename K, typename V>
typename TopResult<K, V>::iterator& TopResult<K, V>::iterator::operator--()
{
        --_it;
        return *this;
}

template <typename K, typename V>
typename TopResult<K, V>::iterator TopResult<K, V>::iterator::GetIterator() const
{
        return *this;
}

template <typename K, typename V>
void TopResult<K, V>::iterator::SetIterator(const typename deque<pair<K,V> >::iterator& it)
{
        _it = it;
}

template <typename K, typename V>
bool TopResult<K, V>::iterator::operator==(const TopResult<K,V>::iterator& it)
{
        return _it == it._it;
}

template <typename K, typename V>
bool TopResult<K, V>::iterator::operator!=(const TopResult<K,V>::iterator& it)
{
        return _it!= it._it;
}

template <typename K, typename V>
pair<K, V>& TopResult<K, V>::iterator::operator*()
{
        return *_it;
}

template <typename KK, typename VV>
typename TopResult<KK,VV>::iterator operator+(const typename TopResult<KK,VV>::iterator& it, int i)
{
        auto res = it;
        res += i;
        return res;
}

template <typename KK, typename VV>
typename TopResult<KK,VV>::iterator operator-(const typename TopResult<KK,VV>::iterator& it, int i)
{
        auto res = it;
        res -= i;
        return res;
}


//***************************************************************
//Class TopResult<K,V>::reverse_iterator
//***************************************************************

template <typename K, typename V>
typename TopResult<K, V>::reverse_iterator& TopResult<K, V>::reverse_iterator::operator=(const TopResult<K,V>::reverse_iterator& rit)
{
        _rit = rit._rit;
        return *this;
}

template <typename K, typename V>
typename TopResult<K, V>::reverse_iterator& TopResult<K, V>::reverse_iterator::operator+=(int i)
{
        _rit += i;
        return *this;
}

template <typename K, typename V>
typename TopResult<K, V>::reverse_iterator& TopResult<K, V>::reverse_iterator::operator-=(int i)
{
        _rit -= i;
        return *this;
}

template <typename K, typename V>
typename TopResult<K, V>::reverse_iterator& TopResult<K, V>::reverse_iterator::operator++()
{
        ++_rit;
        return *this;
}

template <typename K, typename V>
typename TopResult<K, V>::reverse_iterator& TopResult<K, V>::reverse_iterator::operator--()
{
        --_rit;
        return *this;
}

template <typename K, typename V>
typename TopResult<K, V>::reverse_iterator TopResult<K, V>::reverse_iterator::GetIterator() const
{
        return *this;
}

template <typename K, typename V>
void TopResult<K, V>::reverse_iterator::SetIterator(const typename deque<pair<K,V> >::reverse_iterator& rit)
{
        _rit = rit;
}

template <typename K, typename V>
bool TopResult<K, V>::reverse_iterator::operator==(const TopResult<K,V>::reverse_iterator& rit)
{
        return _rit == rit._rit;
}

template <typename K, typename V>
bool TopResult<K, V>::reverse_iterator::operator!=(const TopResult<K,V>::reverse_iterator& rit)
{
        return _rit!= rit._rit;
}

template <typename K, typename V>
pair<K, V>& TopResult<K, V>::reverse_iterator::operator*()
{
        return *_rit;
}

template <typename KK, typename VV>
typename TopResult<KK,VV>::reverse_iterator operator+(const typename TopResult<KK,VV>::reverse_iterator& rit, int i)
{
        auto res = rit;
        res += i;
        return res;
}

template <typename KK, typename VV>
typename TopResult<KK,VV>::reverse_iterator operator-(const typename TopResult<KK,VV>::reverse_iterator& rit, int i)
{
        auto res = rit;
        res -= i;
        return res;
}



int main()
{
	TopResult<int, double> a(10);
	a.Put(pair<int, double>(1, 2));
	a.Put(pair<int, double>(3, 6));
	a.Put(pair<int, double>(2, 3));
	a.Put(pair<int, double>(2, 3));
	a.Put(pair<int, double>(2, 3));
	a.Put(pair<int, double>(1, 2));
	a.Put(pair<int, double>(3, 6));
	a.Put(pair<int, double>(2, 3));
	a.Put(pair<int, double>(-1, 3));
	a.Put(pair<int, double>(2, 3));
	a.Put(pair<int, double>(1, 2));
	a.Put(pair<int, double>(3, 6));
	a.Put(pair<int, double>(2, 3));
	a.Put(pair<int, double>(2, 3));
	a.Put(pair<int, double>(2, 3));
	cout << a.GetFirst().first << " " << a.GetFirst().second << endl;
    cout << a.GetLast().first << " " << a.GetLast().second << endl;
    cout << "-----------------\n";


    for(TopResult<int, double>::iterator it = a.Begin(), end = a.End(); it != end; ++it)
    {
            cout << (*it).first << " " << (*it).second << endl;
    }
	return 0;
}
