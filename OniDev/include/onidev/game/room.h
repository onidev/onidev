#ifndef ONIDEV_GAME_ROOM_H_INCLUDED
#define ONIDEV_GAME_ROOM_H_INCLUDED

#include <vector>
#include <memory>

namespace od
{

class TileMap;

class Room
{
    std::vector< std::shared_ptr< TileMap > > _layers;
    
public:
    void addLayer(std::shared_ptr<TileMap> layer);
    
    const std::vector< std::shared_ptr< TileMap > > & layers() const { return _layers; }
};

}

#endif
