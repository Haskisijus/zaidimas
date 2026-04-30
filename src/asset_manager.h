#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <memory>
#include <string>

using namespace sf;
using namespace std;

class AssetManager {
private:
    unordered_map<string, unique_ptr<Texture>> textures;
    unordered_map<string, unique_ptr<Font>> fonts;
    unordered_map<string, unique_ptr<SoundBuffer>> soundBuffers;

public:
    void loadTexture(const string& name, const string& path);
    Texture& getTexture(const string& name);

    void loadFont(const string& name, const string& path);
    Font& getFont(const string& name);

    void loadSound(const string& name, const string& path);
    SoundBuffer& getSound(const string& name);
};




