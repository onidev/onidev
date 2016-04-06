#include <onidev.h>

class Player:
    public od::AutoIndex<od::Object2d, Player, 10>
{
public:
    Player(float x, float y):
        od::AutoIndex<od::Object2d, Player, 10>(x, y) {}
    
    od::Shape * collisionMask(int p) const override {
        static od::Rect rect(0, 0, 16, 16);
        return &rect;
    }
    
    void step(float dt) override;
    void draw() const override {
        glColor3f(1, 1, 1); 
        od::drawRectangle(pos.x, pos.y, pos.x+16, pos.y+16);
    }
};

void Player::step(float dt)
{
    if(od::keyCheck(od::vk_left)) pos.x -= dt;
    if(od::keyCheck(od::vk_right)) pos.x += dt;
    if(od::keyCheck(od::vk_up)) pos.y -= dt;
    if(od::keyCheck(od::vk_down)) pos.y += dt;
}

class GroundItem:
    public od::AutoIndex<od::Object2d, GroundItem, 5>
{
public:
    GroundItem(float x, float y): od::AutoIndex<od::Object2d, GroundItem, 5>(x, y) {}
    void draw() const override {
        glColor3f(0.4, 0.4, 0.4); 
        od::drawRectangle(pos.x, pos.y, pos.x+16, pos.y+16);
    }
};

class SkyItem:
    public od::AutoIndex<od::Object2d, SkyItem, 15>
{
public:
    SkyItem(float x, float y): od::AutoIndex<od::Object2d, SkyItem, 15>(x, y) {}
    void draw() const override {
        glColor3f(0, 1, 1); 
        od::drawRectangle(pos.x, pos.y, pos.x+16, pos.y+16);
    }
};

int main()
{
    using namespace od;
    Window win(640, 480, "Global Object Depth Example");
    win.setSynchronization(true);
    
    InstanceContext<Object2d> ic;
    od::instanceCreate<Player>(64, 64);
    
    for(int i=0; i<win.width (); i+=32)
    for(int j=0; j<win.height(); j+=32)
        od::instanceCreate<GroundItem>(i, j);
    
    for(int i=16; i<win.width (); i+=64)
    for(int j=16; j<win.height(); j+=64)
        od::instanceCreate<SkyItem>(i, j);
    
    float depth_min = 10;
    float depth_max = 10;
    
    framerateSetDelta(60);
    while(win.open())
    {
        win.ioHandle();
        
        if(keyPressed(vk_escape))
            break;
        
        drawClear();
        win.updateView();
        
        float dt = deltaTime();
        ic.update(dt);
        
        depth_min -= 0.05f * dt;
        depth_max += 0.05f * dt;
        
        ic.renderLayers(depth_min, depth_max);
        
        framerateUpdate();
        win.screenRefresh();
    }
    
    return 0;
}
