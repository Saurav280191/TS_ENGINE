#include "tspch.h"
#include "Object.h"

namespace TS_ENGINE
{
	Object::Object() :
		mName("Default"),
		mEntityType(EntityType::DEFAULT),
		mEntityID(0)
	{
		mNode = CreateRef<Node>();
	}

	Object::~Object()
	{
		mNode.reset();
	}

	const EntityType Object::GetEntityType() const
	{
		return mEntityType;
	}

	EntityID Object::GetEntityID()
	{
		return mEntityID;
	}
}
