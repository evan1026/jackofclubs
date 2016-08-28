#ifndef COMPONENT_H
#define COMPONENT_H

#include <SFML/Graphics.hpp>

class Component {

    protected:
        sf::Rect<int> _boundingBox;
        sf::Vector2i _localPos;

        void setGlobalPosition(const sf::Vector2i& pos);

    public:
        Component(const sf::Vector2i& localPos, const sf::Vector2i& parentPos, const sf::Vector2i& size);

        virtual void render(sf::RenderWindow& w) = 0;

        void setSize(const sf::Vector2i& size);
        void setLocalPosition(const sf::Vector2i& pos);
        void setParentPosition(const sf::Vector2i& pos);

        sf::Vector2i getSize();
        sf::Vector2i getLocalPosition();
        sf::Vector2i getGlobalPosition();
        sf::Vector2i getParentPosition();

        sf::Rect<int> getBounds();
};

#endif
