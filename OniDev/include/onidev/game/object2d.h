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

#ifndef ONIDEV_GAME_OBJECT2D_H_INCLUDED
#define ONIDEV_GAME_OBJECT2D_H_INCLUDED

#include <onidev/game/create.h>
#include <onidev/collision/rect.h>
#include <onidev/collision/grid.h>
#include <onidev/math/vec2.h>

namespace od
{

template<class T>
class GenericObject2d
{
    friend class InstanceContainer<T>;
    friend class InstanceContext<T>;
    
protected:
    InstanceContext<T> * _context;
    
public:
    od::Vec2 pos;
    
    GenericObject2d(float x, float y);
    virtual ~GenericObject2d();
    
    virtual size_t objectIndex() const = 0;
    virtual size_t objectPriority() const { return 0; }
    
    virtual bool generateUid() const { return false; }
    virtual void setUid(unsigned uid) {}
    virtual unsigned uid() const { return 0; }
    
    virtual void instanceDestroy();
    virtual void instanceKill();
    
    virtual void eventDestroyed() {}
    virtual void eventCreated() {}
    virtual void step(float dt) {}
    virtual void draw() const {}
    virtual void predraw() const {}
    virtual void postdraw() const {}
    
    std::shared_ptr<T> shared() const;
    
    template<class Object>
    std::shared_ptr<Object> shared() const;
    
    const InstanceContext<T> & context() const;
    InstanceContext<T> & context();
    
    
    // virtual methods
    virtual const od::Shape * collisionMask(int p = 0) const;
    
    // collision detection methods
    bool placeMeeting(float x, float y, GenericObject2d* other, int other_mask_index = 0);
    
    GenericObject2d* instancePosition(float x, float y, int object, int other_mask_index) const;
    GenericObject2d* instancePlace(float x, float y, int object, int other_mask_index) const;
    bool placeMeeting(float x, float y, int object, int other_mask_index) const;
    
    template<class Object>
    GenericObject2d* instancePosition(float x, float y, int other_mask_index = 0) const;
    
    template<class Object>
    GenericObject2d* instancePlace(float x, float y, int other_mask_index = 0) const;
    
    template<class ... Args>
    bool placeMeeting(float x, float y, int other_mask_index = 0) const;
    
    bool tileFree(float x, float y) const;
    
    // bounding box methods
    od::Rect boundingBox() const;
    od::Rect boundingBox(const od::Shape * collision_mask) const;
    od::Rect boundingBoxRelative() const;
};

}

#include "object2d.inc"

namespace od
{

class StreamData;

class Object2d:
    public GenericObject2d<Object2d>
{
    template<class U, class V, int P>
    friend class AutoIndex;
    
    static size_t GlobalCounter; // AutoIndex need to acces to this static member
    
public:
    Object2d(float x, float y):
        GenericObject2d<Object2d>(x, y) {}
    
    virtual void evSend(StreamData& stream) {}
    virtual void evReceive(StreamData& stream) {}
};

class CollisionGrid:
    public GenericCollisionGrid2d<Object2d>
{
public:
    CollisionGrid(size_t cell_wid, size_t cell_hei, size_t cols, size_t rows):
        GenericCollisionGrid2d<Object2d>(cell_wid, cell_hei, cols, rows) {}
};

}

#endif
