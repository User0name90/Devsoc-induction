#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include "button.hpp"


int main()
{
	sf::RenderWindow window( sf::VideoMode( { 600, 400 } ), "Engine" );
	window.setVerticalSyncEnabled(true);
    sf::View view;
    view.setSize({600,400});
	view.setCenter({300,200});
	window.setView(view);


	sf::RectangleShape space({500,300});
	space.setOrigin(space.getLocalBounds().getCenter());
	space.setFillColor(sf::Color(0,0,255,100));
	space.setPosition(view.getCenter()-sf::Vector2f(+50.f,+50.f));

	sf::Texture tex("Images/concrete.jpg");
	sf::Sprite background(tex);
    background.setScale({view.getSize().x/(float)tex.getSize().x, view.getSize().y/(float)tex.getSize().y});
	window.draw(background);

	Button button1(sf::Texture("Images/image2.jpg"),{view.getCenter().x+260,view.getCenter().y+170},"gravity");

	std::function<void(const sf::Event::Closed)> onClose = [&window](const sf::Event::Closed&)
	{
		window.close();
	};
	std::function<void(const sf::Event::MouseMoved)> onMouseMoved=[&window, &view,&button1] (const sf::Event::MouseMoved& e)
	{
		sf::Vector2f mousepos=window.mapPixelToCoords(e.position);
		if(button1.getBounds().contains(mousepos)) button1.mouseON();
		else button1.mouseOFF();
	};
	std::function<void(const sf::Event::MouseButtonPressed)> onMouseButtonPressed=[&window, &view,&button1] (const sf::Event::MouseButtonPressed& e)
	{
		sf::Vector2f mousepos=window.mapPixelToCoords(e.position);
		if(button1.getBounds().contains(mousepos)) button1.mousePress();
		else button1.mouseOFF();
	};
	std::function<void(const sf::Event::MouseButtonReleased)> onMouseButtonReleased=[&window, &view,&button1] (const sf::Event::MouseButtonReleased& e)
	{
		sf::Vector2f mousepos=window.mapPixelToCoords(e.position);
		if(button1.getBounds().contains(mousepos)) button1.mouseOFF();
	};
	std::function<void(const sf::Event::KeyPressed)> onKeyPressed =  [&window](const sf::Event::KeyPressed& key)
	{

	};
	std::function<void(const sf::Event::Resized)> onResize = [&window,&view,&background,&tex,&space](const sf::Event::Resized& e)
	{
		view.setCenter(view.getCenter() + sf::Vector2f((view.getSize().x-e.size.x)/2,(view.getSize().y-e.size.y)/2));
		space.move({view.getSize().x-e.size.x,view.getSize().y -e.size.y});	
        view.setSize({(float)e.size.x, (float)e.size.y});
		//space.setPosition();
		space.setSize({(float)e.size.x-100,(float)e.size.y-100});
		window.setView(view);
		background.setPosition({view.getCenter().x-(float)view.getSize().x/2,view.getCenter().y - (float)view.getSize().y/2});
		//background.setOrigin(view.getCenter());
		background.setScale({view.getSize().x/(float)tex.getSize().x, view.getSize().y/(float)tex.getSize().y});
	};
	std::function<void(const sf::Event::FocusLost)> onLostFocus=[&window](const sf::Event::FocusLost&t)
	{
	};
	
	while (window.isOpen())
	{
        window.handleEvents(onResize,onClose,onMouseMoved,onMouseButtonPressed,onMouseButtonReleased);
        window.clear();
        window.draw(background);
		window.draw(space);
		window.draw(button1);
        window.display();
	}
}