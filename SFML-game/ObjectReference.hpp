#pragma once

#include <Map.hpp>
#include <ObjectLayer.hpp>
#include <ObjectSprite.hpp>

class ObjectReference
{
	Map *m_map;
	int m_id;
public:
	ObjectReference(Map *map, int id) :
		m_map{ map },
		m_id{ id }
	{}

	ObjectSprite *get()
	{
		for (auto &[layerName, layer] : m_map->m_objectMap)
		{
			for (auto &sprite : layer->objects)
			{
				if (sprite->gid == m_id)
					return sprite.get();
			}
		}
	}
};

