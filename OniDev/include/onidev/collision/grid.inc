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
	grid.inc: Grid Space Partitioning for the game engine.
=============================================================================*/

/*-----------------------------------------------------------------------------
	Constructor.
-----------------------------------------------------------------------------*/

template<class T>
GenericCollisionGrid2d<T>::GenericCollisionGrid2d(size_t cell_wid, size_t cell_hei, size_t cols, size_t rows):
    _grid(cols * rows),
    _wid(cols),
    _hei(rows),
    _cell_wid(cell_wid),
    _cell_hei(cell_hei),
    _count(0)
{
}

/*-----------------------------------------------------------------------------
	SpacePartitioning overrides.
-----------------------------------------------------------------------------*/

template<class T>
void GenericCollisionGrid2d<T>::insert(T * entity)
{
    od::Rect box = entity->boundingBox();
    int x1 = box.x1 / _cell_wid;
    int y1 = box.y1 / _cell_hei;
    int x2 = box.x2 / _cell_wid;
    int y2 = box.y2 / _cell_hei;
    
    if(x1 < 0) x1 = 0;
    if(y1 < 0) y1 = 0;
    if(x2 > _wid-1) x2 = _wid-1;
    if(y2 > _hei-1) y2 = _hei-1;
    
    for(size_t i=x1; i<=x2; ++i)
    for(size_t j=y1; j<=y2; ++j)
    {
        auto & t = _grid[i + j*_wid];
        if(!t)
        {
            t.reset(new std::vector<T*>());
        }
        
        t->push_back(entity);
        //printf("insert(%d %d)\n", i, j);
    }
    _count++;
}

template<class T>
bool GenericCollisionGrid2d<T>::remove(T * entity)
{
    od::Rect box = entity->boundingBox();
    int x1 = box.x1 / _cell_wid;
    int y1 = box.y1 / _cell_hei;
    int x2 = box.x2 / _cell_wid;
    int y2 = box.y2 / _cell_hei;
    
    if(x1 < 0) x1 = 0;
    if(y1 < 0) y1 = 0;
    if(x2 > _wid-1) x2 = _wid-1;
    if(y2 > _hei-1) y2 = _hei-1;
    
    bool find = false;
    for(size_t i=x1; i<=x2; ++i)
    for(size_t j=y1; j<=y2; ++j)
    {
        auto & t = _grid[i + j*_wid];
        if(t)
        {
            for(auto it=t->begin(); it!=t->end(); ++it)
            {
                if( *it == entity )
                {
                    t->erase(it);
                    find = true;
                    break;
                }
            }
        }
    }
    
    if(!find)
    {
        printf("GenericCollisionGrid2d<T>::remove - Bruteforce deep search\n");
        for(auto& t: _grid)
        {
            if(t)
            {
                for(auto it=t->begin(); it!=t->end(); ++it)
                {
                    if( *it == entity )
                    {
                        t->erase(it);
                        find = true;
                        break;
                    }
                }
            }
        }
    }
    
    if(find)
        _count--;
    
    return find;
}

template<class T>
void GenericCollisionGrid2d<T>::purge()
{
    _count = 0;
    _grid.clear();
    _grid.resize(_wid * _hei);
}

template<class T>
void GenericCollisionGrid2d<T>::clear() // ??
{
    purge();
}

template<class T>
T* GenericCollisionGrid2d<T>::instanceCollision(float x, float y) const
{
    int i = x / _cell_wid;
    int j = y / _cell_wid;
    
    if(i < 0) i = 0;
    if(j < 0) j = 0;
    if(i > _wid-1) i = _wid-1;
    if(j > _hei-1) j = _hei-1;
    
    auto & t = _grid[i + j*_wid];
    if(t && !t->empty())
    {
        for(T * e: *t)
        {
            if(e->collisionMask()->intersect(x - e->pos.x, y - e->pos.y))
                return e;
        }
    }
    
    return nullptr;
}

