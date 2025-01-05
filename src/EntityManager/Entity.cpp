#include "tspch.h"
#include "EntityManager/Entity.h"

namespace TS_ENGINE
{
	EntityID Entity::mIdCount = 0;
	std::queue<EntityID> Entity::mIdRemoved;

	Entity::Entity(const std::string& name, EntityType entityType) :
		mName(name),
		mEntityType(entityType)
	{
		if (!mIdRemoved.empty())
		{
			mId = mIdRemoved.front();
			mIdRemoved.pop();
		}
		else
		{
			mId = mIdCount++;
		}
	}

	Entity::~Entity()
	{
		mIdRemoved.push(mId);
	}

	EntityID Entity::GetEntityID() const
	{
		return mId;
	}

	void Entity::SetName(const std::string& name)
	{
		TS_CORE_TRACE("Setting name for Entity with entityID {0} to {1}", mId, name);
		mName = name;
	}

	const char* Entity::GetEntityTypeStr(EntityType type)
	{
		const char* typeStr = "";

		switch (type)
		{
		case EntityType::PRIMITIVE:
			typeStr = "PRIMITIVE";
			break;
		case EntityType::MESH:
			typeStr = "MESH";
			break;
		case EntityType::BONE:
			typeStr = "BONE";
			break;
		case EntityType::BONEGUI:
			typeStr = "BONEGUI";
			break;
		case EntityType::CAMERA:
			typeStr = "CAMERA";
			break;
		case EntityType::SKYBOX:
			typeStr = "SKYBOX";
			break;
		case EntityType::SCENE:
			typeStr = "SCENE";
			break;
		case EntityType::EMPTY:
			typeStr = "EMPTY";
			break;
		}

		return typeStr;
	}

	/*
	const EntityType Entity::GetEntityTypeFromStr(std::string str)
	{
		if (str == "NODE")
			return NODE;
		if (str == "GAMEOBJECT")
			return GAMEOBJECT;
		if (str == "CAMERA")
			return CAMERA;
		if (str == "DEFAULT")
			return DEFAULT;

		return DEFAULT;
	}*/
}
