#include "engine.hpp"
#include <iostream>
Engine::Engine(sf::RectangleShape& rect):space{rect}, randomEngine{std::random_device{}()},sound_buffer{"Audio/collide.mp3"},collision_sound{sound_buffer}
{
    collision_sound.setPlayingOffset(sf::seconds(0.1f));
}
Engine::~Engine()
{
    terminate();
}
 void Engine::deleteSphere(size_t index)
{
    if(index>=spheres.size())return;
    if(stop_flag.test_and_set() || terminate_flag.test()) return;
    std::unique_lock<std::shared_mutex> ul(spheres_list_mutex);

    if(index>=spheres.size()) return;
    std::list<Sphere>::iterator it= spheres.begin();
    std::advance(it,index);
    spheres.erase(it);
    
    stop_flag.clear();
    stop_flag.notify_all();
}
void Engine::createSphere()
{
    if(stop_flag.test_and_set() || terminate_flag.test()) return;
    std::unique_lock<std::shared_mutex> ul(spheres_list_mutex);
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

    distribution.param(std::uniform_real_distribution<double>::param_type(-70.0,70.0));
    velocity.x=distribution(this->randomEngine);
    velocity.y=distribution(this->randomEngine);

    distribution.param(std::uniform_real_distribution<double>::param_type(1,75));
    double mass=distribution(this->randomEngine);
    spheres.push_back(Sphere(mass,radius,position,velocity));
    if(mass<=36)
    {
        spheres.back().setColor({0,uint8_t(mass/36*255),uint8_t(255 - mass/36*255)});
    }
    else
    {
        spheres.back().setColor({uint8_t((mass-36)/36*255),uint8_t(255-(mass-36)/36*255),0});
    }
    stop_flag.clear();
    stop_flag.notify_all();
    //Remove this
                    std::cout<<position.x<< ' '<<position.y<<'\n';
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
            //remove this
}

void Engine::checkBounds()
{
    std::shared_lock<std::shared_mutex> sl(spheres_list_mutex);
    std::list<Sphere>::iterator it;

    while(1)
    {
        while(stop_flag.test())
        {
            sl.unlock();
            stop_flag.wait(true);
        }
        if(terminate_flag.test()) return;
        
        if(!sl.owns_lock()) sl.lock();
        for(it=spheres.begin();it!=spheres.end();it++)
        {
            it->mutex->lock();
            sf::Vector2f vel=it->getVelocity();
            if(it->getPosition().x-it->getRadius()<=space.getGlobalBounds().position.x && it->getVelocity().x<=0)
            {
                it->setVelocity({-vel.x,vel.y});
                sound_amp=std::abs(vel.x/1.4);
                playSound();
            }
            if(it->getPosition().x+it->getRadius()>=space.getGlobalBounds().position.x+space.getSize().x && it->getVelocity().x>=0)
            {
               it->setVelocity({-vel.x,vel.y}); 
               sound_amp=std::abs(vel.x/1.40);
               playSound();
            }
            if(it->getPosition().y-it->getRadius()<=space.getGlobalBounds().position.y && it->getVelocity().y<=0)
            {
                it->setVelocity({vel.x,-vel.y});
                sound_amp=std::abs(vel.y/1.40);
                playSound();
            }
            if(it->getPosition().y+it->getRadius()>=space.getGlobalBounds().position.y+space.getSize().y && it->getVelocity().y>=0)
            {
                it->setVelocity({vel.x,-vel.y});
                sound_amp=std::abs(vel.y/1.40);
                playSound();
            }
            it->mutex->unlock();

            if(spheres.size()==1)
            {
                std::this_thread::sleep_for(std::chrono::microseconds(500));
            }
        }
    }
}
void Engine::checkCollision()
{
    std::shared_lock<std::shared_mutex> sl(spheres_list_mutex);
    std::list<Sphere>::iterator it_first,it_second;
    
    double mass_sum,mass_dif,mass_ratio;
    float v1n,v2n,v1t,v2t,v1n_new,v2n_new,distance;
    sf::Vector2f vel1,vel2;
    sf::Vector2f normal;
    sf::Vector2f tangent;
    while(1)
    {
        while(stop_flag.test())
        {
            sl.unlock();
            stop_flag.wait(true);
        }
        if(terminate_flag.test()) return;
        
        if(!sl.owns_lock()) sl.lock();
        
        for(it_first=spheres.begin();it_first!= --spheres.end();it_first++)
        {
            for(it_second=std::next(it_first);it_second!=spheres.end();it_second++)
            {
                std::scoped_lock lock(*it_first->mutex,*it_second->mutex);
                normal=(it_second->getPosition()- it_first->getPosition());

                distance=normal.length();
                normal/=distance;
                tangent = normal.rotatedBy(sf::degrees(90));
                sf::Vector2f vel1=it_first->getVelocity();
                sf::Vector2f vel2=it_second->getVelocity();

                v1n=vel1.dot(normal);
                v2n=vel2.dot(normal);

                v1t=vel1.dot(tangent);
                v2t=vel2.dot(tangent);
                if(std::abs(distance)<=it_first->getRadius()+it_second->getRadius() && v1n-v2n>0.f)
                {
                    tangent=normal.rotatedBy(sf::degrees(90));
                    
                    mass_sum=it_first->getMass()+it_second->getMass();
                    mass_dif=it_first->getMass()-it_second->getMass();
                    

                    v1n_new=(mass_dif/mass_sum)*v1n + (2*it_second->getMass()/mass_sum)*v2n;
                    v2n_new=((2*it_first->getMass())/mass_sum)*v1n + (-mass_dif/mass_sum)*v2n;

                    it_first->setVelocity(v1n_new*normal+v1t*tangent);
                    it_second->setVelocity(v2n_new*normal+v2t*tangent);

                    sound_amp=std::abs(v1n-v2n);
                    playSound();
                    
                }
            }
        }
 
    }
}

