#include "button.h"

Button::Button() : isHovered(false) {
    shape.setSize(Vector2f(200, 50));
    shape.setFillColor(Color::Blue);
    shape.setOutlineColor(Color::White);
    shape.setOutlineThickness(2);
    shape.setOrigin(100, 25);
}

void Button::setPosition(float x, float y) {
    shape.setPosition(x, y);
    FloatRect textBounds = text.getLocalBounds();
    float textX = x - textBounds.width / 2.f - textBounds.left;
    float textY = y - textBounds.height / 2.f - textBounds.top;
    text.setPosition(textX, textY);
}

void Button::setText(const std::string& str, Font& font, int size) {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(Color::White);
}

void Button::setSize(float width, float height) {
    shape.setSize(Vector2f(width, height));
    shape.setOrigin(width / 2.f, height / 2.f);
}

void Button::update(Vector2f mousePos) {
    isHovered = shape.getGlobalBounds().contains(mousePos);
    if (isHovered) {
        shape.setFillColor(Color::Cyan);
    } else {
        shape.setFillColor(Color::Blue);
    }
}

bool Button::isClicked(Vector2f mousePos) const {
    return shape.getGlobalBounds().contains(mousePos);
}

void Button::draw(RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

void Button::draw(RenderTexture& texture) {
    texture.draw(shape);
    texture.draw(text);
}

Vector2f Button::getPosition() const {
    return shape.getPosition();
}

