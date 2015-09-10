#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "PhoneBook.h"


//***************************************************************************
//Class Subscriber
//***************************************************************************

Subscriber::Subscriber(const std::string& name, const unsigned int& number):_name(name),_email("\0")
{
    _numbers.push_back(number);
}

Subscriber::Subscriber(const std::string& name, const unsigned int& number, const std::string& email):_name(name), _email(email)
{
    _numbers.push_back(number);
}

Subscriber::Subscriber(const std::string& name, const OtherInfo& other):_name(name), _numbers(other.numbers), _email((other.email.empty())?"\0":other.email) {}

Subscriber::Subscriber(const Subscriber& other)
{
    _name = other._name;
    _numbers = other._numbers;
    _email = other._email;
}

Subscriber& Subscriber::operator=(const Subscriber& other)
{
    _name = other._name;
    _numbers = other._numbers;
    _email = other._email;
    return *this;
}

void Subscriber::SetName(const std::string& name)
{
    _name = name;
}

std::string Subscriber::GetName() const
{
    return _name;
}

void Subscriber::AddNumber(const unsigned int& number)
{
    if(_numbers.end() == find(_numbers.begin(), _numbers.end(), number) )
        _numbers.push_back(number);
}

void Subscriber::PrintNumbers() const
{
    size_t size = _numbers.size();
    for(int i = 0; i < size; ++i)
        std::cout << i + 1 << ") " <<  _numbers[i] << std::endl;
}

unsigned int Subscriber::GetINumber(int i) const
{
    return _numbers.at(i);
}

void Subscriber::DeleteINumber(int i)
{
    if(i < _numbers.size())
        _numbers.erase(_numbers.begin() + (i - 1));
    if(i == 1 && _numbers.size() == 1)
        _numbers.pop_back();
}

void Subscriber::SetEmail(const std::string& email)
{
    _email = email;
}

std::string Subscriber::GetEmail() const
{
    if(_email != "\0")
        return _email;
    throw std::logic_error("Email is empty");
}

void Subscriber::PrintEmail() const
{
    if(_email != "\0")
        std::cout << _email << std::endl;
}

void Subscriber::DeleteEmail()
{
    _email = "\0";
}

void Subscriber::PrintAllAboutSubscriber() const
{
    std::cout << _name << std::endl;
    this -> PrintEmail();
    this -> PrintNumbers();
}


//***************************************************************************
//Class PhoneBook
//***************************************************************************

void PhoneBook::AddSubscriber(const Subscriber& sub)
{
    int size = sub._numbers.size();
    if(size > 0)
    {
        OtherInfo other;
        for(int i = 0; i < size; ++i)
        {
            if(_by_number.find(sub._numbers[i]) == _by_number.end())
            {
                _by_number[sub._numbers[i]] = sub._name;
                other.numbers.push_back(sub._numbers[i]);
            }
        }
        auto it = _by_name.find(sub._name);
        if(it == _by_name.end())
        {
            other.email = sub._email;
            _by_name[sub._name] = other;
        }
        else
        {
            if( (it -> second).numbers != other.numbers)
            {
                std::vector<unsigned int> buf;
                buf.resize(other.numbers.size() + (it -> second).numbers.size());
                std::sort((it -> second).numbers.begin(), (it -> second).numbers.end());
                std::sort( other.numbers.begin(), other.numbers.end());
                auto buf_it = std::set_union((it -> second).numbers.begin(),
                                             (it -> second).numbers.end(),
                                             other.numbers.begin(),
                                             other.numbers.end(),
                                             buf.begin());

                buf.resize(buf_it - buf.begin());
                (it -> second).numbers = buf;
            }

            if( (it -> second).email != sub._email)
                (it -> second).email = sub._email;
        }
    }
}

