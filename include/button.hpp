#pragma once
#include <SFML/Graphics.hpp>

/**
 * @brief Helps in creating buttons
 * @details Has Text, Font, Spirit and texture. 
 * @note Dimensions of all the buttons and font are same
 */
class Button : public sf::Drawable
{
    /**
     * @details Used for loading texture to a button
     */
    sf::Texture texture;
    /**
     * @details Used to creat rectangle box for button. It is constant for all buttons
     */
    sf::Sprite background;
    /**
     * @details Font is constant for all buttons in constructor (Arial). 
     */
    static const sf::Font style;
    /**
     * @details Text to be displayed on button. Its placement is same for all buttons
     */
    sf::Text label;
    /**
     * @details It represents the color multiplication applied to the after the button is pressed
     */
    sf::Color press_color;
    public:

    /**
     * @brief Used for making toggle buttons like play/pause, gravity etc.
     */
    enum class State {
        Press,
        Released
    };
    /**
     * @details Current state of button
     */
    State state=State::Released;
    /**
     * @brief Constructor for Button class
     * @details It autmatically sets the size of spirite and position of label with respect to spirite
     * @param sf::Texture : Texture for button
     * @param sf::Vector2f : Position of the center of the spirit with repect to view.
     * @param std::string : Label on the button
     * @param sf::Color : Rrepresents the color multiplication applied to the after the button is pressed;
     */
    Button(sf::Texture, sf::Vector2f,std::string,sf::Color);
    /**
     * @brief Draws the spirit and the text
     * 
     */
    void draw(sf::RenderTarget& target,sf::RenderStates states) const override;
    /**
     * @brief Used to blacken the button when mouse is on the button
     */
    void mouseON();
    /**
     * @brief Used to restore the original texture changed by @fn mouseOFF() and @fn mousePress()
     * @return void (nothing)
     */
    void mouseOFF();
    /**
     * @brief Applies sf::Color press_color on the texture. Used when button is pressed.
     */
    void mousePress();
    sf::FloatRect getBounds() const;
};