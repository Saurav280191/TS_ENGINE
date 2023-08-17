#pragma once
#include "EntityManager/Entity.h"
#include "EntityManager/EntityManager.h"
#include "SceneManager/Node.h"
#include "Renderer/Material.h"

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

#pragma region GameObject specific
	//TODO: Move these properties from Object to GameObject class once ECS is implemneted
	public:
		void SetMaterial(Ref<Material> material);
		Ref<Material> GetMaterial() const;

		void EnableDepthTest();
		void DisableDepthTest();
		void EnableAlphaBlending();
		void DisableAlphaBlending();
	private:

	protected:
		Ref<Material> mMaterial;
		bool mDepthTestEnabled;
		bool mAlphaBlendingEnabled;		
#pragma endregion
	};
}

