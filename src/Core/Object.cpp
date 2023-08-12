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

	void Object::SetName(const std::string& name)
	{
		mName = name;
		mNode->SetName(name);		
	}

	void Object::ReplaceNode(Ref<Node> node)
	{
		mNode->Destroy();
		mNode.reset();
		mNode = nullptr;

		mNode = node;
	}
}
