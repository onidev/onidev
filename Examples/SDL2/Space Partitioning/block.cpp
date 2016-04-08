#include "block.h"
#include <GL/gl.h>

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
    glBegin(GL_QUADS);
        glVertex2f(pos.x, pos.y);
        glVertex2f(pos.x + 16, pos.y);
        glVertex2f(pos.x + 16, pos.y + 16);
        glVertex2f(pos.x, pos.y + 16);
    glEnd();
}
