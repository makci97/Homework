#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Drugstore.h"

using namespace std;

//**********************************************************************************
//Class Drug
//**********************************************************************************

Drug::Drug(const Drug& drug)
{
	_name = drug._name;
	_producer = drug._producer;
	_ingredient = drug._ingredient;
	_dosage = drug._dosage;
	_form = drug._form;
}

string Drug::GetName() const
{
	return _name;
}

string Drug::GetProducer() const
{
	return _producer;
}

string Drug::GetIngredient() const
{
	return _ingredient;
}

double Drug::GetDosage() const
{
	return _dosage;
}

Form Drug::GetForm() const
{
	return _form;
}

		
void Drug::SetName(const string& name)
{
	_name = name;
}

void Drug::SetProducer(const string& producer)
{
	_producer = producer;
}

void Drug::SetIngredient(const string& ingredient)
{
	_ingredient = ingredient;
}

void Drug::SetDosage(double dosage)
{
	_dosage = dosage;
}

void Drug::SetForm(Form form)
{
	_form = form;
}


Drug& Drug::operator=(const Drug& other)
{
        if(this != &other)
        {
                _name = other._name;
                _producer = other._producer;
                _ingredient = other._ingredient;
                _dosage = other._dosage;
                _form = other._form;
        }
	return *this;
}

bool Drug::operator==(const Drug& other) const
{
	if(_name == other._name && _producer == other._producer && _ingredient == other._ingredient && _dosage == other._dosage && _form == other._form)
		return true;

	return false;
}

bool Drug::operator!=(const Drug& other) const
{
	if(_name == other._name && _producer == other._producer && _ingredient == other._ingredient && _dosage == other._dosage && _form == other._form)
		return false;

	return true;
}

//**********************************************************************************


ostream& operator << (ostream &stream, const Drug& drug)
{
	stream << "{" << drug._name << "; " << drug._producer << "; " << drug._ingredient << "; "<< drug._dosage << "; "<< drug._form << "}" ;
	return stream;
}

string CharToString(char* c)
{
	string result;
	char* buf = c;
	while(*buf != '\0' && *buf != '\n')
		result.push_back(*(buf++));
	
	return result;
}

//**********************************************************************************
//Class Item
//**********************************************************************************

Item::Item(const string& name, const string& producer, const string& ingredient, double dosage, Form form, double price)
{
	_name = name;
	_producer = producer;
	_ingredient = ingredient;
	_dosage = dosage;
	_form = form;
	_price = price;
}

Item::Item(const Item& item)
{
	_name = item._name;
	_producer = item._producer;
	_ingredient = item._ingredient;
	_dosage = item._dosage;
	_form = item._form;
	_price = item._price;
}

double Item::GetPrise() const
{
	return _price;
}

void Item::SetPrice(double price)
{
	_price = price;
}

Item& Item::operator=(const Item& other)
{
        if(this != &other)
        {
                this -> Drug::operator =(other);
                _price = other._price;
        }
	return *this;
}

bool Item::operator==(const Item& other) const
{
	if(_name == other._name && _producer == other._producer && _ingredient == other._ingredient && _dosage == other._dosage && _form == other._form && _price == other._price)
		return true;

	return false;
}

bool Item::operator!=(const Item& other) const
{
	if(_name == other._name && _producer == other._producer && _ingredient == other._ingredient && _dosage == other._dosage && _form == other._form && _price == other._price)
		return false;

	return true;
}

//**********************************************************************************

ostream& operator << (ostream &stream, const Item& item)
{
	stream << "{" << item._name << "; " << item._producer << "; " << item._ingredient << "; "<< item._dosage << "; "<< item._form << "; "<< item._price << "}" ;
	return stream;
}
//**********************************************************************************
//Class Drugstore
//**********************************************************************************

Drugstore::Drugstore(const vector<Drug>& drugs)
{
	int size = drugs.size();
        Item buf;
        buf._price = -1;

	for(int i = 0; i < size; ++i)
	{
                buf.Drug::operator =(drugs[i]);
                _items.push_back(buf);
	}
}

Drugstore::Drugstore(const vector<Item>& items)
{
	int size = items.size();

	for(int i = 0; i < size; ++i)
	{
		_items.push_back(items[i]);
	}
}


Drugstore::Drugstore(const Drugstore& drugstore)
{
	int size = drugstore._items.size();

	for(int i = 0; i < size; ++i)
	{
		_items.push_back(drugstore._items[i]);
	}
}

