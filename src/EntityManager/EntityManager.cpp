#include "tspch.h"
#include "EntityManager/EntityManager.h"

namespace TS_ENGINE
{
	Ref<EntityManager> EntityManager::mInstance = NULL;

	Ref<EntityManager> EntityManager::GetInstance()
	{
		if (mInstance == NULL)
			mInstance = CreateRef<EntityManager>();

		return mInstance;
	}

	Ref<Entity> EntityManager::Register(const std::string& name, const EntityType& entityType)
	{
		Ref<Entity> entity = CreateRef<Entity>(name, entityType);
		mEntityLookUp.insert({ entity->GetEntityID(), mEntities.size() });
		mEntities.push_back(entity);

		TS_CORE_TRACE("New entity registered with Name: {0}, Type: {1}, EntityID: {2}", name.c_str(), Entity::GetEntityTypeStr(entityType), entity->GetEntityID());

		return entity;
	}

	Ref<Entity> EntityManager::Get(EntityID id)
	{
		auto it = mEntityLookUp.find(id);

		if (it != mEntityLookUp.end())
			return Ref<Entity>(mEntities[it->second]);
		else
			return Ref<Entity>();
	}

	void EntityManager::Remove(EntityID id)
	{
		auto it = mEntityLookUp.find(id);

		if (it != mEntityLookUp.end())
		{
			EntityCollectionIndex i = it->second;
			EntityID back = mEntities.back()->GetEntityID();

			//Swap and pop
			std::swap(mEntities[i], mEntities.back());
			mEntities.pop_back();

			mEntityLookUp[back] = i;
			mEntityLookUp.erase(id);
		}
	}
	
	void EntityManager::PrintEntities()
	{
		TS_CORE_TRACE("Following entities are registered: ");

		for (auto& entity : mEntities)
		{
			TS_CORE_TRACE(entity->GetName().c_str());
		}
	}
}
