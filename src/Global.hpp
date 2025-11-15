#include "Log.hpp"
#include "Types.hpp"
#include "Assert.hpp"

struct Global {
	const char* name = "Hi";
};

extern Global* g;
extern Log* l;
