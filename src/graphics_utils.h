#pragma once

#include <SFML/Graphics.hpp>
#include "objects.h"

using namespace sf;

namespace graphics {
    // Raketos piešimo funkcijos
    void DrawRocket(RenderWindow& window, float x, float y, bool hasThrust);
    void DrawRocketScaled(RenderWindow& window, float x, float y, bool hasThrust, float scaleX, float scaleY);

    // Platformos piešimo funkcijos
    void DrawPlatform(RenderWindow& window, float x, float y);
    void DrawPlatformScaled(RenderWindow& window, float x, float y, float scaleX, float scaleY);
    void DrawPlatformTyped(RenderWindow& window, const Plate& plate, float scaleX, float scaleY);

    // Fono piešimo funkcija
    void DrawBackground(RenderWindow& window, float scaleX = 1.0f, float scaleY = 1.0f);

    // Teksto centravimas
    void CenterOrigin(Text& text);
}

