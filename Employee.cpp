#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
using namespace std;

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
using namespace std;

typedef struct Employee
{
	char id[10];
	char name[30];
	char password[10];
	bool isManager;
	Employee* next = nullptr;
}Employee;

typedef struct
{
	Employee* head = nullptr;
	int size = 0;
}EmpList;

/*returns true if the list is empty*/
bool el_isEmpty(EmpList el)
{
	return el.size == 0;
}
/*pushes a new object to the head of the list*/
void el_prepend(Employee* e, EmpList* el)
{
	e->next = el->head;
	el->head = e;
	++el->size;
}
/*pushes a new object to the tail of the list*/
void el_append(Employee* e, EmpList* el)
{
	if (!el_isEmpty(*el))
	{
		Employee* last = el->head;
		for (int i = 0; i < el->size - 1; i++)
		{
			last = last->next;
		}
		last->next = e;
		++el->size;
	}
	else
		el_prepend(e, el);
}
/*gets an object from the list by index*/
Employee* el_get(int index, const EmpList* el)
{
	Employee* e = el->head;
	while (e && index--) {
		e = e->next;
	}
	return e;
}
/*removes an object from the list by index*/
void el_remove(int index, EmpList* el) {
	Employee* before = el->head;
	Employee* toDelete;

	if (index == 0) {
		toDelete = el->head;
		el->head = el->head->next;
	}
	else {
		Employee* before = (el_get(index - 1, el));
		toDelete = before->next;
		before->next = toDelete->next;
	}

	el->size--;
	delete toDelete;
}
/*return list's size*/
unsigned el_length(const EmpList* el) {
	return el->size;
}
/*transforms a clientlist to array and returns it*/
Employee* el_toArray(EmpList* el)
{
	Employee* arr = new Employee[el->size];
	for (int i = 0; i < el->size; i++)
	{
		arr[i] = *el_get(i, el);
	}

	return arr;
}

bool isExist(const char id[10], EmpList *list)
{
	Client* temp = list->head;
	for (int i = 0; i < list->size; i++)
	{
		if (strcmp(id, temp->id) == 0)
			return true;
		temp = temp->next;
	}
	return false;
}

void createEmployee(const char id[10], const char name[30], const char password[10], bool isManager, EmpList* list)
{
	if (!isExist(id, list))
	{
		ofstream file;
		file.open("employees.txt", ios::app);

		if (!el_isEmpty(*list))
			file << "\n";

		if (file.is_open())
		{
			file << id << ", ";
			file << name << ". ";
			file << password << "- ";
			file << isManager;
			file << "\n";
		}
		file.close();

		Employee* e = new Employee;
		strcpy(e->id, id);
		strcpy(e->name, name);
		strcpy(e->password, password);
		e->isManager = isManager;
		el_append(e, list);
	}
}

void deleteEmployee(const char id[10], EmpList* list)
{
	ifstream file;
	ofstream temp;
	string line, idNumber;
	int index;

	file.open("employees.txt");
	temp.open("temp.txt");

	if (file.is_open() && temp.is_open())
	{
		while (!file.eof())
		{
			getline(file, line);
			index = line.find(",");
			idNumber = line.substr(0, index);
			if (idNumber != id)
				temp << line << endl;
		}

		file.close();
		temp.close();
		remove("employees.txt");
		rename("temp.txt", "employees.txt");
	}

	Employee e;
	for (int i = 0; i < list->size; i++)
	{
		e = *el_get(i, list);
		if (strcmp(e.id, id) == 0)
			el_remove(i, list);
	}
}

int main()
{
	EmpList* list = new EmpList;
	createEmployee("315115454", "omer biton", "54848", true, list);
	createEmployee("20727645", "shirel biton", "12345", false, list);
	createEmployee("31584556", "omer danieli", "48655", false, list);
	createEmployee("315115454", "sdf dsf", "1244", false, list);

	deleteEmployee("20727645", list);
	return 0;
}
