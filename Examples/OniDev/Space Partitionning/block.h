#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include <onidev/game/index.h>
#include <onidev/game/object2d.h>

class Block : public od::AutoIndex<od::Object2d, Block>
{
public:
    Block(float x, float y);
    od::Shape * collisionMask(int p) const override;
    void draw() const override;
};

#endif // BLOCK_H_INCLUDED
