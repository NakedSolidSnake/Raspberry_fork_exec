cmake_minimum_required(VERSION 3.7.0)

project(fork_exec)

set (CMAKE_C_FLAGS "-D_GNU_SOURCE -Wall -std=c99")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_BUILD_TYPE Debug)


#build main
add_executable(main "")

target_sources(main
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/src/main.c
  )

target_include_directories(main 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/lib  
  )

install(TARGETS main DESTINATION bin)


#build counter
add_executable(counter "")

target_sources(counter
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/src/counter.c
  )
 
install(TARGETS counter DESTINATION bin)


#build daemon_standard
add_executable(daemon_standard "")

target_sources(daemon_standard
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/src/daemon_standard.c
  )
 
install(TARGETS daemon_standard DESTINATION bin)

#build daemon_systemcall
add_executable(daemon_systemcall "")

target_sources(daemon_systemcall
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/src/daemon_systemcall.c
  )
 
install(TARGETS daemon_systemcall DESTINATION bin)