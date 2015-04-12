set(SourceFiles
    ${SourceFiles} 
    ${CMAKE_CURRENT_SOURCE_DIR}/Listener.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/Main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/Socket.cc
)

add_subdirectory(Http)
include_directories(.)
include_directories(Http)

add_executable(Test ${SourceFiles})

