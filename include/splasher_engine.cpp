#include"splasher_engine.hpp"
#include<iostream>
size_t SplasherEngine::Circular::count=0;

SplasherEngine::Circular::Circular(particle_splasher ps):ps{std::move(ps)},next{this}
{
    count++;
}
SplasherEngine::Circular::~Circular()
{
    count--;
}
void SplasherEngine::Circular::create_node(particle_splasher ps)
{
    Circular* temp=this->next;
    this->next=new Circular(std::move(ps));
    this->next->next=temp;
}
bool SplasherEngine::Circular::delete_next_node()
{
    if(count ==1) return false;
    Circular* temp= this->next->next;
    delete this->next;
    this->next=temp;
    return true;
}
SplasherEngine::Circular* SplasherEngine::Circular::advance(size_t index)
{
    Circular* temp=this;
    while(index>0)
    {
        temp=temp->next;
        index--;
    }
    return temp;
}
SplasherEngine::Circular* SplasherEngine::Circular::create_and_advance(particle_splasher ps)
{
    Circular* temp=this->next;
    this->next=new Circular(std::move(ps));
    this->next->next=temp;
    return this->next;
}
size_t SplasherEngine::Circular::getCount()
{
    return count;
}
SplasherEngine::SplasherEngine(sf:: Vector2f& gravity,sf::RectangleShape& rect): gravity{gravity},space{rect}
{
}
void SplasherEngine::createSplash(Sphere& sphere,sf::Vector2f pos,char wall)
{
    double mass_reduced;
    double original_mass=sphere.getMass();
    sf::Vector2f initial;
    double angle11 , angle12, angle21, angle22;

    if(wall=='x')
    {
        if(original_mass<2) mass_reduced=original_mass;
        else mass_reduced=reduction_factor*sphere.getMass()*std::abs(sphere.getVelocity().x);
        initial=sf::Vector2f(0,sphere.getVelocity().y);
        angle11=M_PI/2;
        angle21=M_PI*3/2;
        if(sphere.getVelocity().x>0)
        {
            angle12=angle11-atan2(std::abs(sphere.getVelocity().y),std::abs(sphere.getVelocity().x))+0.01;
            angle22=angle21+atan2(std::abs(sphere.getVelocity().y),std::abs(sphere.getVelocity().x))+0.01;
        }
        else
        {
            angle12=angle11+atan2(std::abs(sphere.getVelocity().y),std::abs(sphere.getVelocity().x));
            angle22=angle21-atan2(std::abs(sphere.getVelocity().y),std::abs(sphere.getVelocity().x));
        }
        sphere.setVelocity({static_cast<float>(1-reduction_factor)*sphere.getVelocity().x,sphere.getVelocity().y});
    }
    else if(wall=='y')
    {
        if(original_mass<2) mass_reduced=original_mass;
        else mass_reduced=reduction_factor*sphere.getMass()*std::abs(sphere.getVelocity().y);
        initial=sf::Vector2f(sphere.getVelocity().x,0);
        angle11=0;
        angle21=M_PI;
        if(sphere.getVelocity().y>0)
        {
            angle12=angle11+atan2(std::abs(sphere.getVelocity().x),std::abs(sphere.getVelocity().y));
            angle22=angle21-atan2(std::abs(sphere.getVelocity().x),std::abs(sphere.getVelocity().y));
        }
        else
        {
            angle12=angle11-atan2(std::abs(sphere.getVelocity().x),std::abs(sphere.getVelocity().y));
            angle22=angle21+atan2(std::abs(sphere.getVelocity().x),std::abs(sphere.getVelocity().y));
        }
        sphere.setVelocity({sphere.getVelocity().x,static_cast<float>(1-reduction_factor)*sphere.getVelocity().y});
    }
    else return;
    sphere.setMass(original_mass-mass_reduced);
    sphere.setRadius(sphere.getRadius()*sqrt(sphere.getMass()/original_mass));

    std::lock_guard<std::mutex> lg(list_mutex);
    if(head==nullptr)
    {
        head=new Circular(gravity);
    }
    int total=SplasherEngine::Circular::getCount();
    while(total!=0)
    {
        if(head->ps.isfinished())
        {
            std::cout<<"Going to run new thread. Total:"<<total<<'\n';
            if(head->thread.joinable())
            head->thread.join();
            head->thread=std::thread(&particle_splasher::start,&head->ps,sphere.getColor(),mass_to_particle*mass_reduced,pos,initial,sphere.getVelocity().length(),sf::radians(angle11),
            sf::radians(angle12),sf::radians(angle21),sf::radians(angle22),space.getGlobalBounds());

            std::cout<<"Thread created Successfully"<<'\n';
            break;
        }
        else
        {
            head=head->advance(1);
        }
        total--;
    }
    if(total==0)
    {
        head=head->create_and_advance(gravity);
        head->thread=std::thread(&particle_splasher::start,&head->ps,sphere.getColor(),mass_to_particle*mass_reduced,pos,initial,sphere.getVelocity().length(),sf::radians(angle11),
        sf::radians(angle12),sf::radians(angle21),sf::radians(angle22),space.getGlobalBounds());
    }
}
void SplasherEngine::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    std::lock_guard<std::mutex> lg(list_mutex);
    int total=SplasherEngine::Circular::getCount();
    while(total!=0)
    {
        if(!head->ps.isfinished()) target.draw(head->ps,states);
        head=head->advance(1);
        total--;
    }
}
void SplasherEngine::createSplash(Sphere& sphere1,Sphere& sphere2,float& v1n,float& v2n,float& v1t, float& v2t ,const sf::Vector2f& pos,sf::Vector2f& normal,sf::Vector2f& tangent)
{
    double mass_reduced1 , mass_reduced2;
    double original_mass1,original_mass2;
    double angle111,angle112,angle121,angle122,angle211,angle212,angle221,angle222;
    sf::Vector2f vn1=normal*v1n;
    sf::Vector2f vn2=normal*v2n;
    sf::Vector2f vt1=tangent*v1t;
    sf::Vector2f vt2=tangent*v2t;
    original_mass1=sphere1.getMass();
    original_mass2=sphere2.getMass();
    if(original_mass1<2) mass_reduced1=original_mass1;
    else mass_reduced1=reduction_factor*std::abs(vn1.length()-vn2.length())*sphere1.getMass();
    if(original_mass2<2) mass_reduced2=original_mass2;
    else mass_reduced2=reduction_factor*std::abs(vn1.length()-vn2.length())*sphere2.getMass();

    angle111=vn1.angle().asRadians()+M_PI_2;
    angle112=vn1.angle().asRadians()+M_PI_2;
    angle121=vn1.angle().asRadians()-M_PI_2;
    angle122=vn1.angle().asRadians()-M_PI_2;

    angle211=vn2.angle().asRadians()+M_PI_2;
    angle212=vn2.angle().asRadians()+M_PI_2;
    angle221=vn2.angle().asRadians()-M_PI_2;
    angle222=vn2.angle().asRadians()-M_PI_2;

    double deviation=atan2(vt1.length(),vn1.length());

    angle111-=deviation;
    angle112+=deviation;
    angle121-=deviation;
    angle122+=deviation;

    deviation=atan2(vt2.length(),vn2.length());

    angle211-=deviation;
    angle212+=deviation;
    angle221-=deviation;
    angle222+=deviation;

    
    sphere1.setMass(original_mass1-mass_reduced1);
    sphere2.setMass(original_mass2-mass_reduced2);

    sphere1.setRadius(sphere1.getRadius()*sqrt(sphere1.getMass()/original_mass1));
    sphere2.setRadius(sphere2.getRadius()*sqrt(sphere2.getMass()/original_mass2));

    std::cout << "After set - Mass1: " << sphere1.getMass() << " Radius1: " << sphere1.getRadius() << '\n';
    std::cout << "After set - Mass2: " << sphere2.getMass() << " Radius2: " << sphere2.getRadius() << '\n';

    std::lock_guard<std::mutex> lg(list_mutex);
    if(head==nullptr)
    {
        head=new Circular(gravity);
    }
    v1n*=(float)(1-reduction_factor);
    v2n*=(float)(1-reduction_factor);
    //
    std::cout<<"Calculation for splash done"<<'\n';
    //

    if(SplasherEngine::Circular::getCount()<2) head->create_node(gravity);
    int total=SplasherEngine::Circular::getCount();
    bool first_ran=false,second_ran=false;
    while(total!=0)
    {
        std::cout<<"Entered first loop. Total "<<total<<'\n';
        if(head->ps.isfinished())
        {
            if(head->thread.joinable()) head->thread.join();
            head->thread=std::thread(&particle_splasher::start,&head->ps,sphere1.getColor(),mass_to_particle*mass_reduced1,pos,sphere1.getVelocity(),sphere1.getVelocity().length(),sf::radians(angle111),
            sf::radians(angle112),sf::radians(angle121),sf::radians(angle122),space.getGlobalBounds());
            std::cout<<"Thread ran"<<'\n';
            first_ran=true;
            head=head->advance(1);
            total--;
            break;
        }
        else
        {
            head=head->advance(1);
        }
        total--;
    }
    while(total!=0)
    {
        std::cout<<"Entered second loop. Total "<<total<<'\n';
        if(head->ps.isfinished())
        {
            if(head->thread.joinable()) head->thread.join();
            head->thread=std::thread(&particle_splasher::start,&head->ps,sphere2.getColor(),mass_to_particle*mass_reduced2,pos,sphere2.getVelocity(),sphere2.getVelocity().length(),sf::radians(angle211),
            sf::radians(angle212),sf::radians(angle221),sf::radians(angle222),space.getGlobalBounds());
            std::cout<<"Thread ran"<<'\n';
            second_ran=true;
            head=head->advance(1);
            total--;
            break;
        }
        else
        {
            head=head->advance(1);
        }
        total--;
    }
    
    if(!first_ran)
    {
        head=head->create_and_advance(gravity);
        head->thread=std::thread(&particle_splasher::start,&head->ps,sphere1.getColor(),mass_to_particle*mass_reduced1,pos,sphere1.getVelocity(),sphere1.getVelocity().length(),sf::radians(angle111),
        sf::radians(angle112),sf::radians(angle121),sf::radians(angle122),space.getGlobalBounds());
        head=head->advance(1);
    }
    if(!second_ran)
    {
        head=head->create_and_advance(gravity);
        head->thread=std::thread(&particle_splasher::start,&head->ps,sphere2.getColor(),mass_to_particle*mass_reduced2,pos,sphere2.getVelocity(),sphere2.getVelocity().length(),sf::radians(angle211),
        sf::radians(angle212),sf::radians(angle221),sf::radians(angle222),space.getGlobalBounds());
        head=head->advance(1);
    }
}
void SplasherEngine::stop()
{
    for(int i=0;i<SplasherEngine::Circular::getCount();i++)
    {
        head->ps.stop();
        head=head->advance(1);
    }
}
void SplasherEngine::resume()
{
    for(int i=0;i<SplasherEngine::Circular::getCount();i++)
    {
        head->ps.resume();
        head=head->advance(1);
    }
    
}
void SplasherEngine::terminate()
{
    if(head==nullptr) return;
    int total=SplasherEngine::Circular::getCount();
    
    for(int i=0;i<total;i++)
    {
        head->ps.resume();
        head->ps.terminate();
        head=head->advance(1);
    }
    for(int i=0;i<total;i++)
    {
        if(head->thread.joinable())
        head->thread.join();
        head=head->advance(1);
    }
    while(total>1)
    {
        head->delete_next_node();
        total--;
    }
}
SplasherEngine::~SplasherEngine()
{
    terminate();
    if(head!=nullptr) delete head;
}