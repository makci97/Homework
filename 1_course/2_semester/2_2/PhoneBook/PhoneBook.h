#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>

struct OtherInfo
{
    std::vector<unsigned int> numbers;
    std::string email;
};

class Subscriber
{
    public:
        Subscriber(): _email("\0") {}
        Subscriber(const std::string& name, const unsigned int& number);
        Subscriber(const std::string& name, const unsigned int& number, const std::string& email);
        Subscriber(const std::string& name, const OtherInfo& other);
        Subscriber(const Subscriber& other);
        Subscriber& operator=(const Subscriber& other);

        void SetName(const std::string& name);
        std::string GetName() const;

        void AddNumber(const unsigned int& number);
        void PrintNumbers() const;
        unsigned int GetINumber(int i) const;
        void DeleteINumber(int i);

        void SetEmail(const std::string& email);
        std::string GetEmail() const;
        void PrintEmail() const;
        void DeleteEmail();

        void PrintAllAboutSubscriber() const;

    //protected:
        std::string _name;
        std::vector<unsigned int> _numbers;
        std::string _email;
};

class PhoneBook
{
    friend class Subscriber;
    public:
        PhoneBook() {}
        PhoneBook(const PhoneBook& other):_by_name(other._by_name), _by_number(other._by_number) {}

        void AddSubscriber(const Subscriber& sub);
        void AddNumberForName(const std::string& name, const unsigned int& number);
        void DeleteNumber(const unsigned int& number);
        void DeleteOneNumber(const std::string& name);
        void DeleteSubscriber(const std::string& name);
        void DeleteSubscriber(const Subscriber& sub);

        void PrintAllNumbersOfSubscriber(const std::string& name) const;
        void PrintAllNumbersOfSubscriber(const Subscriber& sub) const;
        void PrintAllAboutSubscriber(const std::string& name) const;
        void PrintAllAboutSubscriber(const Subscriber& sub) const;
        void PrintOwnerByNumber(const unsigned int& number) const;
        void PrintPhoneBook() const;

    private:
        std::map<std::string, OtherInfo> _by_name;
        std::map<unsigned int, std::string> _by_number;
};
