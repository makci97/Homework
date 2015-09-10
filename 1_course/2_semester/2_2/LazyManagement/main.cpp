#include <iostream>
#include <fstream>
#include <string>
#include "LazyManagement.h"

int main()
{
    std::ifstream fs;
    fs.open("input.txt");
    int n;
    fs >> n;
    LazyManagement management(n);
    std::string str;

    do
    {
        std::cout << "Enter a command :";
        std::getline(fs, str);
        std::cout << "Received command " << str << std::endl;
    }
    while( management.Command(str) );

    fs.close();

    return 0;
}
