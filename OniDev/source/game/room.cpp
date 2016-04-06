// Copyright 2013-2015 Boris Marmontel, All Rights Reserved.

/*=============================================================================
	room.cpp
=============================================================================*/

#include <onidev/game/room.h>

namespace od
{

void Room::addLayer(std::shared_ptr<TileMap> layer)
{
    _layers.push_back(layer);
}

}
