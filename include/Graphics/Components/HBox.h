#pragma once

#include "Graphics/Components/Component.h"

class HBox : public Component {

    public:
        HBox(std::initializer_list<std::shared_ptr<Component>> args);
};
