#include "player.h"
#include "block.h"
#include <SDL2/SDL.h>
#include <GL/gl.h>

Player::Player(float x, float y):
    od::AutoIndex<od::Object2d, Player>(x, y)
{
}

od::Shape * Player::collisionMask(int p) const {
    static od::Rect rect(0, 0, 16, 16);
    return &rect;
}

void Player::step(float dt)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_LEFT] && !placeMeeting<Block>(pos.x-dt, pos.y)) pos.x -= dt;
    if(state[SDL_SCANCODE_RIGHT]&& !placeMeeting<Block>(pos.x+dt, pos.y)) pos.x += dt;
    if(state[SDL_SCANCODE_UP]   && !placeMeeting<Block>(pos.x, pos.y-dt)) pos.y -= dt;
    if(state[SDL_SCANCODE_DOWN] && !placeMeeting<Block>(pos.x, pos.y+dt)) pos.y += dt;
}

void Player::draw() const {
    glColor3f(0, 0.5, 0);
    glBegin(GL_QUADS);
        glVertex2f(pos.x, pos.y);
        glVertex2f(pos.x + 16, pos.y);
        glVertex2f(pos.x + 16, pos.y + 16);
        glVertex2f(pos.x, pos.y + 16);
    glEnd();
}