template<class T>
T* GenericCollisionGrid2d<T>::instanceCollision(const T * entity, float x, float y) const
{
    const od::Shape * shape = entity->collisionMask(0);
    
    if(!shape)
        return nullptr;
    
    od::Rect box = entity->boundingBox(shape);
    int x1 = box.x1 / _cell_wid;
    int y1 = box.y1 / _cell_hei;
    int x2 = box.x2 / _cell_wid;
    int y2 = box.y2 / _cell_hei;
    
    if(x1 < 0) x1 = 0;
    if(y1 < 0) y1 = 0;
    if(x2 > _wid-1) x2 = _wid-1;
    if(y2 > _hei-1) y2 = _hei-1;
    
    for(size_t i=x1; i<=x2; ++i)
    for(size_t j=y1; j<=y2; ++j)
    {
        auto & t = _grid[i + j*_wid];
        if(t && !t->empty())
        {
            for(T * e: *t)
            {
                if(e->collisionMask()->collide(*shape, x - e->pos.x, y - e->pos.y))
                    return e;
            }
        }
    }
    return nullptr;
}

template<class T>
std::vector<T*> GenericCollisionGrid2d<T>::instancesCollision(const T * entity, float x, float y)
{
    std::vector<T*> ret;
    const od::Shape * shape = entity->collisionMask(0);
    
    if(!shape)
        return ret;
    
    od::Rect box = entity->boundingBox(shape);
    int x1 = box.x1 / _cell_wid;
    int y1 = box.y1 / _cell_hei;
    int x2 = box.x2 / _cell_wid;
    int y2 = box.y2 / _cell_hei;
    
    if(x1 < 0) x1 = 0;
    if(y1 < 0) y1 = 0;
    if(x2 > _wid-1) x2 = _wid-1;
    if(y2 > _hei-1) y2 = _hei-1;
    
    for(size_t i=x1; i<=x2; ++i)
    for(size_t j=y1; j<=y2; ++j)
    {
        auto & t = _grid[i + j*_wid];
        if(t && !t->empty())
        {
            for(T * e: *t)
            {
                if(e->collisionMask()->collide(*shape, x - e->pos.x, y - e->pos.y))
                    ret.push_back(e);
            }
        }
    }
    return ret;
}

template<class T>
bool GenericCollisionGrid2d<T>::collision(const T * entity, float x, float y)
{
    return instanceCollision(entity, x, y) != nullptr;
}

template<class T>
std::vector<T*> GenericCollisionGrid2d<T>::retrieve(float x1_, float y1_, float x2_, float y2_, int mode) const
{
    std::vector<T*> ret;
    int x1 = x1_ / _cell_wid;
    int y1 = y1_ / _cell_hei;
    int x2 = x2_ / _cell_wid;
    int y2 = y2_ / _cell_hei;
    
    if(x1 < 0) x1 = 0;
    if(y1 < 0) y1 = 0;
    if(x2 > _wid-1) x2 = _wid-1;
    if(y2 > _hei-1) y2 = _hei-1;
    
    for(size_t i=x1; i<=x2; ++i)
    for(size_t j=y1; j<=y2; ++j)
    {
        auto & t = _grid[i + j*_wid];
        if(t && !t->empty())
        {
            for(T* e: *t)
            {
                od::Rect b = e->boundingBox();
                
                if(mode == Interior)
                {
                    if(b.x1 >= x1_ && b.x2 <= x2_ && b.y1 >= y1_ && b.y2 <= y2_)
                        ret.push_back(e);
                }
                else if(mode == Intersection)
                {
                    if(x2_ > b.x1 && x1_ < b.x2 && y2_ > b.y1 && y1_ < b.y2)
                        ret.push_back(e);
                }
                else
                {
                    if(x2_ > b.x1 && x1_ < b.x2 && y2_ > b.y1 && y1_ < b.y2 &&
                     !(b.x1 >= x1_ && b.x2 <= x2_ && b.y1 >= y1_ && b.y2 <= y2_))
                        ret.push_back(e);
                }
            }
        }
    }
    return ret;
}

template<class T>
size_t GenericCollisionGrid2d<T>::count() const {
    return _count;
}

/*-----------------------------------------------------------------------------
	GenericCollisionGrid2d getters.
-----------------------------------------------------------------------------*/

template<class T>
size_t GenericCollisionGrid2d<T>::width() const {
    return _wid;
}

template<class T>
size_t GenericCollisionGrid2d<T>::height() const {
    return _hei;
}

template<class T>
size_t GenericCollisionGrid2d<T>::cellWidth() const {
    return _cell_wid;
}

template<class T>
size_t GenericCollisionGrid2d<T>::cellHeight() const {
    return _cell_hei;
}

