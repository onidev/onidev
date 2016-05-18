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

#ifndef ONIDEV_GAME_CONTEXT_H_INCLUDED
#define ONIDEV_GAME_CONTEXT_H_INCLUDED

#include <onidev/game/container.h>
#include <onidev/game/room.h>
#include <onidev/collision/partitioning.h>
#include <memory>

namespace od
{

template<class T>
class InstanceContext
{
    // Le contexte par defaut (existe toujours)
    static InstanceContext * _def;
    // Le contexte global (de base est celui par défaut)
    static InstanceContext * _global;
    static const std::vector<std::shared_ptr<T>> _empty_array;
    
    InstanceContainer<T> _instances;
    std::vector<int> _priority, _object_priority;
    std::vector< std::unique_ptr<SpacePartitioning<T>> > _partitioning;
    
    Room _room;
    
    // permet de savoir si on peut erase un element du tableau
    // global d'instances ou s'il faut attendre
    bool _inside_loop_update = false;
    int  _loop_object = -1;
    int  _loop_cursor = -1;
    
    void partitioningInsertInstance(size_t object, T* ins);
    void updateObjectPriority(size_t object, int priority);
    
public:
    InstanceContext();
    ~InstanceContext();
    
    ///void addObjectIndex(size_t object, int priority);
    
    void add(size_t object, std::shared_ptr<T> ins, int priority);
    void add(size_t object, std::shared_ptr<T> ins, int priority, unsigned uid);
    std::shared_ptr<T> getSharedPtr(const T* ins) const;
    void remove(T * ins);
    void move(std::shared_ptr<T> ins); // move instance from other context
    void purge();
    template<class Object>
    void clear();
    
    T* find(size_t object, unsigned uid);
    template<class Object> T* find(unsigned uid);
    
    template<class Object>
    size_t instanceNumber() const;
    const std::vector<std::shared_ptr<T>> & instances(size_t object) const;
    template<class Object>
    const std::vector<std::shared_ptr<T>> & instances() const;
    size_t instanceNumber(size_t object) const;
    size_t instanceNumber() const;
    
    size_t lastObjectIndex() const { return _instances.instances.size(); }
    const std::vector<int>& renderPriority() const { return _priority; }
    
    void update(float deltaTime);
    void render() const;
    void renderObject(size_t object) const;
    void renderLayers(int priority_min, int priority_max) const;
    
    template<class... Args>
    void render(bool call_predraw, Args&&... args) const;
    
    template<class... Args>
    void renderObject(size_t object, bool call_predraw, Args&&... args) const;
    
    template<class... Args>
    void renderLayers(int priority_min, int priority_max, bool call_predraw, Args&&... args) const;
    
    template<class Object, class Partitioning, class...Args>
    void setSpacePartitioning(Args&&... args);
    const SpacePartitioning<T> * getSpacePartitioning(size_t object);
    template<class Object>
    std::vector<Object*> retrieveInstances(float x1, float y1, float x2, float y2, int param = 0) const;
    
    void setRoom(const Room & room);
    const Room & room() const;
    void addLayer(std::shared_ptr<TileMap> layer);
    
    void setGlobal();
    static InstanceContext & getDefault();
    static InstanceContext & getGlobal();
    static void setGlobal(InstanceContext & ic);
};

#include "context.inc"
    
}

#endif
