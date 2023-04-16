#pragma once
#include "Node.h"

namespace TS_ENGINE
{
	class Scene
	{
	public:
		Scene(std::string name);		
		~Scene();

		const Ref<Node> GetSceneNode() const
		{
			return mSceneNode;
		}

		void Draw(Ref<Shader> shader);
	private:		
		Ref<Node> mSceneNode;
	};
}


