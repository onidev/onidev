#ifndef ONIDEV_COLLISION_PARTITIONNING_H_INCLUDED
#define ONIDEV_COLLISION_PARTITIONNING_H_INCLUDED

#include <vector>

namespace od
{

template<class T>
class SpacePartitioning 
{
public:
    virtual ~SpacePartitioning() {}
    
    // Add entity to the partitioning engine
    virtual void insert(T * entity) = 0;
    // Remove it
    virtual bool remove(T * entity) = 0;
    
    virtual T* instanceCollision(float x, float y) const = 0;
    virtual T* instanceCollision(const T * entity, float dx, float dy) const = 0;
    virtual std::vector<T*> instancesCollision(const T * entity, float x, float y) = 0;
    
    virtual bool collision(const T * entity, float dx, float dy) = 0;
    
    virtual std::vector<T*> retrieve(float x1, float y1, float x2, float y2, int param) const = 0;
    
    virtual size_t count() const = 0;
};

}

#endif
