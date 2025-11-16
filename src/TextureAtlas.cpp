#include "TextureAtlas.hpp"
#include <unordered_map>
#include "Assert.hpp"
#include "raylib.h"


TextureAtlas::TextureAtlas(std::string assetDir) {
	this->assetDir = assetDir;
	map = new std::unordered_map<std::string, Texture2D>;
}

TextureAtlas::~TextureAtlas() {
}

Texture TextureAtlas::GetTexture(std::string name) {
	ASSERT(map->contains(name), "Could not find assset: " + name);
	return map->at(name);
}

void TextureAtlas::TALoadTexture(std::string name) {
	std::string concat = assetDir + name + ".png";
	Texture2D tex = LoadTexture(concat.c_str());
	ASSERT(IsTextureValid(tex), "Could not load texture: " + concat);
	map->emplace(std::make_pair(name, LoadTexture(concat.c_str())));
}


