#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include <onidev.h>

class World:
    public od::TileMap
{
    int wid, hei;
    std::vector<bool> tiles;
    
public:
    World(int wid, int hei);
    World(const od::Image& im);
    
    bool tileSolid(int x, int y) const;
    od::Shape * tileCollisionMask(int x, int y) const;
    
    int width() const;
    int height() const;
    int tileWidth() const;
    int tileHeight() const;
    
//    float xoffset() const { return 8.f; }
//    float yoffset() const { return 10.f; }
    
    void set(int x, int y, bool val);
    
    void draw() const;
};

#endif // WORLD_H_INCLUDED
