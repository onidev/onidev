#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include <onidev/game/index.h>
#include "object.h"

class Block:
    public od::AutoIndex<sfObject, Block>
{
public:
    Block(float x, float y);
    od::Shape * collisionMask(int p) const override;
    void draw(sf::RenderTarget& render_target) const override;
};

#endif // BLOCK_H_INCLUDED
