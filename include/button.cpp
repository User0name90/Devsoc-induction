#include"button.hpp"

const sf::Font Button::style("arial/ARIAL.TTF");
Button::Button(sf::Texture texture,sf::Vector2f posi,std::string str,sf::Color Press_Color=sf::Color::Green): label{style},texture{std::move(texture)},background{this->texture}, press_color{Press_Color}
{
    background.setPosition(posi);
    label.setPosition(posi);
    label.setString(str);
    background.setTextureRect(sf::IntRect({0,0},{40,35}));
    background.setOrigin({20.f, 17.5f}); // half of 3030
    label.setOrigin({label.getLocalBounds().size / 2.f});
    label.move(sf::Vector2f{-5.f,-50.f});
}
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
        // draw the vertex array
    target.draw(background, states);
    target.draw(label,states);

}
void Button::mouseON() 
{
        background.setColor({200,200,200});
}
void Button::mouseOFF() 
{
    background.setColor({255,255,255});
}
void Button::mousePress()
{
    background.setColor(press_color);
}
sf::FloatRect Button::getBounds() const
{
    return background.getGlobalBounds();
}