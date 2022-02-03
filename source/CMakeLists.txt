cmake_minimum_required(VERSION 3.16)

set(THIRDPARTY_PREFIX "" CACHE STRING "Third-party prefix.")

set(URLAPP_PROJECT urlapp)
set(URLAPP_EXE     urlapp)

set(URLAPP_COMPONENT_RUNTIME ${URLAPP_PROJECT}-runtime)

project(
  ${URLAPP_PROJECT}
    VERSION     0.1.1
    DESCRIPTION "Url app example."
    LANGUAGES   CXX)

find_package(Threads REQUIRED)

add_executable(${URLAPP_EXE})
target_sources(${URLAPP_EXE} PRIVATE urlapp.cpp)
target_include_directories(${URLAPP_EXE} PUBLIC ${THIRDPARTY_PREFIX}/include)
target_link_directories(${URLAPP_EXE} PRIVATE ${THIRDPARTY_PREFIX}/lib)
target_link_libraries(${URLAPP_EXE} PRIVATE curl wolfssl Threads::Threads)

include(GNUInstallDirs)

install(
  TARGETS ${URLAPP_EXE}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT ${URLAPP_COMPONENT_RUNTIME})
