#include <SFML/Graphics.hpp>
#include <onidev/game.h>
#include "object.h"

class Player:
    public od::AutoIndex<sfObject, Player>
{
public:
    Player(float x, float y):
        od::AutoIndex<sfObject, Player>(x, y) {}
    
    od::Shape* collisionMask(int p) const override {
        static od::Rect rect(0, 0, 16, 16);
        return &rect;
    }
    
    void step(float dt) override;
    void draw(sf::RenderTarget& render_target) const override
    {
        sf::RectangleShape rectangle;
        rectangle.setPosition(sf::Vector2f(pos.x, pos.y));
        rectangle.setSize(sf::Vector2f(16, 16));
        render_target.draw(rectangle);
    }
};

void Player::step(float dt)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) pos.x -= dt;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) pos.x += dt;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) pos.y -= dt;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) pos.y += dt;
}

int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    
    sf::RenderWindow window(sf::VideoMode(640, 480), "Instance Context Example", sf::Style::Close, contextSettings);
    window.setVerticalSyncEnabled(true);
    window.setActive();
    
    od::InstanceContext<sfObject> ic;
    od::instanceCreate<Player>(64, 64);
    
    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if((event.type == sf::Event::Closed)
            ||((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
                window.close();
        }
        
        window.clear(sf::Color(150, 150, 150, 0));
        
        ic.update(1.f);
        ic.render(true, window);
        
        window.display();
    }
    
    return 0;
}
