CMAKE_MINIMUM_REQUIRED( VERSION 3.7.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
project(TS_ENGINE)

message("Architecture: " ${CMAKE_ARCHITECTURE})
message("BuildType: " ${CMAKE_BUILD_TYPE})

# Set output directories for all builds (Debug, Release, etc.)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/build/$(platform)/$(Configuration)/lib )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/build/$(platform)/$(Configuration)/lib )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/build/$(platform)/$(Configuration)/bin )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

#SOURCE_FILES
set (SOURCE_FILES
src/GameObject.h
src/GameObject.cpp
src/ModelLoader.h
src/ModelLoader.cpp
src/stb_image.h
)

#Core Filter
file (GLOB CoreSrc 
src/Core/Application.h
src/Core/Application.cpp
src/Core/Base.h
src/Core/EntryPoint.h
src/Core/Input.h
src/Core/KeyCodes.h
src/Core/Layer.h
src/Core/Layer.cpp
src/Core/LayerStack.h
src/Core/LayerStack.cpp
src/Core/Log.h
src/Core/Log.cpp
src/Core/MouseCodes.h
src/Core/TSAssert.h
src/Core/tcpch.h
src/Core/tcpch.cpp
src/Core/Window.h
src/Core/Window.cpp
)
source_group("Core" FILES ${CoreSrc})

#Events Filter
file(GLOB EventsSrc
src/Events/ApplicationEvent.h
src/Events/Event.h
src/Events/KeyEvent.h
src/Events/MouseEvent.h
)
source_group("Events" FILES ${EventsSrc})

#ImGui Filter
file(GLOB ImGuiSrc 
src/ImGui/ImGuiBuild.cpp
src/ImGui/ImGuiLayer.h
src/ImGui/ImGuiLayer.cpp
)
source_group("ImGui" FILES ${ImGuiSrc})

#Platform/OpenGL Filter
file(GLOB PlatformOpenGLSrc
src/Platform/OpenGL/OpenGLBuffer.h
src/Platform/OpenGL/OpenGLBuffer.cpp
src/Platform/OpenGL/OpenGLContext.h
src/Platform/OpenGL/OpenGLContext.cpp
src/Platform/OpenGL/OpenGLRendererAPI.h
src/Platform/OpenGL/OpenGLRendererAPI.cpp
src/Platform/OpenGL/OpenGLShader.h
src/Platform/OpenGL/OpenGLShader.cpp
src/Platform/OpenGL/OpenGLTexture.h
src/Platform/OpenGL/OpenGLTexture.cpp
src/Platform/OpenGL/OpenGLUniformBuffer.h
src/Platform/OpenGL/OpenGLUniformBuffer.cpp
src/Platform/OpenGL/OpenGLVertexArray.h
src/Platform/OpenGL/OpenGLVertexArray.cpp
)
source_group("Platform\\OpenGL" FILES ${PlatformOpenGLSrc})

#Platform/Windows filters
file(GLOB PlatformWindowsSrc
src/Platform/Windows/WindowsInput.cpp
src/Platform/Windows/WindowsWindow.h
src/Platform/Windows/WindowsWindow.cpp
)
source_group("Platform\\Windows" FILES ${PlatformWindowsSrc})

#Primitive Filter
file (GLOB PrimitiveSrc 
src/Primitive/Cube.h
src/Primitive/Cube.cpp
src/Primitive/Mesh.h
src/Primitive/Mesh.cpp
src/Primitive/Model.h
src/Primitive/Model.cpp
src/Primitive/Quad.h
src/Primitive/Quad.cpp
src/Primitive/Sphere.h
src/Primitive/Sphere.cpp
)
source_group("Primitive" FILES ${PrimitiveSrc})

#Renderer Filter
file(GLOB RendererSrc
src/Renderer/Batcher.h
src/Renderer/Batcher.cpp
src/Renderer/Buffer.h
src/Renderer/Buffer.cpp
src/Renderer/Camera.h
src/Renderer/Camera.cpp
src/Renderer/GraphicsContext.h
src/Renderer/GraphicsContext.cpp
src/Renderer/RenderCommand.h
src/Renderer/RenderCommand.cpp
src/Renderer/Renderer.h
src/Renderer/Renderer.cpp
src/Renderer/RendererAPI.h
src/Renderer/RendererAPI.cpp
src/Renderer/Shader.h
src/Renderer/Shader.cpp
src/Renderer/Texture.h
src/Renderer/Texture.cpp
src/Renderer/TextureAtlas.h
src/Renderer/TextureAtlas.cpp
src/Renderer/TextureAtlasCreator.h
src/Renderer/TextureAtlasCreator.cpp
src/Renderer/TextureAtlasHelper.h
src/Renderer/TS_ENGINE.h
src/Renderer/UniformBuffer.h
src/Renderer/UniformBuffer.cpp
src/Renderer/VertexArray.h
src/Renderer/VertexArray.cpp
)
source_group("Renderer" FILES ${RendererSrc})

#Setting type of TS_Engine project ie. .lib in this case
add_library (TS_ENGINE
${SOURCE_FILES} #Source Files Default Filter
${CoreSrc} #Core Filter
${EventsSrc} #Events Filter
${ImGuiSrc} #ImGui Filter
${PlatformOpenGLSrc} #Platform OpenGL Filter
${PlatformWindowsSrc} #Platform Windows Filter
${PrimitiveSrc} #Primitives Filter
${RendererSrc} #Renderer Filter
)

#Add definations
add_compile_definitions(
    TS_PLATFORM_WINDOWS
    TS_DEBUG
    _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
     WIN32
     _DEBUG
     _WIN64
     BT_USE_DOUBLE_PRECISION
     USE_GRAPHICAL_BENCHMARK
     _CRT_SECURE_NO_WARNINGS
     _CRT_SECURE_NO_DEPRECATE
     _SCL_SECURE_NO_WARNINGS
     )

 #Include directories
target_include_directories (TS_ENGINE PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/src
${CMAKE_CURRENT_SOURCE_DIR}/src/Core
${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/include
${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/include/imgui
${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/include/imgui/backends
${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/include/glad
${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/include/assimp/include
${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/include/assimp/build/x64/include
)

target_precompile_headers(TS_ENGINE PRIVATE src/Core/tspch.h)