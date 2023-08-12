#pragma once

#include "Core/Base.h"
#include "Events/Event.h"

namespace TS_ENGINE {
	
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const 
		{
			return mDebugName;
		}

	protected:
		std::string mDebugName;
	};
}
