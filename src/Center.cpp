#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include "button.hpp"
#include "engine.hpp"

int main()
{
// 	sf::RenderTexture rt({40u, 35u});  // pass size in constructor
// 	rt.clear(sf::Color::Transparent);

// 	// Horizontal bar
// sf::RectangleShape hBar({30.f, 7.f});
// hBar.setPosition({5.f, 14.f});
// hBar.setFillColor(sf::Color::White);
// rt.draw(hBar);

// // Vertical bar
// sf::RectangleShape vBar({7.f, 25.f});
// vBar.setPosition({16.5f, 5.f});
// vBar.setFillColor(sf::Color::White);
// rt.draw(vBar);

// 	rt.display();
// 	rt.getTexture().copyToImage().saveToFile("Images/plus.png");

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
	Button button_play(sf::Texture("Images/play.png"),{view.getCenter().x+100,view.getCenter().y+160},"Play/Pause",sf::Color::Yellow);
	Button button_debug(sf::Texture("Images/debug.png"),{view.getCenter().x-30,view.getCenter().y+160},"Dubug",sf::Color::Cyan);
	Button button_snowball(sf::Texture("Images/splash2.png"),{view.getCenter().x-130,view.getCenter().y+160},"Snowball",sf::Color::Magenta);
	Engine eg(space);

	std::function<void(const sf::Event::Closed)> onClose = [&window](const sf::Event::Closed&)
	{
		window.close();
	};
	std::function<void(const sf::Event::MouseMoved)> onMouseMoved=[&window, &view,&button1,&button_add,&button_delete,&button_play,&button_debug,&button_snowball] (const sf::Event::MouseMoved& e)
	{
		sf::Vector2f mousepos=window.mapPixelToCoords(e.position);

		if(button1.state!=Button::State::Press)
		{
			if(button1.getBounds().contains(mousepos)) button1.mouseON();
			else button1.mouseOFF();
		}
		if(button_play.state!=Button::State::Press)
		{
			if(button_play.getBounds().contains(mousepos)) button_play.mouseON();
			else button_play.mouseOFF();
		}
		if(button_debug.state!=Button::State::Press)
		{
			if(button_debug.getBounds().contains(mousepos)) button_debug.mouseON();
			else button_debug.mouseOFF();
		}
		if(button_snowball.state!=Button::State::Press)
		{
			if(button_snowball.getBounds().contains(mousepos)) button_snowball.mouseON();
			else button_snowball.mouseOFF();
		}

		if(button_add.getBounds().contains(mousepos)) button_add.mouseON();
		else button_add.mouseOFF();

		if(button_delete.getBounds().contains(mousepos)) button_delete.mouseON();
		else button_delete.mouseOFF();
	};
	std::function<void(const sf::Event::MouseButtonPressed)> onMouseButtonPressed=[&window, &view,&button1,&eg,&button_add,&button_delete, &button_play,&button_debug,&button_snowball](const sf::Event::MouseButtonPressed& e)
	{
		sf::Vector2f mousepos=window.mapPixelToCoords(e.position);
		if(button1.getBounds().contains(mousepos))
		{
			
			if(button1.state==Button::State::Released)
			{	
				button1.mousePress();
				button1.state=Button::State::Press;
				eg.setGravity({0,60});
			}
			else
			{
				button1.mouseOFF();
				button1.state=Button::State::Released;
				eg.setGravity({0,0});
			}
		}

		if(button_play.getBounds().contains(mousepos))
		{
			
			if(button_play.state==Button::State::Released)
			{	
				button_play.mousePress();
				button_play.state=Button::State::Press;
				eg.stop();
			}
			else
			{
				button_play.mouseOFF();
				button_play.state=Button::State::Released;
				eg.resume();
			}
		}

		if(button_debug.getBounds().contains(mousepos))
		{
			
			if(button_debug.state==Button::State::Released)
			{	
				button_debug.mousePress();
				button_debug.state=Button::State::Press;
				eg.setDebug();
			}
			else
			{
				button_debug.mouseOFF();
				button_debug.state=Button::State::Released;
				eg.clearDebug();
			}
		}

		if(button_snowball.getBounds().contains(mousepos))
		{
			
			if(button_snowball.state==Button::State::Released)
			{	
				button_snowball.mousePress();
				button_snowball.state=Button::State::Press;
				eg.setSnowball();
			}
			else
			{
				button_snowball.mouseOFF();
				button_snowball.state=Button::State::Released;
				eg.clearSnowball();
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
	std::function<void(const sf::Event::MouseButtonReleased)> onMouseButtonReleased=[&window, &view,&button1,&eg,&button_add,&button_delete,&button_play,&button_debug,&button_snowball] (const sf::Event::MouseButtonReleased& e)
	{
		sf::Vector2f mousepos=window.mapPixelToCoords(e.position);
		// if(button1.getBounds().contains(mousepos))
		// {
		// 	//nothing
		// } 
		// if(button_play.getBounds().contains(mousepos))
		// {
		// 	//nothing
		// }
		// if(button_debug.getBounds().contains(mousepos))
		// {
		// 	//nothing
		// }
		// if(button_snowball.getBounds().contains(mousepos))
		// {
		// 	//nothing
		// }
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
		if(eg.testSnowball())
		{
			eg.stop();
			std::unique_lock<std::shared_mutex> ul(eg.spheres_list_mutex);
			eg.spheres.remove_if([](const Sphere& s){ return s.isDead();});
			eg.resume();
		}
        window.handleEvents(onResize,onClose,onMouseMoved,onMouseButtonPressed,onMouseButtonReleased);
        window.clear();
        window.draw(background);
		window.draw(space);
		window.draw(button1);
		window.draw(button_add);
		window.draw(button_delete);
		window.draw(button_play);
		window.draw(button_debug);
		window.draw(button_snowball);
		window.draw(eg);
        window.display();
	}
}