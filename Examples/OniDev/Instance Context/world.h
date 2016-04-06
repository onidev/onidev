#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include <onidev.h>

class World:
    public od::TileMap
{
    int wid, hei;
    std::vector<bool> tiles;
    
public:
    World(int wid, int hei, float px, float py);
    
    bool tileSolid(int x, int y) const override;
    od::Shape * tileCollisionMask(int x, int y) const override;
    
    int width() const override;
    int height() const override;
    int tileWidth() const override;
    int tileHeight() const override;
    
//    float xoffset() const override { return 8.f; }
//    float yoffset() const override { return 10.f; }
    
    void set(int x, int y, bool val);
    
    void draw() const;
};

#endif // WORLD_H_INCLUDED
