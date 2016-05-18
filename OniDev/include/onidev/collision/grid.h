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

#ifndef ONIDEV_COLLISION_GRID_H_INCLUDED
#define ONIDEV_COLLISION_GRID_H_INCLUDED

#include <memory>
#include <onidev/collision/rect.h>
#include <onidev/collision/partitioning.h>

namespace od
{

template<class T>
class GenericCollisionGrid2d:
    public SpacePartitioning<T>
{
    std::vector< std::unique_ptr< std::vector<T*> > > _grid;
    size_t _wid;
    size_t _hei;
    size_t _cell_wid;
    size_t _cell_hei;
    size_t _count;
    
public:
    enum Mode
    {
        Interior,
        Intersection,
        OutlineIntersection
    };
    
    GenericCollisionGrid2d(size_t cell_wid, size_t cell_hei, size_t cols, size_t rows);
    
    // SpacePartitioning overrides
    void purge();
    void clear();
    
    void insert(T * entity) override;
    bool remove(T * entity) override;
    
    T* instanceCollision(float x, float y) const override;
    T* instanceCollision(const T * entity, float dx, float dy) const override;
    std::vector<T*> instancesCollision(const T * entity, float x, float y) override;
    
    bool collision(const T* entity, float dx, float dy) override;
    std::vector<T*> retrieve(float x1, float y1, float x2, float y2, int mode) const override;
    
    size_t count() const override;
    
    // CollisionGrid getters
    size_t width() const;
    size_t height() const;
    size_t cellWidth() const;
    size_t cellHeight() const;
    
};

#include "grid.inc"

}

#endif
