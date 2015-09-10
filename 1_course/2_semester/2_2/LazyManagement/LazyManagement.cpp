#include <iostream>
#include <list>
#include <string>
#include <cstring>
#include <utility>
#include <map>
#include <algorithm>
#include "LazyManagement.h"

//*************************************************************************
//Class LazyManagement
//*************************************************************************

void LazyManagement::AddCommand(std::string& command)
{
    command.erase(command.begin(), command.begin() + 4);

    std::string surname;
    auto it = command.begin(), end = command.end();
    while( *(++it) != ' ' && it != end );


    surname.resize(std::distance(command.begin(), it));
    std::copy(command.begin(), it, surname.begin());
    command.erase(command.begin(), it + 1);

    it = command.begin();
    end = command.end();
    while( *it != ' ' && it != end )
        ++it;

    command.erase(command.begin(), it + 1);
    unsigned long int time = std::stoi(command, nullptr);

    this -> Add(surname, time);
}

void LazyManagement::BadCommand(std::string& command)
{
    command.erase(command.begin(), command.begin() + 4);

    std::string buf;
    auto it = command.begin(), end = command.end();
    while( *it != ' ' && it != end )
        ++it;
    std::copy(command.begin(), it, buf.begin());
    command.erase(command.begin(), it + 1);
    unsigned long int time = std::stoi(buf, nullptr);

    unsigned long int k = std::stoi(command, nullptr);
    this -> Bad(time, k);
}

unsigned long int LazyManagement::ManageCommand(std::string& command)
{
    command.erase(command.begin(), command.begin() + 7);
    this -> Manage(command);
}

void LazyManagement::FireCommand(std::string& command)
{
    command.erase(command.begin(), command.begin() + 5);

    this -> Fire( std::stoi(command, nullptr) );
}

void LazyManagement::Add(const std::string& surname, const unsigned long int& time)
{
    this -> Time(time);
    _report.push_back(make_pair(surname, time));
    auto it = _surname_in_report.find(surname);
    if( it != _surname_in_report.end() )
    {
        ++(it -> second);
    }
    else
    {
        _surname_in_report[surname] = 1;
    }
}

void LazyManagement::Bad(const unsigned long int& time, const unsigned long int& k)
{
    this -> Time(time);
    if(_report.empty() || k == 0)
        std::cout << "NOBODY" << std::endl;
    else
    {
        unsigned long int true_k = k;
        if(k > _report.size())
        {
           true_k = _report.size();
        }

        std::vector<std::string> print;
        print.reserve(true_k);
        auto it = _report.begin();
        for(unsigned long int i = 0; i < true_k; ++i)
        {
            print.push_back(it -> first);
            ++it;
        }

        std::sort(print.begin(), print.end());
        auto it_vec = print.begin(), end_vec = print.end();
        while( it_vec != end_vec)
        {
            std::cout << *it_vec << std::endl;
            ++it_vec;
        }

        print.clear();
        _report.erase(_report.begin(), it);
    }
}

unsigned long int LazyManagement::Manage(const std::string& surname)
{
    unsigned long int result = 0;

    auto it_arch = _surname_in_archive.find(surname);
    if( it_arch != _surname_in_archive.end() )
    {
        result += it_arch -> second;
    }

    auto it_report = _surname_in_report.find(surname);
    if( it_report != _surname_in_report.end() )
    {
        result += it_report -> second;
    }

    std::cout << result << std::endl;

    return result;
}

