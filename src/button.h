#pragma once

#include "gui_element.h"
#include <string>

class Button : public GuiElement {
private:
    Text text;
    bool isHovered;

public:
    RectangleShape shape;

    Button();
    Button(const std::string& value, Font& font, unsigned int size);

    void setText(const std::string& str, Font& font, int size);
    void setSize(float width, float height);
    void update(Vector2f mousePos);
    bool isClicked(Vector2f mousePos) const;
    void centerText();

    void draw(RenderWindow& window) const override;
    bool contains(Vector2f point) const override;
    void setPosition(float x, float y) override;

    Vector2f getPosition() const;
};

