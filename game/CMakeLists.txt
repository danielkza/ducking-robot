include(FindSDL)
include(FindSDL_image)
include(FindSDL_mixer)

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)

include_directories(${DUCKING_ROBOT_SOURCE_DIR}/game)
include_directories(${SDL_INCLUDE_DIR}
                    ${SDLIMAGE_INCLUDE_DIR}
                    ${SDLMIXER_INCLUDE_DIR})

file(GLOB SOURCES "*.c")

link_directories(${DUCKING_ROBOT_BINARY_DIR})
add_executable(ducking_robot WIN32 MACOSX_BUNDLE ${SOURCES})

target_link_libraries(ducking_robot
                      ${SDL_LIBRARY}
                      ${SDLIMAGE_LIBRARY}
                      ${SDLMIXER_LIBRARY})

MACRO(CAR var)
  SET(${var} ${ARGV1})
ENDMACRO(CAR)

LIST(GET SDL_LIBRARY 0 SDL_LIB_DIR)
GET_FILENAME_COMPONENT(SDL_LIB_DIR "${SDL_LIB_DIR}" PATH)


LIST(GET SDLMIXER_LIBRARY 0 SDLMIXER_LIB_DIR)
GET_FILENAME_COMPONENT(SDLMIXER_LIB_DIR "${SDLMIXER_LIB_DIR}" PATH)


LIST(GET SDLIMAGE_LIBRARY 0 SDLIMAGE_LIB_DIR)
GET_FILENAME_COMPONENT(SDLIMAGE_LIB_DIR "${SDLIMAGE_LIB_DIR}" PATH)


add_custom_command(TARGET ducking_robot POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL_LIB_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}SDL${CMAKE_SHARED_LIBRARY_SUFFIX}" "${RUNTIME_OUTPUT_DIRECTORY}"
)

add_custom_command(TARGET ducking_robot POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDLMIXER_LIB_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}SDL_mixer${CMAKE_SHARED_LIBRARY_SUFFIX}" "${RUNTIME_OUTPUT_DIRECTORY}"
)

add_custom_command(TARGET ducking_robot POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDLIMAGE_LIB_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}SDL_image${CMAKE_SHARED_LIBRARY_SUFFIX}" "${RUNTIME_OUTPUT_DIRECTORY}"
)
