#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <onidev/game/index.h>
#include <onidev/game/object2d.h>

class Player : public od::AutoIndex<od::Object2d, Player>
{
public:
    Player(float x, float y);
    od::Shape * collisionMask(int p) const override;
    void step(float dt) override;
    void draw() const override;
};

#endif // PLAYER_H_INCLUDED
