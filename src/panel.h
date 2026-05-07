#pragma once

#include "gui_element.h"

class Panel : public GuiElement {
private:
    RectangleShape shape;

public:
    Panel();
    Panel(Vector2f size, Color fillColor, Color outlineColor = Color::Transparent, float outlineThickness = 0.f);

    void setSize(Vector2f size);
    void setFillColor(Color color);
    void setOutline(Color color, float thickness);

    void draw(RenderWindow& window) const override;
    bool contains(Vector2f point) const override;
    void setPosition(float x, float y) override;

    RectangleShape& getShape();
    const RectangleShape& getShape() const;
};

