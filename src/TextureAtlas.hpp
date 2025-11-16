#pragma once
#include <raylib.h>
#include <unordered_map>
#include "Global.hpp"

class TextureAtlas {
public:
	TextureAtlas(std::string assetDir = "assets/");
	~TextureAtlas();
	Texture2D GetTexture(std::string name);
	void TALoadTexture(std::string name);
	void LoadBackgroundTextures();

	std::vector<Texture2D> backgroundTextures;
private:
	std::unordered_map<std::string, Texture2D>* map;
	std::string assetDir;
};


