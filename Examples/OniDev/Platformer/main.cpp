#include <onidev.h>
#include "world.h"
#include "player.h"

int main()
{
    od::Window win(640, 480, "Platformer Example");
    win.setSynchronization(true);
    
    glClearColor(0.3, 0.3, 0.3, 0);
    
    od::InstanceContext<od::Object2d> world;
    
    auto tilemap = std::make_shared<World>(od::Image("map.png"));
    world.addLayer(tilemap);
    
    od::instanceCreate<Player>(48, 48);
    
    od::framerateSetDelta(60);
    
    while(win.open())
    {
        win.ioHandle();
        
        if(od::keyPressed(od::vk_escape))
            break;
        
        od::drawClear();
        win.updateView();
        
        world.update( od::deltaTime() );
        world.render();
        tilemap->draw();
        
        win.screenRefresh();
        od::framerateUpdate();
    }
    
    return 0;
}
