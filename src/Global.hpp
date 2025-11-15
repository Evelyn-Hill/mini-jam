#pragma once
#include "Log.hpp"
#include <vector>
#include "Entity.hpp"

struct Global {
	std::vector<Entity*> entities;
};

extern Global* g;
extern Log* l;
