cmake_minimum_required(VERSION 3.8)
project(42run)

#Specific fix
if (APPLE)
	set(CMAKE_CXX_FLAGS "-stdlib=libc++")
endif ()

#Setup Glad
set(GLAD_API "gl=4.1")
add_subdirectory(glad ${CMAKE_BINARY_DIR}/glad/build)

#Setup GLFW
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_INSTALL "" OFF)
add_subdirectory(glfw ${CMAKE_BINARY_DIR}/glfw/build)

#Setup Assimp
set(BUILD_SHARED_LIBS OFF)
set(ASSIMP_NO_EXPORT ON)
add_subdirectory(assimp ${CMAKE_BINARY_DIR}/assimp/build)

#Setup FreeType
add_subdirectory(freetype ${CMAKE_BINARY_DIR}/freetype/build)

#Setup SFML Audio Lib
option(BUILD_SHARED_LIBS "" FALSE)
option(SFML_BUILD_WINDOW "" FALSE)
option(SFML_BUILD_GRAPHICS "" FALSE)
option(SFML_BUILD_NETWORK "" FALSE)
option(SFML_BUILD_DOC "" FALSE)
add_definitions(-DSFML_STATIC)
add_subdirectory(SFML ${CMAKE_BINARY_DIR}/SFML/build)

#Setup cpp version and flags
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Setup directories
include_directories(includes
		glm
		stb
		glad/build/includes
		glfw/include/GLFW
		assimp/include
		assimp/build/include
		freetype/include
		SFML/include
		${CMAKE_BINARY_DIR}/assimp/build/include)
link_directories(${CMAKE_BINARY_DIR}/glad/build
		${CMAKE_BINARY_DIR}/glfw/build/src
		${CMAKE_BINARY_DIR}/assimp/build/code
		${CMAKE_BINARY_DIR}/freetype/build
		${CMAKE_BINARY_DIR}/SFML/build/lib)

#Setup Main Lib with all compilations flag
set(SOURCE_FILES
		srcs/Cameras/Camera.cpp
		srcs/GameEntities/basic/Cubemap.cpp
		srcs/Exceptions/GeneralException.cpp
		srcs/WindowManager/Glfw_manager.cpp
		srcs/Interfaces/IEntity.cpp
		srcs/Interfaces/IInteractive.cpp
		srcs/Interfaces/ICollidable.cpp
		srcs/WindowManager/Input.cpp
		srcs/OpenGL/oGL_module.cpp
		srcs/OpenGL/Shader.cpp
		srcs/GameEntities/basic/Simple_box.cpp
		srcs/OpenGL/Texture.cpp
		srcs/WindowManager/Window.cpp
		srcs/Platformer/World.cpp
		srcs/Model/Model.cpp
		srcs/Model/Mesh.cpp
		srcs/GameEntities/basic/Prop.cpp
		srcs/UI/FontSet.cpp
		srcs/UI/Ui.cpp
		srcs/GameEntities/utility/CollisionBox.cpp
		srcs/Cameras/ThirdPersonCamera.cpp
		srcs/GameEntities/composed/Player.cpp
		srcs/GameEntities/composed/CollidableBox.cpp
		srcs/Interfaces/IGravity.cpp
		srcs/GameEntities/basic/Sprite.cpp
		srcs/GameEntities/composed/CollidableProp.cpp
		srcs/Audio/Audio.cpp
		srcs/WindowManager/Gamepad.cpp
		srcs/GameEntities/composed/Room.cpp
		srcs/Interfaces/ITranslatable.cpp
		srcs/Platformer/InitDebugWorld.cpp
		srcs/Runner/RunnerWorld.cpp
		srcs/Runner/InitRunnerWorld.cpp
		srcs/Runner/TemplatedRoom.cpp
		srcs/UI/ShaderSurface/ShaderSurface.cpp
		srcs/Runner/loops/Character_screen.cpp
		srcs/Runner/loops/Title_screen.cpp
		srcs/Runner/loops/Main_loop.cpp
		srcs/Runner/loops/Game_over_loop.cpp
		srcs/Utility/Score.cpp
		srcs/Runner/loops/Loop_input.cpp
		srcs/UI/ShaderSurface/OrientableShaderSurface.cpp
		srcs/OpenGL/Framebuffer.cpp
		srcs/UI/ShaderSurface/TextureShaderBuffer.cpp)
add_library(engine STATIC ${SOURCE_FILES})
target_link_libraries(engine glad glfw assimp freetype sfml-audio sfml-system)
target_compile_options(engine PRIVATE -Wall -Wextra)

#Creating executable
set(SOURCE_FILES_MAIN
		srcs/main.cpp)
add_executable(42run ${SOURCE_FILES_MAIN})
target_compile_options(42run PRIVATE -Wall -Wextra)

#Build Order
add_dependencies(engine glad glfw assimp freetype sfml-audio sfml-system)
add_dependencies(42run engine)
target_link_libraries(42run glad glfw assimp freetype sfml-audio sfml-system engine)

#Copy assets
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_BINARY_DIR})
file(COPY models DESTINATION ${CMAKE_BINARY_DIR})
file(COPY fonts DESTINATION ${CMAKE_BINARY_DIR})
file(COPY sounds DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ControllerConfigFiles/gamecontrollerdb_205.txt DESTINATION ${CMAKE_BINARY_DIR}/ControllerConfigFiles)