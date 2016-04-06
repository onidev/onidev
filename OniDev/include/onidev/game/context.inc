// Copyright 2013-2015 Boris Marmontel, All Rights Reserved.

/*=============================================================================
	context.inc: Instance context for the game engine.
=============================================================================*/

/*-----------------------------------------------------------------------------
	Static members.
-----------------------------------------------------------------------------*/

template<class T>
InstanceContext<T> * InstanceContext<T>::_def = new InstanceContext<T>;
template<class T>
InstanceContext<T> * InstanceContext<T>::_global = _def;
template<class T>
const std::vector<std::shared_ptr<T>> InstanceContext<T>::_empty_array;

/*-----------------------------------------------------------------------------
	Constructor & destructor.
-----------------------------------------------------------------------------*/

template<class T>
InstanceContext<T>::InstanceContext()
{
    _global = this;
}
template<class T>
InstanceContext<T>::~InstanceContext()
{
    _instances.purge();
    
    if(_global == this)
        _global = _def;
}

/*-----------------------------------------------------------------------------
	Private methods.
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	Public methods.
-----------------------------------------------------------------------------*/
/**
template<class T>
void InstanceContext<T>::addObjectIndex(size_t object, int priority)
{
    /// @todo resize _instances.instances
    
    if(_partitioning.size() != _instances.instances.size())
    {
        _partitioning.resize(_instances.instances.size());
    }
    updateObjectPriority(object, priority);
}
*/
template<class T>
void InstanceContext<T>::partitioningInsertInstance(size_t object, T* ins)
{
    // doublon
    if(_partitioning.size() != _instances.instances.size())
    {
        _partitioning.resize(_instances.instances.size());
    }
    // ------
    if(_partitioning[object])
    {
        _partitioning[object]->insert( ins );
    }
}

template<class T>
void InstanceContext<T>::updateObjectPriority(size_t object, int priority)
{
    bool update = false;
    if(_priority.size() != _instances.instances.size())
    {
        _priority.resize(_instances.instances.size());
        _object_priority.resize(_priority.size());
        update = true;
    }
    
    if(_object_priority[object] != priority || update)
    {
        _object_priority[object] = priority;
        
        for(int i=0; i<_priority.size(); ++i)
        {
            _priority[i] = i;
        }
        std::sort(_priority.begin(), _priority.end(), [this](int a, int b)
          {
              return _object_priority[a]<_object_priority[b];
          });
    }
}

template<class T>
void InstanceContext<T>::add(size_t object, std::shared_ptr<T> ins, int priority)
{
    _instances.add(object, ins);
    updateObjectPriority(object, priority);
    partitioningInsertInstance(object, ins.get());
}

template<class T>
void InstanceContext<T>::add(size_t object, std::shared_ptr<T> ins, int priority, unsigned uid)
{
    _instances.add(object, ins, uid);
    updateObjectPriority(object, priority);
    partitioningInsertInstance(object, ins.get());
}

template<class T>
std::shared_ptr<T> InstanceContext<T>::getSharedPtr(const T* ins) const
{
    int object = ins->objectIndex();
    
    if(object >= _instances.instances.size())
        return std::shared_ptr<T>();
    
    auto& t = _instances.instances[object];
    for(auto& ii: t)
    {
        if(ins == ii.get())
            return ii;
    }
    return std::shared_ptr<T>();
}

template<class T>
void InstanceContext<T>::remove(T * ins)
{
    if(_partitioning[ins->objectIndex()])
    {
        _partitioning[ins->objectIndex()]->remove( ins );
    }
    
    _instances.remove(ins->objectIndex(), ins, _inside_loop_update, _loop_object, _loop_cursor);
}

template<class T>
void InstanceContext<T>::move(std::shared_ptr<T> ins)
{
    // 1) Remove instance from its context
    InstanceContext<T>* prev = ins->_context;
    prev->remove(ins.get());
    
    // 2) Assign new context
    add(ins->objectIndex(), ins, ins->objectPriority());
    ins->_context = this;
}

template<class T>
void InstanceContext<T>::purge()
{
    _instances.purge();
    ///_partitioning.purge();
    //_temp.clear();
}

template<class T>
template<class Object>
void InstanceContext<T>::clear()
{
    auto & t = _instances.instances[Object::ObjectIndex];
    while(!t.empty())
    {
        t.back()->instanceDestroy();
    }
}

template<class T>
T* InstanceContext<T>::find(size_t object, unsigned uid)
{
    return _instances.find(object, uid);
}

template<class T>
template<class Object>
T* InstanceContext<T>::find(unsigned uid)
{
    return find(Object::ObjectIndex, uid);
}

template<class T>
template<class Object>
size_t InstanceContext<T>::instanceNumber() const
{
    if(Object::ObjectIndex >= _instances.instances.size())
        return 0;
    return _instances.number(Object::ObjectIndex);
}

template<class T>
const std::vector<std::shared_ptr<T>> & InstanceContext<T>::instances(size_t object) const
{
    if(object >= _instances.instances.size())
        return _empty_array;
    return _instances.instances[object];
}

