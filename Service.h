#pragma once
#include "Repo.h"
#include "Observer.h"

class Service: public Observable {
private:
	Repo& repo;
public:
	Service(Repo& repo) :repo{ repo } {};
	vector<Task> get_all();
	vector<Task> cauta_prog(string prog);
	vector<Task> filtrare_stare(string state);
	void modifyState(int id, string state);
	void adauga(Task t);
};

vector<string> split(string input, char delimiter);