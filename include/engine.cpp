#include "engine.hpp"

Engine::Engine(sf::RectangleShape& rect):space{rect}, randomEngine{std::random_device{}()}
{

}
 void Engine::deleteSphere(int index)
{
    if(index>=spheres.size()) return;
    std::list<Sphere>::iterator it= spheres.begin();
    std::advance(it,index);
    spheres.erase(it);
}
void Engine::createSphere()
{
    //yet to do 
    double max_x=this->space.getGlobalBounds().getCenter().x + this->space.getGlobalBounds().size.x/2;
    double min_x=this->space.getGlobalBounds().getCenter().x - this->space.getGlobalBounds().size.x/2;
    double min_y=this->space.getGlobalBounds().getCenter().y -this->space.getGlobalBounds().size.y/2;
    double max_y=this->space.getGlobalBounds().getCenter().y + this->space.getGlobalBounds().size.y/2;
    sf::Vector2f velocity;
    sf::Vector2f position;
    distribution.param(std::uniform_real_distribution<double>::param_type(10,100));
    double radius=distribution(this->randomEngine);

    distribution.param(std::uniform_real_distribution<double>::param_type(min_x+radius,max_x-radius));
    position.x=distribution(randomEngine);

    distribution.param(std::uniform_real_distribution<double>::param_type(min_y+radius,max_y-radius));
    position.y=distribution(randomEngine);

    distribution.param(std::uniform_real_distribution<double>::param_type(-20.0,20.0));
    velocity.x=distribution(this->randomEngine);
    velocity.y=distribution(this->randomEngine);

    distribution.param(std::uniform_real_distribution<double>::param_type(1,75));
    double mass=distribution(this->randomEngine);
    spheres.push_back(Sphere(mass,radius,position,velocity));
}