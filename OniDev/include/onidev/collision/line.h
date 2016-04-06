#ifndef ONIDEV_COLLISION_LINE_H_INCLUDED
#define ONIDEV_COLLISION_LINE_H_INCLUDED

#include <onidev/collision/shape.h>

namespace od
{

/// @brief A line shape for manage collisions
class Line:
    public Shape
{
public:
    float x1;
    float y1;
    float x2;
    float y2;
    
    Line();
    Line(const Line & rect);
    Line(float x1, float y1, float x2, float y2);
    
    bool intersect(float x, float y) const;
    
    bool intersect(const Line & line, float dx, float dy) const;
    bool intersect(const Rect & rect, float dx, float dy) const;
    bool intersect(const Disk & disk, float dx, float dy) const;
    bool intersect(const Mask & mask, float dx, float dy) const;
    bool intersect(const ConvexPolygon & poly, float dx, float dy) const;
    bool intersect(const Polygon & polygon, float dx, float dy) const;
    
    bool collide(const Shape & shape, float dx, float dy) const;
    
    const Rect boundingBox() const;
    //float width() const;
    //float height() const;
    
    //const Rect translate(float x, float y) const;
    //void translate(float x, float y);
    //void scale(float x, float y);
    
    Line & operator=(const Line & rect);
};

}

/**
 * @class od::Line
 * @ingroup Collisions
 */

#endif // ONIDEV_COLLISION_LINE_H_INCLUDED
