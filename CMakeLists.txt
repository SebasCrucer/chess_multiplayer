cmake_minimum_required(VERSION 3.17)
project(chess_multiplayer)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system thread)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "No se pudieron encontrar las librerías de Boost")
endif()

include_directories(${Boost_INCLUDE_DIRS})

set(SOURCES
        main.cpp
        src/PlayersQueue.cpp
        src/ChessServer.cpp
        src/GameInstance.cpp
        src/Player.cpp
        src/Session.cpp
)

add_executable(chess_multiplayer ${SOURCES})

target_link_libraries(chess_multiplayer PRIVATE ${Boost_LIBRARIES})
