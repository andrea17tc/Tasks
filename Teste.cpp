#include "Repo.h"
#include "task.h"
#include <cassert>

void test_repo() {
	Repo r;
	r.read_from_file();
	vector<Task> rez{ r.get_all() };
	assert(rez.size() == 10);
	r.write_to_file();
	
}

