#pragma once

#include "Graphics/Components/Rectangle.h"
#include "Graphics/Components/Slider.h"
#include "Graphics/Components/Text.h"
#include "Graphics/Menu/Menu.h"

/*!
 * Class for the menu that does color selection (press c when
 * on the world screen). Consists 3 sliders that control 3 ints,
 * as well as a color preview that changes colors as the slider
 * moves.
 *
 * Note that this class will modify an sf::Color directly. If this
 * is not the desired behavior, simply create a temporary sf::Color
 * that will be modified and write it back to wherever it needs to
 * go after the menu is exitted.
 */
class ColorSelectorMenu : public Menu {

    // These references point to the values stored in the sf::Color
    // that gets passed in the constructor. This way, the menu writes
    // to the data directly
    sf::Uint8& _redRef;
    sf::Uint8& _greenRef;
    sf::Uint8& _blueRef;

    std::shared_ptr<Slider<sf::Uint8>> _redSlider;
    std::shared_ptr<Slider<sf::Uint8>> _greenSlider;
    std::shared_ptr<Slider<sf::Uint8>> _blueSlider;

    std::shared_ptr<Rectangle> _colorRect;

    std::shared_ptr<Text> _redText;   // Says "Red" above the red slider
    std::shared_ptr<Text> _greenText; // Says "Green" above the green slider
    std::shared_ptr<Text> _blueText;  // Says "Blue" above the blue slider
    std::shared_ptr<Text> _titleText; // Says "Color Selector" up at the top

    protected:
        virtual void layout(const sf::RenderWindow& w) override;

    public:
        ColorSelectorMenu(sf::Color& color);
        virtual ~ColorSelectorMenu() = default;
};