void LazyManagement::Fire(unsigned long int count)
{
    if(_archive.empty() || count == 0)
        std::cout << "ALIVE" << std::endl;

    else
    {

        auto it_arch = _archive.begin();

        while(count > 0 && !_archive.empty() )
        {
            count -= it_arch -> second.size();

            std::sort(it_arch -> second.begin(), it_arch -> second.end());

            auto it_vec_sur_in_arch = it_arch -> second.begin(), end_vec_sur_in_arch = it_arch -> second.end();
            while( it_vec_sur_in_arch != end_vec_sur_in_arch )
            {

                auto it_sur_in_rep = _surname_in_report.find(*it_vec_sur_in_arch);
                if( it_sur_in_rep != _surname_in_report.end() )
                {
                    unsigned long int count_in_rep = it_sur_in_rep -> second;
                    auto it_list = _report.begin(), end_list = _report.end();
                    while( it_list != end_list && count_in_rep > 0)
                    {
                        if( it_list -> first == *it_vec_sur_in_arch)
                        {
                            auto buf = it_list;
                            ++it_list;
                            _report.erase(buf);
                        }
                        else
                            ++it_list;
                    }

                    _surname_in_report.erase(it_sur_in_rep);

                }

                auto it_sur_in_arch = _surname_in_archive.find(*it_vec_sur_in_arch);
                if( it_sur_in_arch != _surname_in_archive.end() )
                    _surname_in_archive.erase(it_sur_in_arch);

                std::cout << *it_vec_sur_in_arch << "\t" << it_arch -> first << std::endl;
                ++it_vec_sur_in_arch;
            }

            _archive.erase(it_arch);
            it_arch = _archive.begin();
        }

    }

}


bool LazyManagement::Command(std::string command)
{
    switch(command[0])
    {
        case 'A':
            this -> AddCommand(command);
            return true;
        case 'B':
            this -> BadCommand(command);
            return true;
        case 'M':
            this -> ManageCommand(command);
            return true;
        case 'F':
            this -> FireCommand(command);
            return true;
        case 'E':
            return false;
        default:
            return true;
    }
}



void LazyManagement::Time(const unsigned long int& time)
{
    /*
    auto it_list = std::lower_bound(_report.begin(),_report.end(), [&](std::pair<std::string, unsigned long int> p)
    {
        return ( (time - p.second) > _n);
    });*/

    //for(auto it_list = _report.begin(); time - it_list -> second > _n ; ++it);
    auto it_list = _report.begin(), end_list = _report.end();
    while( time - it_list -> second > _n && it_list != end_list)
        ++it_list;

    auto it_for_list_erase = it_list;

    if(it_list != _report.begin())
    {
        auto begin = _report.begin();

        do
        {
            --it_list;
            std::string surname = it_list -> first;
            auto it_sur_arch = _surname_in_archive.find(surname);
            if(it_sur_arch != _surname_in_archive.end())
            {
                unsigned long int count_sur_in_archive = it_sur_arch -> second;
                auto it_archive = _archive.find(count_sur_in_archive);
                it_archive -> second.erase( std::find(it_archive -> second.begin(), it_archive -> second.end(), surname) );
                it_archive = _archive.find(count_sur_in_archive + 1);
                if( it_archive != _archive.end() )
                {
                    it_archive -> second.push_back(surname);
                }
                else
                {
                    std::vector<std::string> vec_for_arch;
                    vec_for_arch.push_back(surname);
                    _archive[count_sur_in_archive + 1] = vec_for_arch;
                }

                ++(it_sur_arch -> second);
            }
            else
            {
                auto it_archive = _archive.find(1);
                if( it_archive != _archive.end())
                {
                    it_archive -> second.push_back(surname);

                }
                else
                {
                    std::vector<std::string> vec_for_arch;
                    vec_for_arch.push_back(surname);
                    _archive[1] = vec_for_arch;
                }

                _surname_in_archive[surname] = 1;
            }

            auto it_sur_report = _surname_in_report.find(surname);
            if( it_sur_report != _surname_in_report.end() )
            {
                --(it_sur_report -> second);
                if( it_sur_report -> second == 0 )
                    _surname_in_report.erase(it_sur_report);
            }

        }
        while(it_list != begin);
        

        _report.erase(_report.begin(), it_for_list_erase);
    }    
}
