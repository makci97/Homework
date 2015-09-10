#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <map>

class LazyManagement
{
    public:
        LazyManagement(const int& n):_n(n) {}
        void AddCommand(std::string& command);
        void BadCommand(std::string& command);
        unsigned long int ManageCommand(std::string& command);
        void FireCommand(std::string& command);
        
        void Add(const std::string& surname, const unsigned long int& time);
        void Bad(const unsigned long int& time, const unsigned long int& k);
        unsigned long int Manage(const std::string& surname);
        void Fire(unsigned long int count);

        bool Command(std::string command);

    private:
        int _n;
        std::list<std::pair<std::string, unsigned long int> > _report;
        std::map<unsigned long int, std::vector<std::string>, std::greater<unsigned long int> > _archive;
        std::map<std::string, unsigned long int> _surname_in_report;
        std::map<std::string, unsigned long int> _surname_in_archive;
        void Time(const unsigned long int& time);
        
};

/*
ADD surname reason time
BAD time k
MANAGE surname
FIRE count
EXIT
*/
