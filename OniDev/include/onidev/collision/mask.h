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

#ifndef ONIDEV_COLLISION_MASK_H_INCLUDED
#define ONIDEV_COLLISION_MASK_H_INCLUDED

#include <onidev/collision/shape.h>
#include <cstdio>

namespace od
{

/// @brief A pixel perfect mask shape for manage collisions
class Mask:
    public Shape
{
    friend class Collision;
    
    int wx; // offset
    int wy;
    
    int x1;
    int y1;
    int x2;
    int y2;
    
    int wid;
    int hei;
    char * data; // plutot utiliser std::vector<bool> ?

    void calculateBoundingBox();
    
public:
    Mask(const Mask & shape);
    Mask(const char * mask, int wid, int hei, int wx=0, int wy=0);
    Mask(const void * data, size_t image_width, size_t image_height, int wx=0, int wy=0);
    Mask(const void * image_data, size_t image_width, size_t image_height, int x, int y, int width, int height, int wx, int wy);
    //Mask(const class Image & image, int wx=0, int wy=0);
    //Mask(const class Image & image, int x, int y, int width, int height, int wx=0, int wy=0);
    ~Mask();
    
    bool intersect(float x, float y) const;
    
    bool intersect(const Line & line, float dx, float dy) const {return false;}
    bool intersect(const Rect & rect, float dx, float dy) const;
    bool intersect(const Disk & disk, float dx, float dy) const;
    bool intersect(const Mask & mask, float dx, float dy) const;
    bool intersect(const ConvexPolygon & poly, float dx, float dy) const { return false; }
    bool intersect(const Polygon & poly, float dx, float dy) const { return false; }
    
    bool collide(const Shape & s, float dx, float dy) const;
    
    const Rect boundingBox() const;
    
    void draw(float x, float y) const;
    
    Mask & operator = (const Mask & m);
};
}

/**
 * @class od::Mask
 * @ingroup Collisions
 *
 * Mask provide a pixel perfect mask.
 * It can be used for very precise collisions, against a loss of fastness
 * and memory usage (it depends on the size of the masks).
 */

#endif

