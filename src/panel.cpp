#include "panel.h"

Panel::Panel() {
    shape.setFillColor(Color::Transparent);
}

Panel::Panel(Vector2f size, Color fillColor, Color outlineColor, float outlineThickness) {
    shape.setSize(size);
    shape.setFillColor(fillColor);
    shape.setOutlineColor(outlineColor);
    shape.setOutlineThickness(outlineThickness);
}

void Panel::setSize(Vector2f size) {
    shape.setSize(size);
}

void Panel::setFillColor(Color color) {
    shape.setFillColor(color);
}

void Panel::setOutline(Color color, float thickness) {
    shape.setOutlineColor(color);
    shape.setOutlineThickness(thickness);
}

void Panel::draw(RenderWindow& window) const {
    window.draw(shape);
}

bool Panel::contains(Vector2f point) const {
    return shape.getGlobalBounds().contains(point);
}

void Panel::setPosition(float x, float y) {
    shape.setPosition(x, y);
}

RectangleShape& Panel::getShape() {
    return shape;
}

const RectangleShape& Panel::getShape() const {
    return shape;
}

