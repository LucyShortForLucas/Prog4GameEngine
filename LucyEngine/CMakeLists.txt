cmake_minimum_required(VERSION 3.26)
project(LucyEngine)

add_library(${PROJECT_NAME} STATIC
  ../Minigin/Font.cpp
  ../Minigin/Texture2D.cpp 
  eng/src/Actor.cpp
  eng/src/Engine.cpp
  eng/src/AbstractComponent.cpp 
  components/src/TextureRenderer.cpp
  components/src/TextRenderer.cpp
  components/src/Transform2d.cpp
  components/src/Rotator.cpp
  eng/src/Services.cpp 
  eng/src/LucyRenderer.cpp 
  eng/src/ResourceLoader.cpp
  eng/src/GameTime.cpp 
  components/src/FpsTracker.cpp
  components/src/ThrashTheCache.cpp
  Input/src/CommandInputGroup.cpp
  Input/src/AbstractCommand.cpp
  utils/src/EventSource.cpp
  utils/src/EventQueue.cpp
  Input/src/Input.cpp
  Input/src/KeyboardState.cpp
  Input/src/GamepadState.cpp
  Commands/src/Move.cpp
  components/src/HpScore.cpp
  components/src/HpScoreDisplay.cpp
  Commands/src/Kill.cpp
  Commands/src/AddScore.cpp
  eng/AudioPlayer.h eng/src/AudioPlayer.cpp
 "eng/Logger.h" "eng/src/Logger.cpp" "utils/src/States.cpp" "eng/src/Serialization.cpp")

target_include_directories(${PROJECT_NAME} PUBLIC
  eng components utils Input Commands ../Minigin
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

include(FetchContent)

# glm
find_package(glm CONFIG QUIET)
if(NOT glm_FOUND)
  FetchContent_Declare(glm URL https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip)
   set(GLM_BUILD_LIBRARY OFF CACHE BOOL "" FORCE)
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) 
  FetchContent_MakeAvailable(glm)
endif()

# SDL3
if (WIN32)
  FetchContent_Declare(SDL3 URL https://www.libsdl.org/release/SDL3-devel-3.4.0-VC.zip)
  FetchContent_MakeAvailable(SDL3)
  set(SDL3_DLL_PATH "${sdl3_SOURCE_DIR}/lib/x64/SDL3.dll" CACHE INTERNAL "")
  list(PREPEND CMAKE_PREFIX_PATH "${sdl3_SOURCE_DIR}")
  find_package(SDL3 CONFIG REQUIRED)
endif()

# SDL3_ttf
if (WIN32)
  FetchContent_Declare(SDL3_ttf URL https://www.libsdl.org/projects/SDL_ttf/release/SDL3_ttf-devel-3.2.2-VC.zip)
  FetchContent_MakeAvailable(SDL3_ttf)
  set(SDL3_TTF_DLL_PATH "${sdl3_ttf_SOURCE_DIR}/lib/x64/SDL3_ttf.dll" CACHE INTERNAL "")
  list(PREPEND CMAKE_PREFIX_PATH "${sdl3_ttf_SOURCE_DIR}")
  find_package(SDL3_ttf CONFIG REQUIRED)
endif()

# SDL3_mixerI
set(SDL3_MIXER_DLL_PATH "${sdl3_mixer_SOURCE_DIR}/lib/x64/SDL3_mixer.dll" CACHE INTERNAL "")
set(SDL3MIXER_BUILD_OPUS OFF CACHE BOOL "" FORCE)
set(SDL3MIXER_BUILD_FLAC_LIBFLAC OFF CACHE BOOL "" FORCE)
set(SDL3MIXER_BUILD_FLAC_DRFLAC ON CACHE BOOL "" FORCE)
set(SDL3MIXER_BUILD_MPG123 OFF CACHE BOOL "" FORCE)
set(SDL3MIXER_BUILD_XMP OFF CACHE BOOL "" FORCE)
set(SDL3MIXER_BUILD_MOD_XMP OFF CACHE BOOL "" FORCE)

FetchContent_Declare(SDL3_mixer
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
  GIT_TAG main
)
FetchContent_MakeAvailable(SDL3_mixer)

# ImGui
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui GIT_TAG v1.92.5-docking GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${sdl3_SOURCE_DIR}/include)
target_link_libraries(imgui PUBLIC SDL3::SDL3)

# Nlohmann Json
include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    URL      https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
    URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
)

FetchContent_MakeAvailable(nlohmann_json)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${nlohmann_json_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
  SDL3::SDL3 
  SDL3_ttf::SDL3_ttf 
  SDL3_mixer::SDL3_mixer
  glm::glm 
  imgui 
  Xinput
  nlohmann_json::nlohmann_json
)