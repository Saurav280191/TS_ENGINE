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

		TS_CORE_TRACE("New Node Entity Registered With Name: {0}, Type: {1}, EntityID: {2}", name.c_str(), Entity::GetEntityTypeStr(entityType), entity->GetEntityID());

		return entity;
	}

	Ref<Entity> EntityManager::Get(EntityID id)
	{
		auto it = mEntityLookUp.find(id);

		if (it != mEntityLookUp.end())
		{
			return Ref<Entity>(mEntities[it->second]);
		}
		else
		{
			TS_CORE_ERROR("Could not find an entity with ID: {0}", id);
			return nullptr;
		}
	}

	Ref<Entity> EntityManager::GetEntityByName(std::string _name)
	{
		for (auto& entity : mEntities)
		{
			if (entity->GetName() == _name)
			{
				return entity;
			}
		}

		TS_CORE_ERROR("Could not find entity with name: {0}", _name);
		return nullptr;
	}

	void EntityManager::Remove(EntityID id)
	{
		TS_CORE_INFO("Removed entity with name: {0}, id: {1} from registry", Get(id)->GetName().c_str(), id);
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
		else
		{
			TS_CORE_ERROR("Could not find an entity with ID: {0}", id);
		}
	}

	void EntityManager::Flush()
	{
		mEntityLookUp.clear();
		mEntities.clear();
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
