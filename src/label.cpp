#include "label.h"

Label::Label() {
    text.setFillColor(Color::White);
}

Label::Label(const std::string& value, Font& font, unsigned int size, Color color) {
    text.setFont(font);
    text.setString(value);
    text.setCharacterSize(size);
    text.setFillColor(color);
}

void Label::setText(const std::string& value) {
    text.setString(value);
}

void Label::setFont(Font& font) {
    text.setFont(font);
}

void Label::setCharacterSize(unsigned int size) {
    text.setCharacterSize(size);
}

void Label::setFillColor(Color color) {
    text.setFillColor(color);
}

void Label::centerOrigin() {
    FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

void Label::draw(RenderWindow& window) const {
    window.draw(text);
}

bool Label::contains(Vector2f) const {
    return false;
}

void Label::setPosition(float x, float y) {
    text.setPosition(x, y);
}

Text& Label::getText() {
    return text;
}

const Text& Label::getText() const {
    return text;
}

