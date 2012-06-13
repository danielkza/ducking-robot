include(FindSDL)
include(FindSDL_image)
include(FindSDL_mixer)

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)

include_directories(${DUCKING_ROBOT_SOURCE_DIR}/game)

link_directories(${DUCKING_ROBOT_BINARY_DIR})
add_executable(ducking_robot WIN32 MACOSX_BUNDLE main.c)

target_link_libraries(ducking_robot ${SDL_LIBRARY})
target_link_libraries(ducking_robot ${SDLIMAGE_LIBRARY})
target_link_libraries(ducking_robot ${SDLMIXER_LIBRARY})
