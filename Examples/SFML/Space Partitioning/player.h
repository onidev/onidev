#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <onidev/game/index.h>
#include "object.h"

class Player:
    public od::AutoIndex<sfObject, Player>
{
public:
    Player(float x, float y);
    od::Shape * collisionMask(int p) const override;
    void step(float dt) override;
    void draw(sf::RenderTarget& render_target) const override;
};

#endif // PLAYER_H_INCLUDED
