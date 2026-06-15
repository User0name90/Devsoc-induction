#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:
    ParticleSystem(unsigned int count) : m_particles(count), m_vertices(sf::PrimitiveType::Points, count)
    {
    }

    void setEmitter(sf::Vector2f position)
    {
        m_emitter = position;
    }
    void update(sf::Time elapsed)
    {
        for (std::size_t i = 0; i < m_particles.size(); ++i)
        {
            // update the particle lifetime
            Particle& p = m_particles[i];
            p.lifetime -= elapsed;

            // if the particle is dead, respawn it
            if (p.lifetime <= sf::Time::Zero)
                resetParticle(i);

            // update the position of the corresponding vertex
			p.velocity+=sf::Vector2f(0.7f,sf::degrees(90.0f));
            m_vertices[i].position += p.velocity * elapsed.asSeconds();
            // update the alpha (transparency) of the particle according to its lifetime
            float ratio           = p.lifetime.asSeconds() / m_lifetime.asSeconds();
            m_vertices[i].color.a = static_cast<std::uint8_t>(ratio * 255);
        }
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        // apply the transform
        states.transform *= getTransform();

        // our particles don't use a texture
        states.texture = nullptr;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    struct Particle
    {
        sf::Vector2f velocity;
        sf::Time     lifetime;
    };

    void resetParticle(std::size_t index)
    {
        // create random number generator
        static std::random_device rd;
        static std::mt19937       rng(rd());

        // give a random velocity and lifetime to the particle
        const sf::Angle angle       = sf::degrees(std::uniform_real_distribution(0.f, 360.f)(rng));
		ran_num=std::uniform_real_distribution(50.f, 100.f)(rng);
        const float     speed       = ran_num;
		if(ran_num>90)
		{
			int temp=255-20*(ran_num-90);
			m_vertices[index].color= sf::Color(temp,temp,255);
		}
		else if(ran_num<60)
		{
			int temp=255-20*(60-ran_num);
			m_vertices[index].color= sf::Color(255,temp,temp);
		}
        m_particles[index].velocity = sf::Vector2f(speed, angle);
        m_particles[index].lifetime = sf::milliseconds(std::uniform_int_distribution(1000, 3000)(rng));
        // reset the position of the corresponding vertex
        m_vertices[index].position = m_emitter;
    }

    std::vector<Particle> m_particles;
    sf::VertexArray       m_vertices;
    sf::Time              m_lifetime{sf::seconds(3)};
    sf::Vector2f          m_emitter;
	float ran_num=0;
};
// int main()
// {
// 	sf::RenderWindow window( sf::VideoMode( { 200, 200 } ), "SFML works!" );
// 	window.setVerticalSyncEnabled(true);
// 	window.setView(sf::View(sf::Vector2f(100.0f,100.0f),sf::Vector2f(200.0f,200.0f)));
// 	sf::CircleShape shape( 100.f );
// 	shape.setFillColor( sf::Color::Green );
// 	shape.setOutlineThickness(10.0f);

// 	shape.setOutlineColor(sf::Color(20,230,140));

// 	sf::Texture tex("src/image2.jpg");
// 	sf::Sprite sprite(tex,sf::IntRect({0, 0}, {960, 640}));
// 	window.draw(sprite);
// 	sprite.setColor(sf::Color( 0,255,0));

// 	sf::Font font("arial/ARIAL.TTF");

// 	sf::Text text(font); // a font is required to make a text object

// 	// set the string to display
// 	text.setString("H  e  llo world");

// 	// set the character size
// 	text.setCharacterSize(24); // in pixels, not points!

// 	// set the color
// 	text.setFillColor(sf::Color::Red);

// 	// set the text style
// 	text.setStyle(sf::Text::Bold | sf::Text::Underlined);
// 	text.move(sf::Vector2f(33.0,33.0));
// 	text.rotate(sf::degrees(30));
// 	text.scale(sf::Vector2f(1.5f,1.f));
// 	text.setOrigin({10,10});
// 	sf::FloatRect bounding=text.getGlobalBounds();
// 	sf::VertexArray d= sf::VertexArray(sf::PrimitiveType::LineStrip,5);
// 	d[0].position=bounding.position;
// 	d[1].position=bounding.position + sf::Vector2f(bounding.size.x,0.f) ;
// 	d[2].position=bounding.position + bounding.size;
// 	d[3].position=bounding.position + sf::Vector2f(0.f,bounding.size.y);
// 	d[4].position=bounding.position ;
	

