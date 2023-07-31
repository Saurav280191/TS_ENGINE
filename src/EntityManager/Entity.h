#pragma once
#include "tspch.h"

namespace TS_ENGINE
{
	enum EntityType
	{
		NODE,
		GAMEOBJECT,
		LIGHT,
		CAMERA,
		DEFAULT
	};

	typedef unsigned long int EntityID;

	class Entity
	{
	private:
		EntityID mId;
		static EntityID mIdCount;
		static std::queue<EntityID> mIdRemoved;

		std::string mName;
		EntityType mEntityType;
	public:
		Entity(const std::string& name, EntityType entityType);
		~Entity();


		EntityID GetEntityID() const;
		const char* GetName() const;

		static const char* GetEntityTypeStr(EntityType type);
		static const EntityType GetEntityTypeFromStr(std::string str);
	};
}