void Drugstore::PutDrug(const Drug& drug)
{
        Item buf(drug);
        _items.push_back(buf);
}

void Drugstore::PutDrug(const Drug& drug, double price)
{
        Item buf(drug, price);
        _items.push_back(buf);
}

void Drugstore::PutDrug(Item item)
{
	_items.push_back(item);
}

double Drugstore::GetPrice(const Drug& drug)
{
        int size = _items.size();

	for(int i = 0; i < size; ++i)
        {
                if(_items[i].Drug::operator==(drug))
			return _items[i]._price;
	}

	throw logic_error("This drug not found");
}

void Drugstore::ListOfDrugsAccordingToTheName(const string& name, vector<Item>& items)
{
	int size = _items.size();
	items.clear();

	for(int i = 0; i < size; ++i)
	{
		try
		{
			if(_items[i]._name.find_first_of(name) == 0)
				items.push_back(_items[i]);
		}
		catch(...){ }
	}
}

void Drugstore::ListOfDrugsAccordingToTheProducer(const string& producer, vector<Item>& items)
{
	int size = _items.size();
	items.clear();

	for(int i = 0; i < size; ++i)
	{
		if(_items[i]._producer == producer)
			items.push_back(_items[i]);
	}
}

void Drugstore::ListOfDrugsAccordingToTheIngredient(const string& ingredient, vector<Item>& items)
{
	int size = _items.size();
	items.clear();

	for(int i = 0; i < size; ++i)
	{
		if(_items[i]._ingredient == ingredient)
			items.push_back(_items[i]);
	}
}

void Drugstore::ListOfDrugsAccordingToTheProducerWithTheIngredient(const string& producer, const string& ingredient, vector<Item>& items)
{
	int size = _items.size();
	items.clear();

	for(int i = 0; i < size; ++i)
	{
		if(_items[i]._producer == producer && _items[i]._ingredient == ingredient)
			items.push_back(_items[i]);
	}
}

Drugstore& Drugstore::operator=(const Drugstore& other)
{
        if(this != &other)
        {
                int size = other._items.size();
                _items.resize(size);
                while(--size >= 0)
                    _items[size] = other._items[size];
        }
	return *this;
}

bool Drugstore::operator==(const Drugstore& other) const
{
	int size = _items.size();

	if(size != other._items.size())
		return false;

	for(int i = 0; i < size; ++i)
		if(_items[i] != other._items[i])
			return false;

	return true;
}

bool Drugstore::operator!=(const Drugstore& other) const
{
	if(*this == other)
		return false;

	return true;
}

//**********************************************************************************
//Class Order
//**********************************************************************************

Order::Order(const Order& order)
{
	int size = order._order.size();

	for(int i = 0; i < size; ++i)
	{
		_order.push_back(order._order[i]);
	}
}

void Order::OrderAccordingToTheName(const vector<string>& names, const Drugstore& drugstore)
{
	int size_drugstore = drugstore._items.size(), size_names = names.size();
	_order.clear();

	for(int i = 0; i < size_names; ++i)
	{
		for(int j = 0; j < size_drugstore; ++j)
		{
			if(_order[j]._name == names[i])
				_order.push_back(drugstore._items[i]);
		}
	}
}

void Order::OrderAccordingToTheIngredient(const vector<string>& ingredient, const Drugstore& drugstore)
{
	int size_drugstore = drugstore._items.size(), size_ingredient = ingredient.size();
	_order.clear();

	for(int i = 0; i < size_ingredient; ++i)
	{
		bool test = false;
		Item min;

		for(int j = 0; j < size_drugstore; ++j)
		{
			if(_order[j]._ingredient == ingredient[i])
			{
				if(!test)
				{
					min = drugstore._items[i];
					test = true;
				}
				else
					if(min._price > drugstore._items[i]._price)
						min = drugstore._items[i];
			}
		}

		if(test)
			_order.push_back(min);
	}
}

int Order::Size()
{
	return _order.size();
}

Order& Order::operator=(const Order& other)
{
        if(this != &other)
        {
                int size = other._order.size();
                _order.resize(size);
                while(--size >= 0)
                    _order[size] = other._order[size];
        }
        return *this;
}


Item& Order::operator[](int i)
{
	return _order[i];
}

//**********************************************************************************

ostream& operator << (ostream &stream, const Order& order)
{
	int size = order._order.size();

	for(int i = 0; i < size; ++i)
	{
		stream << order._order[i] << endl;
	}
	return stream;
}
