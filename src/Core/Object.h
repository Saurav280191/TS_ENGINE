#pragma once
#include "EntityManager/Entity.h"
#include "EntityManager/EntityManager.h"
#include "SceneManager/Node.h"

namespace TS_ENGINE
{
	class Node;
	class Object
	{
	public:
		Object();
		~Object();

		virtual void Initialize() = 0;
		virtual void SetName(const std::string& name) = 0;
		virtual void Update(Ref<Shader> shader, float deltaTime) = 0;

		const std::string& GetName() const
		{
			return mName;
		}
		const EntityType GetEntityType() const;				
		EntityID GetEntityID();

		const Ref<Node> GetNode()
		{
			return mNode;
		}
	protected:
		Ref<Node> mNode;
		std::string mName;		
		EntityType mEntityType;
		EntityID mEntityID;
	private:
	};
}

