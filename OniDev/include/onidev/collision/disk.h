#ifndef ONIDEV_COLLISION_DISK_H_INCLUDED
#define ONIDEV_COLLISION_DISK_H_INCLUDED

namespace od
{

/// @brief A disk shape for manage collisions
class Disk:
    public Shape
{
public:
    float x; ///< Horizontal coordinate of the disk
    float y; ///< Vertical coordinate of the disk
    float r; ///< Radius of the disk
    
    /**
     * @brief Create a disk with nul radius
     */
    Disk();
    /**
     * @brief Create a disk by copy
     */
    Disk(const Disk & d);
    /**
     * @brief Create a disk
     * @param x, y  Coordinates of the disk
     * @param r  Radius of the disk
     */
    Disk(float x, float y, float r);
    
    bool intersect(float x, float y) const;
    bool intersect(const Line & line, float dx, float dy) const;
    bool intersect(const Rect & rect, float dx, float dy) const;
    bool intersect(const Disk & disk, float dx, float dy) const;
    bool intersect(const Mask & mask, float dx, float dy) const;
    bool intersect(const ConvexPolygon & poly, float dx, float dy) const { return false; }
    bool intersect(const Polygon & poly, float dx, float dy) const { return false; }
    
    bool collide(const Shape & s, float dx, float dy) const;
    
    const Rect boundingBox() const;
    
    void translate(float x, float y);
    
    Disk & operator=(const Disk & d);
};
}

/**
 * @class od::Disk
 * @ingroup Collisions
 */

#endif // ONIDEV_COLLISION_DISK_H_INCLUDED
