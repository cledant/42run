cmake_minimum_required(VERSION 3.8)
project(42run)

#Setup cpp version and flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if (APPLE)
	set(CMAKE_C_FLAGS "-stdlib=libc++")
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
add_subdirectory(SFML ${CMAKE_BINARY_DIR}/SFML/build)

#Setup 42Run
if (APPLE)
	set(CMAKE_CXX_FLAGS "-stdlib=libc++ -Wall -Werror -Wextra -O2 -g")
else ()
	set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -O2")
endif ()
include_directories(includes
		glm
		stb
		glad/build/includes
		glfw/include/GLFW
		assimp/include
		assimp/build/include
		freetype/include
		SFML/include)
link_directories(${CMAKE_BINARY_DIR}/glad/build
		${CMAKE_BINARY_DIR}/glfw/build/src
		${CMAKE_BINARY_DIR}/assimp/build/code
		${CMAKE_BINARY_DIR}/freetype/build
		${CMAKE_BINARY_DIR}/SFML/build/lib)
set(SOURCE_FILES
		srcs/Cameras/Camera.cpp
		srcs/GameEntities/Cubemap.cpp
		srcs/Exceptions/GeneralException.cpp
		srcs/WindowManager/Glfw_manager.cpp
		srcs/Interfaces/IEntity.cpp
		srcs/Interfaces/IInteractive.cpp
		srcs/Interfaces/ICollidable.cpp
		srcs/WindowManager/Input.cpp
		srcs/main.cpp
		srcs/OpenGL/oGL_module.cpp
		srcs/OpenGL/Shader.cpp
		srcs/GameEntities/Simple_box.cpp
		srcs/OpenGL/Texture.cpp
		srcs/WindowManager/Window.cpp
		srcs/Platformer/World.cpp
		srcs/Model/Model.cpp
		srcs/Model/Mesh.cpp
		srcs/GameEntities/Prop.cpp
		srcs/UI/FontSet.cpp
		srcs/UI/Ui.cpp
		srcs/Utility/CollisionBox.cpp
		srcs/Cameras/ThirdPersonCamera.cpp
		srcs/GameEntities/Player.cpp
		srcs/GameEntities/CollidableBox.cpp
		srcs/Interfaces/IGravity.cpp
		srcs/GameEntities/Sprite.cpp
		srcs/GameEntities/CollidableProp.cpp
		srcs/Audio/Audio.cpp
		srcs/WindowManager/Gamepad.cpp
		srcs/GameEntities/Room.cpp
		srcs/Interfaces/ITranslatable.cpp
		srcs/Platformer/InitDebugWorld.cpp
		srcs/Runner/RunnerWorld.cpp
		srcs/Runner/InitRunnerWorld.cpp
		srcs/Runner/TemplatedRoom.cpp
		srcs/UI/ShaderSurface.cpp srcs/Runner/loops/Character_screen.cpp srcs/Runner/loops/Title_screen.cpp srcs/Runner/loops/Main_loop.cpp srcs/Runner/loops/Game_over_loop.cpp)

#OS specific libs
if (APPLE)
	find_library(OPEN_GL OpenGL REQUIRED)
	find_library(COCOA COCOA REQUIRED)
	find_library(IOKIT IOkit REQUIRED)
	find_library(COREVIDEO CoreVideo REQUIRED)
	set(EXTRA_LIBS ${OPEN_GL} ${COCOA} ${IOKIT} ${COREVIDEO})
endif ()

add_executable(42run ${SOURCE_FILES})
target_link_libraries(42run glad glfw assimp freetype sfml-audio sfml-system ${EXTRA_LIBS})
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY textures DESTINATION ${CMAKE_BINARY_DIR})
file(COPY models DESTINATION ${CMAKE_BINARY_DIR})
file(COPY fonts DESTINATION ${CMAKE_BINARY_DIR})
file(COPY sounds DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ControllerConfigFiles/gamecontrollerdb_205.txt DESTINATION ${CMAKE_BINARY_DIR}/ControllerConfigFiles)