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
