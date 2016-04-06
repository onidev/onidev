#include <SFML/Graphics.hpp>
#include <onidev/game.h>
#include "player.h"
#include "block.h"

int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    
    sf::RenderWindow window(sf::VideoMode(640, 480), "Space Partitioning Example", sf::Style::Close, contextSettings);
    window.setVerticalSyncEnabled(true);
    window.setActive();
    
    od::InstanceContext<sfObject> ic;
    ic.setSpacePartitioning<Block, sfCollisionGrid>(128, 128, 640/128+1, 480/128+1);
    const sfCollisionGrid & sp = *static_cast<const sfCollisionGrid*>(ic.getSpacePartitioning(Block::ObjectIndex));
    
    // optimized selection demo
    std::vector<od::Rect> selected;    
    
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
    
    for(int i=16*3; i<624; i+=16*3)
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
            
            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                int x = event.mouseButton.x / sp.cellWidth();
                int y = event.mouseButton.y / sp.cellHeight();
                
                selected.clear();
                auto t = ic.retrieveInstances<Block>(x * sp.cellWidth(), y * sp.cellHeight(),
                            (x+1) * sp.cellWidth(), (y+1) * sp.cellHeight(), od::CollisionGrid::Intersection);
                for(Block* ii: t)
                {
                    selected.push_back(ii->boundingBox());
                }
            }
        }
        
        window.clear(sf::Color(150, 150, 150, 0));
        
        ic.update(1.f);
        ic.render(true, window);
        
        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(sp.cellWidth(), sp.cellHeight()));
        rectangle.setFillColor(sf::Color(0, 0, 0, 0));
        rectangle.setOutlineThickness(1);
        rectangle.setOutlineColor(sf::Color(128, 255, 128));
        
        for(size_t i=0; i<sp.width(); ++i)
        for(size_t j=0; j<sp.height(); ++j)
        {
            rectangle.setPosition(sf::Vector2f(i*sp.cellWidth(), j*sp.cellHeight()));
            window.draw(rectangle);
        }
        
        rectangle.setOutlineColor(sf::Color(255, 0, 0));
        for(auto& r: selected)
        {
            rectangle.setPosition(sf::Vector2f(r.x1, r.y1));
            rectangle.setSize(sf::Vector2f(r.width(), r.height()));
            window.draw(rectangle);
        }
        
        window.display();
    }
    
    return 0;
}
