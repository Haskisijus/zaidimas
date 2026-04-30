#pragma once

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

class Button {
private:
    Text text;
    bool isHovered;

public:
    RectangleShape shape;

    Button();

    void setPosition(float x, float y);
    void setText(const std::string& str, Font& font, int size);
    void setSize(float width, float height);
    void update(Vector2f mousePos);
    bool isClicked(Vector2f mousePos) const;
    void draw(RenderWindow& window);
    void draw(RenderTexture& texture);
    Vector2f getPosition() const;
};

