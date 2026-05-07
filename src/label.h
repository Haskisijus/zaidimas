#pragma once

#include "gui_element.h"
#include <string>

class Label : public GuiElement {
private:
    Text text;

public:
    Label();
    Label(const std::string& value, Font& font, unsigned int size, Color color);

    void setText(const std::string& value);
    void setFont(Font& font);
    void setCharacterSize(unsigned int size);
    void setFillColor(Color color);
    void centerOrigin();

    void draw(RenderWindow& window) const override;
    bool contains(Vector2f point) const override;
    void setPosition(float x, float y) override;

    Text& getText();
    const Text& getText() const;
};

