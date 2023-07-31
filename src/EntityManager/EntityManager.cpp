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

	EntityID EntityManager::Instantiate(const std::string& name, EntityType entityType)
	{
		std::shared_ptr<Entity> e = std::make_shared<Entity>(name, entityType);
		mEntityLookUp.insert({e->GetEntityID(), mEntities.size() });
		mEntities.push_back(e);

		std::stringstream ss;
		ss << "Added new object named: " << name
		<< " of type: " << Entity::GetEntityTypeStr(entityType)
		<< " to EntityManager with ID: " << e->GetEntityID();

		TS_CORE_INFO(ss.str());

		return e->GetEntityID();
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
}
