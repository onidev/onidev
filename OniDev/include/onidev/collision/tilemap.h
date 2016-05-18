// OniDev - Copyright (c) 2013-2016 Boris Marmontel
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.

#ifndef ONIDEV_COLLISION_TILEMAP_H_INCLUDED
#define ONIDEV_COLLISION_TILEMAP_H_INCLUDED

#include <onidev/collision/shape.h>

namespace od
{

class TileMap
{
public:
    TileMap();
    virtual ~TileMap();
    
    virtual bool tileSolid(int x, int y) const = 0;
    virtual od::Shape * tileCollisionMask(int x, int y) const = 0;
    
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual int tileWidth() const = 0;
    virtual int tileHeight() const = 0;
    
    virtual float xoffset() const;
    virtual float yoffset() const;
    
    bool collision(const od::Shape * shape, float dx, float dy) const;
};

}

#endif
