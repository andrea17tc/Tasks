#include "Service.h"
#include "Repo.h"
#include <sstream>

using namespace std;

vector<Task> Service::get_all() {
	return repo.get_all();
}

void Service::adauga(Task t) {
	repo.validare(t);
	repo.adauga(t);
    this->notify_all();
}
vector<string> split(string input, char delimiter) {
    vector<string> result;
    stringstream ss(input);
    string item;

    while (getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}

vector<Task> Service::cauta_prog(string prog) {
    vector<Task> tasks;
    vector<Task> ts{ repo.get_all() };
    for (auto t : ts) {
        vector<string> rez{ t.get_programatori() };
        for (auto p : rez) {
            if (prog.compare(p) == 0)
                tasks.push_back(t);
        }
    }
    return tasks;
}

vector<Task> Service::filtrare_stare(string state) {
    vector<Task> tasks;
    vector<Task> ts{ repo.get_all() };
    for (auto t : ts) {
        if (t.get_stare().compare(state) == 0)
            tasks.push_back(t);
    }
    return tasks;
}


void Service::modifyState(int id, string state)
{
    auto tasks = repo.get_all();
    for (auto task : tasks)
    {
        if (task.get_id() == id)
        {
            repo.modifyTask(task, state);
            notify_all();
            break;
        }
    }
}
