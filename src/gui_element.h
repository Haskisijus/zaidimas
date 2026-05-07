#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class GuiElement {
public:
    virtual ~GuiElement() = default;

    virtual void draw(RenderWindow& window) const = 0;
    virtual bool contains(Vector2f point) const = 0;
    virtual void setPosition(float x, float y) = 0;
};

