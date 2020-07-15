#pragma once

#include <SFML/Graphics.hpp>

#include "Graphics/Components/Component.h"

class Rectangle : public Component {

    sf::RectangleShape _theRectangle;

    protected:
        void renderComponent(sf::RenderWindow& w) override;

    public:
        Rectangle() : Rectangle(sf::Vector2i(0,0)) {}
        explicit Rectangle(sf::Vector2i size) : Rectangle(sf::Vector2i(0,0), size) {}
        Rectangle(sf::Vector2i localPos, sf::Vector2i size) : Component(localPos, size, false /*childrenAllowed*/) {}

        virtual void layout(const sf::RenderWindow& w) override;

        void setFillColor(const sf::Color& color);
        void setOutlineColor(const sf::Color& color);
        void setOutlineThickness(float thickness);
        const sf::Color& getFillColor() const;
        const sf::Color& getOutlineColor() const;
        float getOutlineThickness() const;
};
