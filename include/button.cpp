#include"button.hpp"

const sf::Font Button::style("arial/ARIAL.TTF");
Button::Button(sf::Texture texture,sf::Vector2f posi,std::string str): label{style},texture{texture},background{this->texture}
{
    label.setString(str);
    background.setTextureRect(sf::IntRect({0,0},{40,35}));
    background.setOrigin({20.f, 17.5f}); // half of 3030
    label.setOrigin({label.getLocalBounds().size / 2.f});
    label.move(sf::Vector2f{-5.f,-35.f});
    setPosition(posi);
}
void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

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
    background.setColor({0,255,0});
}
sf::FloatRect Button::getBounds() const
{
    return getTransform().transformRect(background.getGlobalBounds());
}