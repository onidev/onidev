#include <onidev.h>

class Square:
    public od::AutoIndex<od::Object2d, Square>
{
public:
    Square(float x, float y):
        od::AutoIndex<od::Object2d, Square>(x, y) {}
    
    void predraw() const override {
        glColor3f(0, 0, 0);
        // one draw call for all instances (and not for each)
        glBegin(GL_QUADS);
    }
    
    void draw() const override {
        od::drawRectangleVertex(pos.x, pos.y, pos.x+4, pos.y+4);
    }
    
    void postdraw() const override {
        glEnd();
    }
};

int main()
{
    using namespace od;
    Window win(640, 480, "Predraw - Postdraw Example");
    win.setSynchronization(true);
    
    InstanceContext<Object2d> ic;
    
    for(int i=0; i<win.width(); i+=6)
    for(int j=0; j<win.height(); j+=6)
        od::instanceCreate<Square>(i, j);
    
    framerateSetDelta(60);
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        drawClear();
        win.updateView();
        
        ic.update(deltaTime());
        ic.render();
        
        framerateUpdate();
        win.screenRefresh();
    }
    
    return 0;
}
