set(SourceFiles
    ${SourceFiles} 
    ${CMAKE_CURRENT_SOURCE_DIR}/Main.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/Socket.cc
)

include_directories(.)
add_executable(Test ${SourceFiles})

