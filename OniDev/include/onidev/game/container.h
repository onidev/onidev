#ifndef ONIDEV_GAME_CONTAINER_H_INCLUDED
#define ONIDEV_GAME_CONTAINER_H_INCLUDED

#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <cassert>

namespace od
{

template<class T>
struct InstanceContainer
{
    std::vector<std::vector<std::shared_ptr<T>>> instances;
    
    // unique identifiers for multiplayer engine
    /// @todo can be deactivated with template parameter
    std::vector<std::map<unsigned, T*>> uid_map;
    std::vector<unsigned> last_uid;
    
    bool destroyed;
    
    InstanceContainer();
    ~InstanceContainer();
    
    void add(size_t object, std::shared_ptr<T> & ins);
    void add(size_t object, std::shared_ptr<T> & ins, unsigned uid);
    void remove(size_t object, T * ins, bool inside_loop, int loop_object, int& loop_cursor);
    void purge();
    
    T* find(size_t object, unsigned uid);
    
    size_t number(int object) const;
    size_t number() const;
};

#include "container.inc"

}

#endif
