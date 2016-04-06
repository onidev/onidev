#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <onidev.h>

class Player:
    public od::AutoIndex<od::Object2d, Player>
{
    float vx, vy;
    
public:
    Player(float x, float y);
    
    od::Shape * collisionMask(int p) const override;
    bool placeFree(float x, float y) const;
    
    void step(float dt) override;
    void draw() const override;
};

#endif // PLAYER_H_INCLUDED
