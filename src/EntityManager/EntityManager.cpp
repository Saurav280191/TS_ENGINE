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

	Ref<Entity> EntityManager::Register(const std::string& name)// , EntityType entityType)
	{
		Ref<Entity> entity = CreateRef<Entity>(name);// , entityType);
		mEntityLookUp.insert({ entity->GetEntityID(), mEntities.size() });
		mEntities.push_back(entity);

		std::stringstream ss;
		ss << "Added new object named: " << name
			//<< " of type: " << Entity::GetEntityTypeStr(entityType)
			<< " to EntityManager with ID: " << entity->GetEntityID();

		TS_CORE_INFO(ss.str());

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