template<class T>
template<class Object>
const std::vector<std::shared_ptr<T>> & InstanceContext<T>::instances() const
{
    if(Object::ObjectIndex >= _instances.instances.size())
        return _empty_array;
    return _instances.instances[Object::ObjectIndex];
}
template<class T>
size_t InstanceContext<T>::instanceNumber(size_t object) const
{
    if(object >= _instances.instances.size())
        return 0;
    return _instances.number(object);
}
template<class T>
size_t InstanceContext<T>::instanceNumber() const
{
    return _instances.number();
}
template<class T>
void InstanceContext<T>::update(float delta_time)
{
    _inside_loop_update = true;
    
    for(_loop_object=0; _loop_object<_instances.instances.size(); ++_loop_object)
    {
        auto& t = _instances.instances[_loop_object];
        for(_loop_cursor=0; _loop_cursor<t.size(); ++_loop_cursor)
        {
            t[_loop_cursor]->step(delta_time);
        }
    }
    
    _inside_loop_update = false;
    _loop_object = -1;
    _loop_cursor = -1;
}

template<class T>
void InstanceContext<T>::render(bool call_predraw) const
{
    for(size_t n=0; n<_instances.instances.size(); ++n)
    {
        auto& t = _instances.instances[ _priority[n] ];
        if(!t.empty())
        {
            if(call_predraw)
            {
                t.front()->predraw();
            }
            
            for(const auto & ii: t)
            {
                ii->draw();
            }
            
            if(call_predraw)
            {
                t.back()->postdraw();
            }
        }
    }
}

template<class T>
template<class... Args>
void InstanceContext<T>::renderArgs(bool call_predraw, Args&&... args) const
{
    for(size_t n=0; n<_instances.instances.size(); ++n)
    {
        auto& t = _instances.instances[ _priority[n] ];
        if(!t.empty())
        {
            if(call_predraw)
            {
                t.front()->predraw();
            }
            
            for(const auto & ii: t)
            {
                ii->draw(std::forward<Args>(args)...);
            }
            
            if(call_predraw)
            {
                t.back()->postdraw();
            }
        }
    }
}

template<class T>
void InstanceContext<T>::render(size_t object, bool call_predraw) const
{
    assert( object < _instances.instances.size() );
    const auto& t = _instances.instances[object];
    if(!t.empty())
    {
        if(call_predraw)
        {
            t.front()->predraw();
        }
        
        for(const auto & ii: t)
        {
            ii->draw();
        }
        
        if(call_predraw)
        {
            t.back()->postdraw();
        }
    }
}

template<class T>
void InstanceContext<T>::renderLayers(int priority_min, int priority_max, bool call_predraw)
{
    // 1) search for priority indexes
    int min_index = -1;
    int max_index = -1;
    
    for(size_t n=0; n<_instances.instances.size(); ++n)
    {
        int p = _object_priority[ _priority[n] ];
        //printf("%d ", p);
        
        if(min_index == -1 && p >= priority_min)
            min_index = n;
        
        if(p <= priority_max)
            max_index = n;
    }
    
    if(min_index == -1)
        return;
    
    // 2) render
    //printf("\n(%d %d)=>[%d %d]\n", priority_min, priority_max, min_index, max_index);
    for(size_t n=min_index; n<=max_index; ++n)
    {
        auto& t = _instances.instances[ _priority[n] ];
        if(!t.empty())
        {
            if(call_predraw)
            {
                t.front()->predraw();
            }
            
            for(const auto & ii: t)
            {
                ii->draw();
            }
            
            if(call_predraw)
            {
                t.back()->postdraw();
            }
        }
    }
}

template<class T>
template<class Object, class Partitioning, class...Args>
void InstanceContext<T>::setSpacePartitioning(Args&&... args)
{
    const size_t obj = Object::ObjectIndex;
    if(_partitioning.size() <= obj)
    {
        _partitioning.resize(obj + 1);
    }
    _partitioning[obj].reset( new Partitioning(std::forward<Args>(args)...) );
    
    // add existing instances to the partitionning engine
    if(_instances.instances.size() >= obj + 1)
    {
        for(auto& ins: _instances.instances[obj])
        {
            _partitioning[obj]->insert( ins.get() );
        }
    }
}

template<class T>
const SpacePartitioning<T> * InstanceContext<T>::getSpacePartitioning(size_t object)
{
    // @todo check out of bounds errors
    return _partitioning[ object ].get();
}

/// @todo (si possible?) optimiser le retrieve en faisant le cast coté implémentation du space partitioning
template<class T>
template<class Object>
std::vector<Object*> InstanceContext<T>::retrieveInstances(float x1, float y1, float x2, float y2, int param) const
{
    auto& sp = _partitioning[Object::ObjectIndex];
    if(sp)
    {
        std::vector<T*> ins = sp->retrieve(x1, y1, x2, y2, param);
        std::vector<Object*> ret(ins.size());
        for(size_t i=0; i<ins.size(); ++i)
            ret[i] = reinterpret_cast<Object*>(ins[i]);
           return ret;
    }
    else
    {
        // @todo
    }
    return std::vector<Object*>();
}


template<class T>
void InstanceContext<T>::setRoom(const Room & room)
{
    _room = room;
}
template<class T>
const Room & InstanceContext<T>::room() const
{
    return _room;
}
template<class T>
void InstanceContext<T>::addLayer(std::shared_ptr<TileMap> layer)
{
    _room.addLayer(layer);
}
template<class T>
void InstanceContext<T>::setGlobal()
{
    _global = this;
}
template<class T>
InstanceContext<T> & InstanceContext<T>::getDefault()
{
    return *_def;
}
template<class T>
InstanceContext<T> & InstanceContext<T>::getGlobal()
{
    return *_global;
}
template<class T>
void InstanceContext<T>::setGlobal(InstanceContext & ic)
{
    _global = &ic;
}
