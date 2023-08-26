#include "tspch.h"
#include "EntityManager/Entity.h"

namespace TS_ENGINE
{
	EntityID Entity::mIdCount = 0;
	std::queue<EntityID> Entity::mIdRemoved;

	Entity::Entity(const std::string& name) : // , EntityType entityType) :
		mName(name)//,
		//mEntityType(entityType)
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

	/*const char* Entity::GetEntityTypeStr(EntityType type)
	{
		const char* typeStr = "";

		switch (type)
		{
		case EntityType::NODE:
			typeStr = "NODE";
			break;
		case EntityType::GAMEOBJECT:
			typeStr = "GAMEOBJECT";
			break;
		case EntityType::CAMERA:
			typeStr = "CAMERA";
			break;
		case EntityType::DEFAULT:
			typeStr = "DEFAULT";
			break;
		}

		return typeStr;
	}

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