void Engine::uniformMotion()
{
    std::shared_lock<std::shared_mutex> sl(spheres_list_mutex);
    std::list<Sphere>::iterator it;

    auto begin=std::chrono::steady_clock::now();
    bool just_paused=false;
    for(it=spheres.begin();it!=spheres.end();it++)
    {
        it->last_update=begin;
    }
    float dt;
    while(1)
    {
        while(stop_flag.test())
        {
            sl.unlock();
            stop_flag.wait(true);
            just_paused=true;
        }
        if(terminate_flag.test()) return;
        
        if(!sl.owns_lock()) sl.lock();
        if(just_paused)
        {
            begin=std::chrono::steady_clock::now();   
            for(it=spheres.begin();it!=spheres.end();it++)
            {
                it->last_update=begin;
            }
            just_paused=false;
        }
       
        for(it=spheres.begin();it!=spheres.end();it++)
        {
            it->mutex->lock();

            begin=std::chrono::steady_clock::now();
            dt=std::chrono::duration<float>(begin-it->last_update).count();

            it->last_update=begin;
            it->setPosition(it->getPosition()+it->getVelocity()*dt + gravity*dt*dt/2.0f);
            it->setVelocity(it->getVelocity()+gravity*dt);
            it->mutex->unlock();

            if(spheres.size()==1)
            {
                std::this_thread::sleep_for(std::chrono::microseconds(500));
            }
        }
    }
}
void Engine::resume()
{

    stop_flag.clear();
    stop_flag.notify_all();
}
void Engine::stop()
{
    stop_flag.test_and_set();
}
void Engine::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if(terminate_flag.test()) return;
    std::shared_lock<std::shared_mutex> sl(spheres_list_mutex);
    for(const Sphere& sp :spheres)
    {
        target.draw(sp);
    }
}
void Engine::start()
{
    spheres.clear();
    stop_flag.clear();
    terminate_flag.clear();
    collision_thread=std::thread(&Engine::checkCollision,this);
    uniform_thread  =std::thread(&Engine::uniformMotion,this);
    bounds_thread   =std::thread(&Engine::checkBounds,this);
}
void Engine::terminate()
{
    terminate_flag.test_and_set();
    stop_flag.clear();
    stop_flag.notify_all();
    collision_thread.join();
    uniform_thread.join();
    bounds_thread.join();
    spheres.clear();
    stop_flag.test_and_set();
    //yet to do
}
void Engine::setGravity(sf::Vector2f gravity)
{
    this->gravity=gravity;
}
void Engine::playSound()
{
    collision_sound.setVolume(sound_amp);
    collision_sound.play();
}