void PhoneBook::AddNumberForName(const std::string& name, const unsigned int& number)
{
    if(_by_number.find(number) == _by_number.end())
    {
        _by_number[number] = name;
        auto it = _by_name.find(name);
        if(it == _by_name.end())
        {
            OtherInfo other;
            other.email = "\0";
            other.numbers.push_back(number);
            _by_name[name] = other;
        }
        else
        {
            it -> second.numbers.push_back(number);
        }
    }
}

void PhoneBook::DeleteNumber(const unsigned int& number)
{
    auto it_numb = _by_number.find(number);
    if(it_numb != _by_number.end())
    {
        std::string name = it_numb -> second;
        auto it_name = _by_name.find(name);
        auto it_for_erase = find(it_name -> second.numbers.begin(), it_name -> second.numbers.end(), number);
        it_name -> second.numbers.erase(it_for_erase);

        if( it_name -> second.numbers.empty() )
            _by_name.erase(it_name);

        _by_number.erase(it_numb);
    }
}

void PhoneBook::DeleteOneNumber(const std::string& name)
{
    auto it_del = _by_name.find(name);
    if(it_del != _by_name.end())
    {
        unsigned int number_del = *(it_del -> second.numbers.end() - 1);
        it_del -> second.numbers.pop_back();
        if(it_del -> second.numbers.empty())
            _by_name.erase(it_del);

        _by_number.erase(_by_number.find(number_del));
    }
}

void PhoneBook::DeleteSubscriber(const std::string& name)
{
    auto it_del = _by_name.find(name);
    if(it_del != _by_name.end())
    {
        for(auto it = it_del -> second.numbers.begin(), end = it_del -> second.numbers.end(); it != end; ++it)
        {
            _by_number.erase(_by_number.find(*it));
        }

        _by_name.erase(it_del);
    }
}

void PhoneBook::DeleteSubscriber(const Subscriber& sub)
{
    this ->DeleteSubscriber(sub._name);
}


void PhoneBook::PrintAllNumbersOfSubscriber(const std::string& name) const
{
    auto it_name_for_print = _by_name.find(name);
    if(it_name_for_print != _by_name.end())
    {
        std::cout << "Name: " << name << std::endl;
        int i = 1;
        for(auto it = it_name_for_print -> second.numbers.begin(), end = it_name_for_print -> second.numbers.end(); it != end; ++it)
        {
            std::cout << i << ") " << *it << std::endl;
            ++i;
        }
    }
}

void PhoneBook::PrintAllNumbersOfSubscriber(const Subscriber& sub) const
{
    this -> PrintAllNumbersOfSubscriber(sub._name);
}

void PhoneBook::PrintAllAboutSubscriber(const std::string& name) const
{
    auto it_name_for_print = _by_name.find(name);
    if(it_name_for_print != _by_name.end())
    {
        std::cout << "Name: " << name << std::endl;
        std::cout << "Email: " << it_name_for_print -> second.email << std::endl;

        int i = 1;
        for(auto it = it_name_for_print -> second.numbers.begin(), end = it_name_for_print -> second.numbers.end(); it != end; ++it)
        {
            std::cout << i << ") " << *it << std::endl;
            ++i;
        }
    }
}

void PhoneBook::PrintAllAboutSubscriber(const Subscriber& sub) const
{
    this -> PrintAllAboutSubscriber(sub._name);
}

void PhoneBook::PrintOwnerByNumber(const unsigned int& number) const
{
    auto it_owner_number = _by_number.find(number);
    if(it_owner_number != _by_number.end())
        std::cout << it_owner_number -> second << std::endl;
}

void PhoneBook::PrintPhoneBook() const
{
    for(auto it_name = _by_name.begin(), end_name = _by_name.end(); it_name != end_name; ++it_name)
    {
        for(auto it_number = it_name -> second.numbers.begin(), end_number = it_name -> second.numbers.end(); it_number != end_number; ++it_number)
        {
            std::cout << it_name -> first << "\t" << *it_number << std::endl;
        }
    }
}
