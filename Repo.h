#pragma once
#include "task.h"
#include <vector>

using namespace std;

class Repo {
private:
	vector<Task> all;
public:
	Repo() { read_from_file(); };
	void read_from_file();
	void write_to_file();
	void adauga(Task t);
	int exists(int id);
	void validare(Task t);
	void modifyTask(Task& t, string state);
	vector<Task> get_all();
};