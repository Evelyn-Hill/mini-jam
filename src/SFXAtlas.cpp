#include "SFXAtlas.hpp"
#include "Assert.hpp"
#include <utility>


SFXAtlas::SFXAtlas() {
	Load("click");
	Load("fired");
	Load("ohno");
	Load("ohno2");
	Load("ok");
	Load("perfect");
	Load("ready");
	Load("that1");
	Load("that2");
	Load("that3");
	Load("this1");
	Load("this2");
	Load("this4");
	Load("tryagain");
	Load("wasntyourbest");
}

SFXAtlas::~SFXAtlas () {};

void SFXAtlas::Load(std::string name) {
	std::string assetDir = "assets/";
	std::string ext = ".wav";
	Sound s = LoadSound(std::string(assetDir + name + ext).c_str());
	map.emplace(std::make_pair(name, s));
}

void SFXAtlas::PlaySFX(std::string name) {
	ASSERT(map.contains(name), "SFX: " + name + " Doesnt exist!");
	PlaySound(map.at(name));
}
