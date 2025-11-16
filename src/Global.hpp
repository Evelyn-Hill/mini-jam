#pragma once
#include "Log.hpp"
#include <vector>

struct Entity;

struct Global {
	std::vector<Entity*> entities;
};

extern Global* g;
extern Log* l;
