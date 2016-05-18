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

/*=============================================================================
	object2d.inc
=============================================================================*/

#include <onidev/game/object2d.h>
#include <onidev/collision/tilemap.h>

namespace od
{

/*-----------------------------------------------------------------------------
	Constructor & destructor.
-----------------------------------------------------------------------------*/

template<class T>
GenericObject2d<T>::GenericObject2d(float x, float y):
    _context(&InstanceContext<T>::getGlobal()),
    pos(x, y)
{
}

template<class T>
GenericObject2d<T>::~GenericObject2d()
{
}

/*-----------------------------------------------------------------------------
	Virtual methods.
-----------------------------------------------------------------------------*/

template<class T>
const od::Shape * GenericObject2d<T>::collisionMask(int p) const
{
    return nullptr;
}

/*-----------------------------------------------------------------------------
	GenericObject2d misc methods.
-----------------------------------------------------------------------------*/

template<class T>
void GenericObject2d<T>::instanceDestroy()
{
    this->eventDestroyed();
    
    if(this->_context)
        this->_context->remove(static_cast<T*>(this));
}

template<class T>
void GenericObject2d<T>::instanceKill()
{
    if(this->_context)
        this->_context->remove(static_cast<T*>(this));
}

/*-----------------------------------------------------------------------------
	GenericObject2d collision detection methods.
-----------------------------------------------------------------------------*/

template<class T>
bool GenericObject2d<T>::placeMeeting(float x, float y, GenericObject2d* other, int other_mask_index)
{
    const od::Shape * mask = collisionMask();
    if(mask)
    {
        const od::Shape * mask_other = other->collisionMask(other_mask_index);
        if(mask_other)
        {
            return mask_other->collide(*mask, x - other->pos.x, y - other->pos.y);
        }
    }
    return false;
}

template<class T>
GenericObject2d<T>* GenericObject2d<T>::instancePosition(float x, float y, int object, int other_mask_index) const
{
    if(auto * sp = this->_context->getSpacePartitioning(object))
    {
        return sp->instanceCollision(x, y);
    }
    
    const auto& ins = this->context().instances(object);
    for(auto& ii: ins)
    {
        if(ii)
        {
            const od::Shape * shape = ii->collisionMask(other_mask_index);
            if(shape)
            {
                if(shape->intersect(x - ii->pos.x, y - ii->pos.y))
                    return ii.get();
            }
        }
        
    }
    
    return nullptr;
}

template<class T>
GenericObject2d<T>* GenericObject2d<T>::instancePlace(float x, float y, int object, int otherMaskIndex) const // ne peut pas retourner un std::shared_ptr<GenericObject2d> -->
{
    if(auto * sp = this->_context->getSpacePartitioning(object))
    {
        return sp->instanceCollision(static_cast<const T*>(this), x, y); // --> car sp ne contiens que des pointeurs nus
    }
    
    const od::Shape * mask = collisionMask();
    if(!mask)
    {
        return nullptr;
    }
    
    const auto& ins = this->context().instances(object);
    for(auto& ii: ins)
    {
        if(ii)
        {
            const od::Shape * shape = ii->collisionMask(otherMaskIndex);
            if(shape)
            {
                if(shape->collide(*mask, x - ii->pos.x, y - ii->pos.y))
                    return ii.get();
            }
        }
    }
    return nullptr;
}

template<class T>
bool GenericObject2d<T>::placeMeeting(float x, float y, int object, int otherMaskIndex) const
{
    return instancePlace(x, y, object, otherMaskIndex) != nullptr;
}

template<class T>
bool GenericObject2d<T>::tileFree(float x, float y) const
{
    for(const auto& i: this->_context->room().layers())
    {
        if(i->collision(collisionMask(), x, y))
            return false;
    }
    return true;
}

/*-----------------------------------------------------------------------------
	GenericObject2d bounding box methods.
-----------------------------------------------------------------------------*/

template<class T>
od::Rect GenericObject2d<T>::boundingBox() const
{
    const od::Shape * mask = collisionMask();
    if(mask)
    {
        od::Rect box = mask->boundingBox();
        box.x1 += pos.x;
        box.y1 += pos.y;
        box.x2 += pos.x;
        box.y2 += pos.y;
        return box;
    }
    
    return od::Rect();
}

template<class T>
od::Rect GenericObject2d<T>::boundingBox(const od::Shape * mask) const
{
    if(mask)
    {
        od::Rect box = mask->boundingBox();
        box.x1 += pos.x;
        box.y1 += pos.y;
        box.x2 += pos.x;
        box.y2 += pos.y;
        return box;
    }
    return od::Rect();
}

template<class T>
od::Rect GenericObject2d<T>::boundingBoxRelative() const
{
    const od::Shape * mask = collisionMask();
    if(mask)
    {
        return mask->boundingBox();
    }
    return od::Rect();
}

/*-----------------------------------------------------------------------------
	Template methods.
-----------------------------------------------------------------------------*/

template<class T>
template<class Object>
GenericObject2d<T>* GenericObject2d<T>::instancePosition(float x, float y, int otherMaskIndex) const {
    return instancePosition(x, y, Object::ObjectIndex, otherMaskIndex);
}

template<class T>
template<class Object>
GenericObject2d<T>* GenericObject2d<T>::instancePlace(float x, float y, int otherMaskIndex) const {
    return instancePlace(x, y, Object::ObjectIndex, otherMaskIndex);
}

template<class T>
template<class ... Args>
bool GenericObject2d<T>::placeMeeting(float x, float y, int otherMaskIndex) const {
    for(size_t obj : {Args::ObjectIndex...})
    {
        if(placeMeeting(x, y, obj, otherMaskIndex))
            return true;
    }
    return false;
}


/*-----------------------------------------------------------------------------
	Misc methods.
-----------------------------------------------------------------------------*/

template<class T>
std::shared_ptr<T> GenericObject2d<T>::shared() const
{
    if(_context)
    {
        for(auto& ins: _context->instances(objectIndex()))
        {
            if(ins.get() == this)
            {
                return ins;
            }
        }
    }
    return nullptr;
}

template<class T>
template<class Object>
std::shared_ptr<Object> GenericObject2d<T>::shared() const
{
    if(_context)
    {
        for(auto& ins: _context->instances(objectIndex()))
        {
            if(ins.get() == this)
            {
                return std::static_pointer_cast<Object>(ins);
            }
        }
    }
    return nullptr;
}

template<class T>
const InstanceContext<T> & GenericObject2d<T>::context() const
{
    return *_context;
}

template<class T>
InstanceContext<T> & GenericObject2d<T>::context()
{
    return *_context;
}


}
