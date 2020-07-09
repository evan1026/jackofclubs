#include "Graphics/Components/HBox.h"

HBox::HBox(std::initializer_list<std::shared_ptr<Component>> args) {
    for (auto arg : args) {
        add(arg);
    }
}