// 	sf::VertexArray triangle(sf::PrimitiveType::Triangles, 3);

// 	// define the position of the triangle's points
// 	triangle[0].position = sf::Vector2f(10.f, 10.f);
// 	triangle[1].position = sf::Vector2f(100.f, 10.f);
// 	triangle[2].position = sf::Vector2f(100.f, 100.f);

// 	// define the color of the triangle's points
// 	triangle[0].color = sf::Color::Red;
// 	triangle[1].color = sf::Color::Blue;
// 	triangle[2].color = sf::Color::Green;

// 	std::function<void(const sf::Event::Closed)> onClose = [&window](const sf::Event::Closed&)
// 	{
// 		window.close();
// 	};

// 	std::function<void(const sf::Event::KeyPressed)> onKeyPressed =  [&window,&shape](const sf::Event::KeyPressed& key)
// 	{

// 		if(key.scancode == sf::Keyboard::Scancode::D)	
// 		{
// 			shape.move(sf::Vector2f(-2.0f,-2.0f));
// 			shape.setRadius(shape.getRadius()+2.0f);
// 		}
// 		else if(key.scancode == sf::Keyboard::Scancode::A)
// 		{
// 			shape.move(sf::Vector2f(2.0f,2.0f));
// 			shape.setRadius(shape.getRadius()-2.0f);
// 		}
// 		if (key.scancode == sf::Keyboard::Scancode::Escape)
//     {
//         std::cout << "the escape key was pressed" << std::endl;
//         std::cout << "scancode: " << static_cast<int>(key.scancode) << std::endl;
//         std::cout << "code: " << static_cast<int>(key.code) << std::endl;
//         std::cout << "control: " << key.control << std::endl;
//         std::cout << "alt: " << key.alt << std::endl;
//         std::cout << "shift: " << key.shift << std::endl;
//         std::cout << "system: " << key.system << std::endl;
//         std::cout << "description: " << sf::Keyboard::getDescription(key.scancode).toAnsiString() << std::endl;
//         std::cout << "localize: " << static_cast<int>(sf::Keyboard::localize(key.scancode)) << std::endl;
//         std::cout << "delocalize: " << static_cast<int>(sf::Keyboard::delocalize(key.code)) << std::endl;
//     }
// 	};
// 	std::function<void(const sf::Event::Resized)> onResize = [&window](const sf::Event::Resized& e)
// 	{
// 		window.setView(sf::View(sf::Vector2f(e.size.x/2,e.size.y/2),sf::Vector2f(e.size.x,e.size.y)));
// 	};
// 	std::function<void(const sf::Event::FocusLost)> onLostFocus=[&window](const sf::Event::FocusLost&t)
// 	{
// 		std::cout<<"Lost Focus :/"<<'\n';
// 	};
// 	while (window.isOpen())
// 	{
//     	window.handleEvents(onClose, onKeyPressed,onResize,onLostFocus);
// 		window.clear();
// 		window.draw(sprite);
// 		window.draw( triangle );
// 		window.draw(text);
// 		window.draw(d);
// 		window.display();
// 	}
// 	sf::Angle angle1=sf::radians(124);
// 	std::cout<<angle1.asDegrees() << ' '<<angle1.asRadians()<<'\n';
// }

// int main()
// {
//     // create the window
//     sf::RenderWindow window(sf::VideoMode({512, 256}), "Particles");

//     // create the particle system
//     ParticleSystem particles(3000);

//     // create a clock to track the elapsed time
//     sf::Clock clock;

//     // run the main loop
//     while (window.isOpen())
//     {
//         // handle events
//         while (const std::optional event = window.pollEvent())
//         {
//             if (event->is<sf::Event::Closed>())
//                 window.close();
//         }

//         // make the particle system emitter follow the mouse
//         sf::Vector2i mouse = sf::Mouse::getPosition(window);
//         particles.setEmitter(window.mapPixelToCoords(mouse));

//         // update it
//         sf::Time elapsed = clock.restart();
//         particles.update(elapsed);

//         // draw it
//         window.clear();
//         window.draw(particles);
//         window.display();
//     }
// }