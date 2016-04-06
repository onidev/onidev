#include "block.h"
#include <onidev.h>

Block::Block(float x, float y):
    od::AutoIndex<od::Object2d, Block>(x, y)
{
}

od::Shape * Block::collisionMask(int p) const {
    static od::Rect rect(0, 0, 16, 16);
    return &rect;
}

void Block::draw() const {
    glColor3f(1, 1, 1);
    od::drawRectangle(pos.x, pos.y, pos.x+16, pos.y+16, false);
}
