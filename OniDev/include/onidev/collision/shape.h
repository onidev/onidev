#ifndef ONIDEV_COLLISION_SHAPE_H_INCLUDED
#define ONIDEV_COLLISION_SHAPE_H_INCLUDED

namespace od
{
class Line;
class Rect;
class Disk;
class Mask;
class ConvexPolygon;
class Polygon;

/// @brief Base class of all shapes like rectangle or disk
class Shape
{
public:
    virtual ~Shape() {}
    
    /**
     * @brief Collide with a point
     * @param x, y  The coordinates of the collision point
     */
    virtual bool intersect(float x, float y) const = 0;
    
    /**
     * @brief Collide with a line
     * @param line The line shape for collisions test
     * @param dx, dy  The difference vector between the two shapes
     */
    virtual bool intersect(const Line & line, float dx, float dy) const = 0;
    
    /**
     * @brief Collide with a Axis Aligned Bounding Box
     * @param rect The rectangle shape for collisions test
     * @param dx, dy  The difference vector between the two shapes
     */
    virtual bool intersect(const Rect & rect, float dx, float dy) const = 0;
    
    /**
     * @brief Collide with a disk
     * @param disk The disk shape for collisions test
     * @param dx, dy  The difference vector between the two shapes
     */
    virtual bool intersect(const Disk & disk, float dx, float dy) const = 0;
    
    /**
     * @brief Collide with a pixel perfect mask
     * @param mask The pixel perfect mask for collisions test
     * @param dx, dy  The difference vector between the two shapes
     */
    virtual bool intersect(const Mask & mask, float dx, float dy) const = 0;
    
    /**
     * @brief Collide with a convex polygon
     * @param polygon The convex polygon shape for collisions test
     * @param dx, dy  The difference vector between the two shapes
     */
    virtual bool intersect(const ConvexPolygon & polygon, float dx, float dy) const = 0;
    
    /**
     * @brief Collide with a polygon
     * @param polygon The polygon shape for collisions test
     * @param dx, dy  The difference vector between the two shapes
     */
    virtual bool intersect(const Polygon & polygon, float dx, float dy) const = 0;
    
    /**
     * @brief Collide with a other Shape
     *
     * These function use the pattern visitor for determine if there is a collision with a other shape
     * without having to worry about its nature.
     * @param shape  The mask with which the collision will be tested.
     * @param dx, dy  The difference vector between the two shapes
     * @return  Return true if there is collision between the two mask, false otherwise.
     */
    virtual bool collide(const Shape & shape, float dx, float dy) const = 0;

    /**
     * @brief Bounding Box
     *
     * Each Shape has a bounding box. This is used for two purposes: \n
     * -start with a collision test between two boxes allows to optimise the process. \n
     * -indicate what tiles that are probably in collision with the object.
     * @return Return the bounding box of the object.
     */
    virtual const Rect boundingBox() const = 0;
};
}

#endif


/**
 * @class od::Shape
 * @ingroup Collisions
 *
 * Collisions masks allow objects to interact with each other.
 * You can easily detect if there is an object on another object, whatever the type
 * of mask are used for the two objects.
 * Unfortunately, some interactions are not yet implemented: \n
 * -Mask VS Polygons \n
 * -Mask VS Disk \n
 * -Polygons VS Disk
 *
 * However, the most used are functional.
 */


