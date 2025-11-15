#include "Log.hpp"
#include "Types.hpp"
#include "Assert.hpp"

struct Global {
	const char* name = "Hi";
    float tempo;
};

extern Global* g;
extern Log* l;
