#include "tspch.h"
#include "Renderer/RenderCommand.h"

namespace TS_ENGINE {

	Scope<RendererAPI> RenderCommand::sRendererAPI = RendererAPI::Create();
}
