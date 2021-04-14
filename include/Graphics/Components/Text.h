#pragma once

#include <SFML/Graphics.hpp>

#include "Graphics/Components/Component.h"
#include "Utils/Font.h"

class Text : public Component {

    sf::Text _theText;
    
    protected:
        void renderComponent(sf::RenderWindow& w) override;

    public:
        Text();
        explicit Text(sf::Vector2i localPos, const sf::String& text = "", const sf::Font& font = Font::defaultFont);
        Text(sf::Vector2i localPos, unsigned int fontSize, const sf::String& text="", const sf::Font& font = Font::defaultFont);

        virtual void layout(const sf::RenderWindow& w) override;


        const sf::Color& getFillColor();
        unsigned int getFontSize();
        const sf::String& getString();

        void setFillColor(const sf::Color& color);
        void setFontSize(unsigned int size);
        void setString(const sf::String& string);
        float textWidth();
        float textHeight();
};
