#pragma once
#include "Core/tspch.h"
#include "Renderer/Material.h"
#include "Renderer/Lighting/Light.h"

namespace TS_ENGINE {

	class DirectionalLight : public Light
	{
	public:
		DirectionalLight();
		~DirectionalLight();	
	};
}

