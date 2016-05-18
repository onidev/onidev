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

#ifndef ONIDEV_COLLISION_POLYGON_H_INCLUDED
#define ONIDEV_COLLISION_POLYGON_H_INCLUDED

#include <vector>
#include <onidev/collision/convex_polygon.h>
#include <onidev/collision/rect.h>

namespace od
{

/// @brief A polygon shape for manage collisions
class Polygon:
    public Shape
{
    friend class Collision;
    
    std::vector<ConvexPolygon> poly;
    Rect box;
    void updateBoundingBox();
    
public:
    Polygon(const Polygon & p);
    //Polygon(const ConvexPolygon & p);
    Polygon(const float pts[], const int n[], int count, bool centerShape=false);
    Polygon & operator=(const Polygon & d);
    
    bool intersect(float x, float y) const;
    
    bool intersect(const Line & line, float dx, float dy) const {return false;}
    bool intersect(const Rect & rect, float dx, float dy) const;
    bool intersect(const Disk & disk, float dx, float dy) const;
    bool intersect(const Mask & mask, float dx, float dy) const;
    bool intersect(const ConvexPolygon & poly, float dx, float dy) const;
    bool intersect(const Polygon & poly, float dx, float dy) const;
    
    bool collide(const Shape & s, float dx, float dy) const;
    
    const Rect boundingBox() const;
    
    void draw(float x, float y) const;
    
    void translate(float x, float y);
    void rotate(const Polygon & model, float angle);
    void center();
};
}

/**
 * @class od::Polygon
 * @ingroup Collisions
 */

#endif // POLYGON_H_INCLUDED
