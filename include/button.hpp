#pragma once
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable
{  
    sf::Texture texture;
    sf::Sprite background;
    static const sf::Font style;
    sf::Text label;
    sf::Color press_color;
    public:

    enum class State {
        Press,
        Released
    };
    State state=State::Released;
    Button(sf::Texture, sf::Vector2f,std::string,sf::Color);
    void draw(sf::RenderTarget& target,sf::RenderStates states) const override;
    void mouseON();
    void mouseOFF();
    void mousePress();
    sf::FloatRect getBounds() const;
};