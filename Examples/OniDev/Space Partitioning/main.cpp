#include <onidev.h>
#include "block.h"
#include "player.h"

int main()
{
    od::Window win(640, 480, "OniDev app");
    win.setSynchronization(true);
    
    auto& ic = od::InstanceContext<od::Object2d>::getGlobal();
    
    ic.setSpacePartitioning<Block, od::CollisionGrid>(128, 128, win.width()/128+1, win.height()/128+1);
    const od::CollisionGrid & sp = *static_cast<const od::CollisionGrid*>(ic.getSpacePartitioning(Block::ObjectIndex));
    
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
    
    od::framerateSetDelta(60);
    
    while(win.open())
    {
        win.ioHandle();
        
        if(od::keyPressed(od::vk_escape))
            break;
        
        od::drawClear();
        win.updateView();
        
        if(od::mousePressed(od::mb_left))
        {
            int x = win.mouse_x() / sp.cellWidth();
            int y = win.mouse_y() / sp.cellHeight();
            
            selected.clear();
            auto t = ic.retrieveInstances<Block>(x * sp.cellWidth(), y * sp.cellHeight(),
                        (x+1) * sp.cellWidth(), (y+1) * sp.cellHeight(), od::CollisionGrid::Intersection);
            for(Block * ii: t)
            {
                selected.push_back(ii->boundingBox());
            }
        }
        
        ic.update( 1.f );
        ic.render();
        
        for(size_t i=0; i<sp.width(); ++i)
        for(size_t j=0; j<sp.height(); ++j)
        {
            const size_t wid = sp.cellWidth();
            const size_t hei = sp.cellHeight();
            od::drawRectangle(i*wid, j*hei, (i+1)*wid, (j+1)*hei, true);
        }
        
        glColor3f(1, 0, 0);
        for(auto& r: selected)
        {
            od::drawRectangle(r.x1, r.y1, r.x2, r.y2, true);
        }
        
        win.screenRefresh();
        od::framerateUpdate();
    }
    
    return 0;
}
