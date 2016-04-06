#ifndef ONIDEV_COLLISION_RECTANGLE_H_INCLUDED
#define ONIDEV_COLLISION_RECTANGLE_H_INCLUDED

#include <onidev/core/primitives.h>
#include <onidev/collision/shape.h>

namespace od
{

/// @brief An axis aligned bounding box shape for manage collisions
class Rect:
    public Shape
{
public:
    float x1, y1, x2, y2;
    
    Rect();
    Rect(const Rect & rect);
    Rect(float x1, float y1, float x2, float y2);
    
    bool intersect(float x, float y) const;
    
    bool intersect(const Line & line, float dx, float dy) const;
    bool intersect(const Rect & rect, float dx, float dy) const;
    bool intersect(const Disk & disk, float dx, float dy) const;
    bool intersect(const Mask & mask, float dx, float dy) const;
    bool intersect(const ConvexPolygon & poly, float dx, float dy) const;
    bool intersect(const Polygon & poly, float dx, float dy) const;
    
    bool collide(const Shape & shape, float dx, float dy) const;
    
    const Rect boundingBox() const;
    float width() const;
    float height() const;
    
    Pos center() const;
    
    const Rect translate(float x, float y) const;
    void translate(float x, float y);
    void scale(float x, float y);
    
    Rect & operator=(const Rect & rect);
};

}

#endif // ONIDEV_COLLISION_RECTANGLE_H_INCLUDED


/**
 * @class od::Rect
 * @ingroup Collisions
 *
 * Rect provide a axis aligned bounding box mask (no rotations possibles).
 * Rect collisions are very fast and usefull. They are used in some games like fight games,
 * RPG, platformers, shooters, etc...
 */
 
 
