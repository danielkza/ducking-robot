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
file(GLOB HEADERS "*.h")

link_directories(${DUCKING_ROBOT_BINARY_DIR})
add_executable(ducking_robot WIN32 MACOSX_BUNDLE ${SOURCES} ${HEADERS})

target_link_libraries(ducking_robot
                      ${SDL_LIBRARY}
                      ${SDLIMAGE_LIBRARY}
                      ${SDLMIXER_LIBRARY})

# Retrieve the paths to shared libraries to be copied to output folder

macro(copy_shared_lib target libstring libname)
    SET(libvar "${libstring}")
    list(GET libvar 0 libpath)
    
    get_filename_component(libpath "${libpath}" PATH)
    
    set(libpath "${libpath}/${CMAKE_SHARED_LIBRARY_PREFIX}${libname}${CMAKE_SHARED_LIBRARY_SUFFIX}")

    add_custom_command(TARGET ducking_robot POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${libpath}"
      "$<TARGET_FILE_DIR:${target}>"
    )
endmacro()

copy_shared_lib(ducking_robot "${SDL_LIBRARY}" "SDL")
copy_shared_lib(ducking_robot "${SDLIMAGE_LIBRARY}" "SDL_image")
copy_shared_lib(ducking_robot "${SDLMIXER_LIBRARY}" "SDL_mixer")
