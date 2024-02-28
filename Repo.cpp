#include "Repo.h"
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <algorithm>

void Repo::read_from_file() {
	/*Citeste din fisier
	*/
	int id;
	all.clear();
	std::ifstream in("C:\\Users\\Andrea\\source\\repos\\OOP\\practic\\data.txt");
	string descriere, stare, ceva;
	vector<string> programatori;
	while (in >> id >> descriere) {
		programatori.clear();
		while (in >> ceva) {
			if (ceva.compare("Open") != 0 && ceva.compare("Inprogress")!=0 && ceva.compare("Close")!=0) {
				programatori.push_back(ceva);
			}
			else
				break;
		}
		stare = ceva;
		Task t(id, descriere, programatori, stare);
		all.push_back(t);
	}
	in.close();
}


void Repo::write_to_file() {
	/* Scrie in fisier*/
	std::ofstream out("C:\\Users\\Andrea\\source\\repos\\OOP\\practic\\data.txt");
	sort(all.begin(), all.end(), [](Task t1, Task t2) {return t1.get_stare() < t2.get_stare(); });
	for (auto& p : all) {
		out << p.get_id()<<" "<<p.get_descriere()<<" ";
		for (auto prog : p.get_programatori())
			out << prog << " ";
		out << p.get_stare() << endl;
	}
	out.close();
}

int Repo::exists(int id) {
	/* verifica daca exista id-ul in lista curenta*/
	for (auto t : all) {
		if (t.get_id() == id)
			return 1;
	}
	return 0;
}

void Repo::validare(Task t) {
	/*Verifica daca taskul e valid.	*/
	string desc = t.get_descriere();
	if (desc.size() == 0)
		throw exception();
	string stare = t.get_stare();
	if (stare.compare("Open") != 0 && stare.compare("Inprogress") != 0 && stare.compare("Close") != 0)
		throw exception();
	if (exists(t.get_id()))
		throw exception();
	vector<string> rez{ t.get_programatori() };
	if (rez.size() < 1 || rez.size() > 4)
		throw exception();
}

void Repo::adauga(Task t) {
	/*Adauga un task la lista
	param : t - task-ul
	type: Task
	*/
	read_from_file();
	all.push_back(t);
	write_to_file();
}

vector<Task> Repo::get_all() {
	/*Returneaza toata lista de task-uri*/
	return all;
}

void Repo::modifyTask(Task& task, string state)
{
	task.set_stare(state);
	write_to_file();
}