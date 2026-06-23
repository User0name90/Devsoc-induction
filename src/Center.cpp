#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include "button.hpp"
#include "engine.hpp"

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

	Button button1(sf::Texture("Images/image2.jpg"),{view.getCenter().x+230,view.getCenter().y+140},"gravity",sf::Color::Blue);
	Button button_add(sf::Texture("Images/plus.png"),{view.getCenter().x+230,view.getCenter().y+50},"Add Sphere",sf::Color::Green);
	Button button_delete(sf::Texture("Images/minus.png"),{view.getCenter().x+230,view.getCenter().y-40},"Add Sphere",sf::Color::Red);

	Engine eg(space);

	std::function<void(const sf::Event::Closed)> onClose = [&window](const sf::Event::Closed&)
	{
		window.close();
	};
	std::function<void(const sf::Event::MouseMoved)> onMouseMoved=[&window, &view,&button1,&button_add,&button_delete] (const sf::Event::MouseMoved& e)
	{
		sf::Vector2f mousepos=window.mapPixelToCoords(e.position);

		if(button1.state!=Button::State::Press)
		{
			if(button1.getBounds().contains(mousepos)) button1.mouseON();
			else button1.mouseOFF();
		}

		if(button_add.getBounds().contains(mousepos)) button_add.mouseON();
		else button_add.mouseOFF();

		if(button_delete.getBounds().contains(mousepos)) button_delete.mouseON();
		else button_delete.mouseOFF();
	};
	std::function<void(const sf::Event::MouseButtonPressed)> onMouseButtonPressed=[&window, &view,&button1,&eg,&button_add,&button_delete](const sf::Event::MouseButtonPressed& e)
	{
		sf::Vector2f mousepos=window.mapPixelToCoords(e.position);
		if(button1.getBounds().contains(mousepos))
		{
			
			if(button1.state==Button::State::Released)
			{	
				button1.mousePress();
				button1.state=Button::State::Press;
				eg.setGravity({0,45});
			}
			else
			{
				button1.mouseOFF();
				button1.state=Button::State::Released;
				eg.setGravity({0,0});
			}
		}
		if(button_add.getBounds().contains(mousepos))
		{
			button_add.mousePress();
			eg.createSphere();
		}
		if(button_delete.getBounds().contains(mousepos))
		{
			button_delete.mousePress();
			eg.deleteSphere(0);
		}
	};
	std::function<void(const sf::Event::MouseButtonReleased)> onMouseButtonReleased=[&window, &view,&button1,&eg,&button_add,&button_delete] (const sf::Event::MouseButtonReleased& e)
	{
		sf::Vector2f mousepos=window.mapPixelToCoords(e.position);
		if(button1.getBounds().contains(mousepos))
		{
			//nothing
		} 
		if(button_add.getBounds().contains(mousepos))
		{
			button_add.mouseOFF();
		} 
		if(button_delete.getBounds().contains(mousepos))
		{
			button_delete.mouseOFF();
		}
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
		// std::cout<<space.getOrigin().x<<' '<<space.getOrigin().y<<'\n';
	};
	std::function<void(const sf::Event::FocusLost)> onLostFocus=[&window](const sf::Event::FocusLost&t)
	{

	};
	eg.start();
	eg.createSphere();
	eg.createSphere();
	while (window.isOpen())
	{
        window.handleEvents(onResize,onClose,onMouseMoved,onMouseButtonPressed,onMouseButtonReleased);
        window.clear();
        window.draw(background);
		window.draw(space);
		window.draw(button1);
		window.draw(button_add);
		window.draw(button_delete);
		window.draw(eg);
        window.display();
	}
}