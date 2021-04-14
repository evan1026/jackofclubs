#pragma once

#include "Graphics/Components/Component.h"

class VBox : public Component {

    public:
        VBox(std::initializer_list<std::shared_ptr<Component>> args);

        virtual void layout(const sf::RenderWindow& w) override;
};
