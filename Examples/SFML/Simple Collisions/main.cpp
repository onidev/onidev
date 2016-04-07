#include <SFML/Graphics.hpp>
#include <onidev/game.h>
#include "object.h"


class Block:
    public od::AutoIndex<sfObject, Block>
{
public:
    Block(float x, float y):
        od::AutoIndex<sfObject, Block>(x, y) {}
    
    od::Shape * collisionMask(int p) const override {
        static od::Rect rect(0, 0, 16, 16);
        return &rect;
    }
    
    void draw(sf::RenderTarget& render_target) const override {
        sf::RectangleShape rectangle;
        rectangle.setPosition(sf::Vector2f(pos.x, pos.y));
        rectangle.setSize(sf::Vector2f(16, 16));
        rectangle.setFillColor(sf::Color(255, 255, 255));
        render_target.draw(rectangle);
    }
};

class Player:
    public od::AutoIndex<sfObject, Player>
{
public:
    Player(float x, float y):
        od::AutoIndex<sfObject, Player>(x, y) {}
    
    od::Shape * collisionMask(int p) const override {
        static od::Rect rect(0, 0, 16, 16);
        return &rect;
    }
    
    void step(float dt) override;
    void draw(sf::RenderTarget& render_target) const override {
        sf::RectangleShape rectangle;
        rectangle.setPosition(sf::Vector2f(pos.x, pos.y));
        rectangle.setSize(sf::Vector2f(16, 16));
        rectangle.setFillColor(sf::Color(0, 128, 0));
        render_target.draw(rectangle);
    }
};

void Player::step(float dt)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !placeMeeting<Block>(pos.x-dt, pos.y)) pos.x -= dt;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)&& !placeMeeting<Block>(pos.x+dt, pos.y)) pos.x += dt;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)   && !placeMeeting<Block>(pos.x, pos.y-dt)) pos.y -= dt;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !placeMeeting<Block>(pos.x, pos.y+dt)) pos.y += dt;
}


int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    
    sf::RenderWindow window(sf::VideoMode(640, 480), "Collisions example", sf::Style::Close, contextSettings);
    window.setVerticalSyncEnabled(true);
    window.setActive();
    
    od::InstanceContext<sfObject> ic;
    
    od::instanceCreate<Player>(16*2, 16*2);
    
    for(int i=0; i<640; i+=16)
    {
        od::instanceCreate<Block>(i, 0);
        od::instanceCreate<Block>(i, 480 - 16);
    }
    for(int j=16; j<480; j+=16)
    {
        od::instanceCreate<Block>(0, j);
        od::instanceCreate<Block>(640 - 16, j);
    }
    
    for(int i=16*3; i<640; i+=16*3)
    for(int j=16*3; j<480; j+=16*3)
        od::instanceCreate<Block>(i, j-8);
    
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
