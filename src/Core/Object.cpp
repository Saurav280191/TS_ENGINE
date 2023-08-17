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
		mNode->SetNodeRef(mNode);

		Ref<Shader> shader = TS_ENGINE::Shader::Create("HDRLighting", "HDRLighting.vert", "HDRLighting.frag");
		mMaterial = CreateRef<Material>("HDRLightingMaterial", shader);
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

	//TODO: Move the GameObject properties to GameObject class after implementing ECS
	void Object::SetMaterial(Ref<Material> material)
	{
		mMaterial = material;
	}
	Ref<Material> Object::GetMaterial() const
	{
		return mMaterial;		
	}
	
	void Object::EnableDepthTest()
	{
		mDepthTestEnabled = true;
	}
	void Object::DisableDepthTest()
	{
		mDepthTestEnabled = false;
	}
	void Object::EnableAlphaBlending()
	{
		mAlphaBlendingEnabled = true;
	}
	void Object::DisableAlphaBlending()
	{
		mAlphaBlendingEnabled = false;
	}
}
