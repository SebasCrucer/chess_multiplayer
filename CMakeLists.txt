cmake_minimum_required(VERSION 3.28)
project(chess_multiplayer)

set(CMAKE_CXX_STANDARD 17)

set(Boost "${PROJECT_SOURCE_DIR}/libs/boost_1_86_0")

include_directories(
        ${Boost}
)

set(SOURCES
        src/main.cpp
)

add_executable(chess_multiplayer main.cpp
        src/PlayersQueue.h
        src/PlayersQueue.cpp
        src/ChessServer.h
        src/ChessServer.cpp
        src/GameInstance.h
        src/GameInstance.cpp
        src/Player.h
        src/Player.cpp
        src/Session.h
        src/Session.cpp)
