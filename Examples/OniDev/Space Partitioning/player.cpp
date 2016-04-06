#include "player.h"
#include "block.h"
#include <onidev.h>

Player::Player(float x, float y):
    od::AutoIndex<od::Object2d, Player>(x, y)
{
    //printf("%u\n", objectIndex());
}

od::Shape * Player::collisionMask(int p) const {
    static od::Rect rect(0, 0, 16, 16);
    return &rect;
}

void Player::step(float dt)
{
    using namespace od;
    if(keyCheck(vk_right)&& !placeMeeting<Block>(pos.x+dt, pos.y)) pos.x += dt;
    if(keyCheck(vk_left) && !placeMeeting<Block>(pos.x-dt, pos.y)) pos.x -= dt;
    if(keyCheck(vk_down) && !placeMeeting<Block>(pos.x, pos.y+dt)) pos.y += dt;
    if(keyCheck(vk_up)   && !placeMeeting<Block>(pos.x, pos.y-dt)) pos.y -= dt;
}

void Player::draw() const {
    //printf("%f %f\n", pos.x, pos.y);
    glColor3f(0, 0.5, 0);
    od::drawRectangle(pos.x, pos.y, pos.x+16, pos.y+16, false);
}
