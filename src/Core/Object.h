#pragma once
#include "EntityManager/Entity.h"
#include "EntityManager/EntityManager.h"
#include "SceneManager/Node.h"

namespace TS_ENGINE
{
	enum class PrimitiveType
	{
		QUAD,
		CUBE,
		SPHERE,
		CYLINDER,
		CONE,
		MODEL,
		EMPTY
	};

	class Node;
	class Object
	{
	public:
		Object();
		~Object();

		void SetName(const std::string& name);
		void ReplaceNode(Ref<Node> node);

		//Initialize and Update can change form for every child class
		virtual void Initialize() = 0;
		virtual void Update(Ref<Shader> shader, float deltaTime) = 0;		
		virtual void DeleteMeshes() = 0;

		const std::string& GetName() const
		{
			return mName;
		}
		const EntityType GetEntityType() const
		{
			return mEntityType;
		}	
		const EntityID GetEntityID() const
		{
			return mEntityID;
		}
		const Ref<Node> GetNode() const
		{
			return mNode;
		}
		const PrimitiveType GetPrimitiveType() const
		{
			return mPrimitiveType;
		}
	protected:
		std::string mName;		
		Ref<Node> mNode;
		EntityType mEntityType;		
		EntityID mEntityID;
		PrimitiveType mPrimitiveType;//Only for GameObject EntityType
	};
}

