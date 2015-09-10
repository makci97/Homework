#include <iostream>
#include <vector>
#include "PhoneBook.h"

int main()
{
    Subscriber a("Maxim", 1234, "maxim@mail.ru"), b("Danil", 4321), c("Alexandr", 5678), d("Denis", 8765);
    a.PrintAllAboutSubscriber();
    b.PrintAllAboutSubscriber();
    c.PrintAllAboutSubscriber();
    d.PrintAllAboutSubscriber();
    a.AddNumber(143);
    b.SetEmail("danya@gmail.com");
    c.SetName("Sanya");
    d.DeleteINumber(1);
    a.PrintAllAboutSubscriber();
    b.PrintAllAboutSubscriber();
    c.PrintAllAboutSubscriber();
    d.PrintAllAboutSubscriber();

    //-----------------------------------
    std::cout << "\n----------------------------\n";
    PhoneBook book;
    book.AddSubscriber(a);
    book.AddSubscriber(b);
    book.AddSubscriber(c);
    book.AddSubscriber(a);
    book.AddSubscriber(d);
    std::cout << "All about subscriber" << std::endl;
    book.PrintAllAboutSubscriber(b);
    std::cout << "All numbers of subscriber" << std::endl;
    book.PrintAllNumbersOfSubscriber(a);
    std::cout << "Owner of number" << std::endl;
    book.PrintOwnerByNumber(5678);
    std::cout << "Phone book" << std::endl;
    book.PrintPhoneBook();

    //-----------------------------------
    std::cout << "\n----------------------------\n";

    book.DeleteNumber(143);
    std::cout << "\nPhone book" << std::endl;
    book.PrintPhoneBook();

    book.DeleteSubscriber("Danil");
    std::cout << "\nPhone book" << std::endl;
    book.PrintPhoneBook();

    book.DeleteOneNumber("Sanya");
    std::cout << "\nPhone book" << std::endl;
    book.PrintPhoneBook();

    //-----------------------------------
    std::cout << "\n----------------------------\n";

    book.AddSubscriber(b);
    std::cout << "\nPhone book" << std::endl;
    book.PrintPhoneBook();

    book.AddSubscriber(c);
    std::cout << "\nPhone book" << std::endl;
    book.PrintPhoneBook();

    book.AddNumberForName("Denis", 8765);
    std::cout << "\nPhone book" << std::endl;
    book.PrintPhoneBook();

    return 0;
}
