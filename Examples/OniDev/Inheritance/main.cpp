#include <onidev.h>

class PlayerBase:
    public od::Object2d
{
public:
    PlayerBase(float x, float y):
        od::Object2d(x, y) {}
    
    void draw() const override {
        od::drawRectangle(pos.x, pos.y, pos.x+16, pos.y+16);
    }
};

class Player:
    public od::AutoIndex<PlayerBase, Player>
{
public:
    Player(float x, float y):
        od::AutoIndex<PlayerBase, Player>(x, y) {}
    
    od::Shape * collisionMask(int p) const override {
        static od::Rect rect(0, 0, 16, 16);
        return &rect;
    }
    
    void step(float dt) override;
};

void Player::step(float dt)
{
    if(od::keyCheck(od::vk_left)) pos.x -= dt;
    if(od::keyCheck(od::vk_right)) pos.x += dt;
    if(od::keyCheck(od::vk_up)) pos.y -= dt;
    if(od::keyCheck(od::vk_down)) pos.y += dt;
}

int main()
{
    using namespace od;
    Window win(640, 480, "Instance Context Example");
    win.setSynchronization(true);
    
    InstanceContext<Object2d> ic;
    od::instanceCreate<Player>(64, 64);
    
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
