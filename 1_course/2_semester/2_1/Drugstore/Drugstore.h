#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <exception>
using namespace std;

enum Form{tablets, syrup, solution, ointment, gel, dragee, powder, capsule, granule, tincture, extract};
//	(	1,      2,  	3,      4,      5,      6,	7,  	8,  	9,  	10,     11)

class Drug
{
	friend class Drugstore;
	friend class Order;

	public:
		Drug() { }
		Drug(const Drug& drug);
                Drug(const string& name, const string& producer, const string& ingredient, double dosage, Form form): _name(name), _producer(producer), _ingredient(ingredient), _dosage(dosage), _form(form) { }
		
		string GetName() const;
		string GetProducer() const;
		string GetIngredient() const;
		double GetDosage() const;
		Form GetForm() const;
		
                void SetName(const string& name);
                void SetProducer(const string& producer);
                void SetIngredient(const string& ingredient);
		void SetDosage(double dosage);
		void SetForm(Form form);
		
                Drug& operator=(const Drug& other);
                bool operator==(const Drug& other) const;
                bool operator!=(const Drug& other) const;

                friend ostream& operator << (ostream &stream, const Drug& drug);

	protected:
		string _name;
		string _producer;
		string _ingredient;
		double _dosage;
		Form _form;
};

string CharToString(char* c);

class Item: public Drug
{
	friend class Drugstore;
	friend class Order;

	public:
		Item() { }
                Item(const Drug& drug): Drug(drug), _price(-1) { }
                Item(const Drug& drug, double price): Drug(drug), _price(price) { }
                Item(const string& name, const string& producer, const string& ingredient, double dosage, Form form, double price);
		Item(const Item& item);

                double GetPrise() const;
		void SetPrice(double price);
		
                Item& operator=(const Item& other);
                bool operator==(const Item& other) const;
                bool operator!=(const Item& other) const;
		
                friend ostream& operator << (ostream &stream, const Item& item);

	private:
		double _price;

};

class Drugstore
{
	friend class Order;

	public:
		Drugstore() { }
		Drugstore(const vector<Drug>& drugs);
		Drugstore(const vector<Item>& items);
		Drugstore(const Drugstore& drugstore);
                void PutDrug(const Drug& drug);
                void PutDrug(const Drug& drug, double price);
                void PutDrug(Item item);
		double GetPrice(const Drug& drug);
                void ListOfDrugsAccordingToTheName(const string& name, vector<Item>& items);
                void ListOfDrugsAccordingToTheProducer(const string& producer, vector<Item>& items);
                void ListOfDrugsAccordingToTheIngredient(const string& ingredient, vector<Item>& items);
                void ListOfDrugsAccordingToTheProducerWithTheIngredient(const string& producer, const string& ingredient, vector<Item>& items);

                Drugstore& operator=(const Drugstore& other);
                bool operator==(const Drugstore& other) const;
                bool operator!=(const Drugstore& other) const;

	private:
		vector<Item> _items;
};


class Order
{
	public:
		Order();
                Order(const Order& order);

		void OrderAccordingToTheName(const vector<string>& names, const Drugstore& drugstore);
		void OrderAccordingToTheIngredient(const vector<string>& ingredient, const Drugstore& drugstore);
		int Size();

                Order& operator=(const Order& other);
		Item& operator[](int i);
		
                friend ostream& operator << (ostream &stream, const Order& order);

	private:
		vector<Item> _order;
};
