#include <iostream>
#include <vector> 
#include <thread>
#include <mutex>
#include <algorithm>
using namespace std;

class error :invalid_argument
{
public:
	error(string _description, string _address, int _buildyear):invalid_argument(_description)
	{
		address = _address;
		buildYear = _buildyear;		
	}
	void ShowErrorInfo()
	{
		cout << what() << endl;
		cout << "Address: " << address << endl;
		cout << "Built in: " << buildYear << endl;
	}

private:
	string address;
	int buildYear;
};

class Building
{
private://видит только этот класс
	string address;
	int buildYear;
public://видят все
	Building()//выделяем память
	{}
	Building(string _address, int _buildYear)//еще и инициализируем и заполняем поля
	{
		address = _address;

		if(_buildYear>=1900&&_buildYear<=2020)
			buildYear = _buildYear;
		else { throw error("Incorrect build year", _address, _buildYear); }

	}
	virtual void PrintInfo() //без virtual наследники переопределять функции не смогут
	{
		cout << "Address: " << address << endl;
		cout << "Built in: " << buildYear << endl;
	}

	friend ostream& operator<< (ostream& out, const Building& building); //friend позволяет извне пользоваться приватными полями
	friend istream& operator>> (istream& in, Building& building);
};

class Condominium : Building
{
private:
	int apartmentNumber;
public:
	Condominium() :Building() {}
	Condominium(string _address, int _buildYear, int _aptNumber) :Building(_address, _buildYear)//конструкторы не наследуются сами по себе, помним, что поля родительского класса приватные
	{
		apartmentNumber = _aptNumber;
	}
	void PrintInfo() override
	{
		Building::PrintInfo();
		cout << "Apartments: " << apartmentNumber << endl;
	}
};

void PrintAll(vector<Condominium> _street)
{
	for (int i = 0; i < _street.size(); i++)
		_street[i].PrintInfo();
	cout << endl;
}

ostream& operator<< (ostream& out, const Building& building)//оператор вывода
{
	out << "Address: " << building.address << endl;
	out << "Built in: " << building.buildYear << endl;
	return out;
}

istream& operator>> (istream& in, Building& building)
{	
	in >> building.address;
	in >> building.buildYear;
	return in;
}


mutex mx;//мьютекс для синхронизации потоков
void PrintAllSync(vector<Condominium> _street)
{
	lock_guard<mutex> lock(mx);
	for (int i = 0; i < _street.size(); i++)
		_street[i].PrintInfo();
	cout << endl;
}

int main()
{
	//задание 1
	//на 3
	cout << "Task 1, mark 3:" << endl;
	cout << "------------------------------" << endl;
	Building home("1, Geroya Rossii Solomatina st", 1997);
	home.PrintInfo();
	cout << endl;

	//на 4
	cout << "Task 1, mark 4:" << endl;
	cout << "------------------------------" << endl;
	Building iohome;
	cin >> iohome;
	cout << iohome << endl;


	//на 5
	cout << "Task 1, mark 5:" << endl;
	cout << "------------------------------" << endl;
	try
	{
		Building badHome("2, Geroya Rossii Solomatina st", 1800);
	}
	catch (error e)
	{
		e.ShowErrorInfo();
		cout << endl;
	}


	//задание 2
	//на 3
	cout << "Task 2, mark 3:" << endl;
	cout << "------------------------------" << endl;
	Condominium home2("4, Molodejnaia st", 1957, 100);
	home2.PrintInfo();
	cout << endl;
	

	//задание 3
	//на 3	
	cout << "Task 3, mark 3:" << endl;
	cout << "------------------------------" << endl;

	vector<Condominium> street1;
	Condominium home1_1("1, Geroya Rossii Solomatina st", 1997, 120);
	Condominium home1_2("3, Geroya Rossii Solomatina st", 1980, 150);
	Condominium home1_3("18, Geroya Rossii Solomatina st", 2000, 200);
	street1.resize(3);
	street1[0] = home1_1;
	street1[1] = home1_2;
	street1[2] = home1_3;
	
	vector<Condominium> street2;
	Condominium home2_1("13, Mosrentgen stl", 1977, 150);
	Condominium home2_2("19, Mosrentgen stl", 1968, 80);
	Condominium home2_3("27, Mosrentgen stl", 1965, 100);
	street2.resize(3);
	street2[0] = home2_1;
	street2[1] = home2_2;
	street2[2] = home2_3;
	
	thread t1(PrintAll, street1);
	thread t2(PrintAll, street2);

	t1.join();
	t2.join();//запуск потоков и ожидание их завершения


	//на 4

	cout << "Task 3, mark 4:" << endl;
	cout << "------------------------------" << endl;
	thread t3(PrintAllSync, street1);
	thread t4(PrintAllSync, street2);

	t3.join();
	t4.join();


}
