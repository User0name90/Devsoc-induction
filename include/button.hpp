#pragma once
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable , public sf::Transformable
{
    sf::Sprite background;
    static const sf::Font style;
    sf::Text label;
    sf::Texture texture;
    public:
    Button(sf::Texture, sf::Vector2f,std::string);
    void draw(sf::RenderTarget& target,sf::RenderStates states) const override;
    void mouseON();
    void mouseOFF();
    void mousePress();
    sf::FloatRect getBounds() const;
};