#include "asset_manager.h"
#include <iostream>

using namespace sf;
using namespace std;

void AssetManager::loadTexture(const string& name, const string& path) {
    auto texture = make_unique<Texture>();
    if (texture->loadFromFile(path)) {
        textures[name] = move(texture);
    } else {
        cerr << "Klaida: nepavyko įkelti tekstūros iš " << path << endl;
    }
}

Texture& AssetManager::getTexture(const string& name) {
    return *textures.at(name);
}

void AssetManager::loadFont(const string& name, const string& path) {
    auto font = make_unique<Font>();
    if (font->loadFromFile(path)) {
        fonts[name] = move(font);
    } else {
        cerr << "Klaida: nepavyko įkelti šrifto iš " << path << endl;
    }
}

Font& AssetManager::getFont(const string& name) {
    return *fonts.at(name);
}

void AssetManager::loadSound(const string& name, const string& path) {
    auto soundBuffer = make_unique<SoundBuffer>();
    if (soundBuffer->loadFromFile(path)) {
        soundBuffers[name] = move(soundBuffer);
    } else {
        cerr << "Klaida: nepavyko įkelti garso iš " << path << endl;
    }
}

SoundBuffer& AssetManager::getSound(const string& name) {
    return *soundBuffers.at(name);
}



