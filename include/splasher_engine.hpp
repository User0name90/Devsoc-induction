#include <SFML/Graphics.hpp>
#include "sphere.hpp"
#include "particle_splasher.hpp"
#include <thread>
#include <mutex>

class SplasherEngine : public sf::Drawable
{
    private:
    class Circular
    {
        private:
        static size_t count;
        Circular* next;
        public:
        std::thread thread;
        particle_splasher ps;
        Circular(particle_splasher ps);
        ~Circular();
        void create_node(particle_splasher);
        bool delete_next_node();
        static size_t getCount();
        Circular* advance(size_t index);
        SplasherEngine::Circular* create_and_advance(particle_splasher);
        
    };
    mutable std::mutex list_mutex;
    mutable Circular* head=nullptr;
    sf::RectangleShape& space;
    sf::Vector2f& gravity;
    const double reduction_factor=0.05;
    const unsigned int mass_to_particle=30;
    public:
    SplasherEngine(sf::Vector2f& gravity,sf::RectangleShape& rect);
    ~SplasherEngine();
    void createSplash(Sphere& sphere,sf::Vector2f,char wall);
    void createSplash(Sphere& sphere1,Sphere& sphere2,float& vn1,float& vn2,float& vt1,float& vt2,const sf::Vector2f& pos,sf::Vector2f& normal,sf::Vector2f& tangent);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void stop();
    void resume();
    void terminate();
};