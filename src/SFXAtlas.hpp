#pragma once
#include <string>
#include <unordered_map>
#include <raylib.h>

class SFXAtlas {
public:
	SFXAtlas();
	~SFXAtlas();
	void PlaySFX(std::string name);
private:
	void Load(std::string name);
	std::unordered_map<std::string, Sound> map;
};
