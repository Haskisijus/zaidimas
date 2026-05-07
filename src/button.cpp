#include "button.h"

Button::Button() : isHovered(false) {
    shape.setSize(Vector2f(200, 50));
    shape.setFillColor(Color::Blue);
    shape.setOutlineColor(Color::White);
    shape.setOutlineThickness(2);
    shape.setOrigin(100, 25);
}

Button::Button(const std::string& value, Font& font, unsigned int size) : Button() {
    setText(value, font, static_cast<int>(size));
    centerText();
}

void Button::setPosition(float x, float y) {
    shape.setPosition(x, y);
    centerText();
}

void Button::setText(const std::string& str, Font& font, int size) {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(size);
    text.setFillColor(Color::White);
    centerText();
}

void Button::centerText() {
    FloatRect textBounds = text.getLocalBounds();
    float x = shape.getPosition().x - textBounds.width / 2.f - textBounds.left;
    float y = shape.getPosition().y - textBounds.height / 2.f - textBounds.top;
    text.setPosition(x, y);
}

void Button::setSize(float width, float height) {
    shape.setSize(Vector2f(width, height));
    shape.setOrigin(width / 2.f, height / 2.f);
    centerText();
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

void Button::draw(RenderWindow& window) const {
    window.draw(shape);
    window.draw(text);
}

bool Button::contains(Vector2f point) const {
    return shape.getGlobalBounds().contains(point);
}

Vector2f Button::getPosition() const {
    return shape.getPosition();
}

