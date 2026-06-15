#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>



int main()
{
	sf::RenderWindow window( sf::VideoMode( { 200, 200 } ), "Engine" );
	window.setVerticalSyncEnabled(true);
    sf::View view;
    view.setViewport(sf::FloatRect({0.0f,0.0f},{1.0f,1.0f}));
	window.setView(view);
	sf::CircleShape shape( 100.f );
	shape.setFillColor( sf::Color::Green);
	shape.setOutlineThickness(10.0f);

	shape.setOutlineColor(sf::Color(20,230,140));

	sf::Texture tex("Images/concrete.jpg");
	sf::Sprite background(tex);
    background.setScale({view.getSize().x/(float)tex.getSize().x, view.getSize().y/(float)tex.getSize().y});
	window.draw(background);
	std::function<void(const sf::Event::Closed)> onClose = [&window](const sf::Event::Closed&)
	{
		window.close();
	};

	std::function<void(const sf::Event::KeyPressed)> onKeyPressed =  [&window,&shape](const sf::Event::KeyPressed& key)
	{

		if(key.scancode == sf::Keyboard::Scancode::D)	
		{
			shape.move(sf::Vector2f(-2.0f,-2.0f));
			shape.setRadius(shape.getRadius()+2.0f);
		}
		else if(key.scancode == sf::Keyboard::Scancode::A)
		{
			shape.move(sf::Vector2f(2.0f,2.0f));
			shape.setRadius(shape.getRadius()-2.0f);
		}
		if (key.scancode == sf::Keyboard::Scancode::Escape)
    {
    }
	};
	std::function<void(const sf::Event::Resized)> onResize = [&window,&view,&background](const sf::Event::Resized& e)
	{
        
	};
	std::function<void(const sf::Event::FocusLost)> onLostFocus=[&window](const sf::Event::FocusLost&t)
	{
	};
	while (window.isOpen())
	{
        window.handleEvents(onResize,onClose);
        window.clear();
        window.draw(background);
        window.display();
	}
}