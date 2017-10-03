cmake_minimum_required(VERSION 3.0)

project("Maze Runner" VERSION 0.1.0)

include(GNUInstallDirs)

find_package(gf REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML2 REQUIRED sfml-audio>=2.1)
pkg_check_modules(SFML2 REQUIRED sfml-network>=2.1)

set(RC_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/maze-runner")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

if(MSVC)
    add_definitions(-Wall -g -O2 -std=c++11)
else()
    add_definitions(-Wall -Wextra -g -O2 -std=c++11)
endif()

add_executable(maze-runner
        src/maze-runner.cc
        src/local/Map.cc
        src/local/Player.cc
        src/local/Network.cc)


#add_executable(maze-runner-host
#    src/maze-runner-host.cc
#    src/local/Map.cc
#    src/local/Player.cc
#    src/local/Network.cc)

#add_executable(maze-runner-server
#        src/maze-runner-server.cc
#        src/local/Map.cc
#        src/local/Player.cc
#        src/local/Network.cc)

target_include_directories(maze-runner
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(maze-runner
    gf::gf0
    ${SFML2_LIBRARIES}
)

install(
    TARGETS maze-runner
    RUNTIME DESTINATION games
)

#install(
#    DIRECTORY "${CMAKE_SOURCE_DIR}/data/run-and-crawl"
#    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
#)
