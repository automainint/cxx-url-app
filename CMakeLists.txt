cmake_minimum_required(VERSION 3.16)

set(URLAPP_PROJECT urlapp)
set(URLAPP_EXE     urlapp)
set(URLAPP_OBJ     urlapp-obj)

project(
  ${URLAPP_PROJECT}
    VERSION     0.1.1
    DESCRIPTION "Url app example."
    LANGUAGES   CXX)

include(ExternalProject)

ExternalProject_Add(
  wolfssl
  GIT_REPOSITORY  https://github.com/wolfSSL/wolfssl.git
  GIT_TAG         v4.8.1-stable
  CMAKE_ARGS
    -D CMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/_tmp
    -D CMAKE_INSTALL_INCLUDEDIR=../include
    -D CMAKE_INSTALL_LIBDIR=../lib
    -D WOLFSSL_ECC=yes
    -D WOLFSSL_EXAMPLES=no
    -D WOLFSSL_CRYPT_TESTS=no
    -D WOLFSSL_OPENSSLEXTRA=yes)

set(
  WOLFSSL_TARGET_FILE
  ${CMAKE_SOURCE_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX_CXX}wolfssl${CMAKE_STATIC_LIBRARY_SUFFIX_CXX})

ExternalProject_Add(
  curl
  GIT_REPOSITORY  https://github.com/curl/curl.git
  GIT_TAG         curl-7_81_0
  CMAKE_ARGS
    -D CMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/_tmp
    -D CMAKE_INSTALL_INCLUDEDIR=../include
    -D CMAKE_INSTALL_LIBDIR=../lib
    -D WolfSSL_INCLUDE_DIR=${CMAKE_SOURCE_DIR}/include
    -D WolfSSL_LIBRARY=${WOLFSSL_TARGET_FILE}
    -D CURL_USE_WOLFSSL=ON
    -D BUILD_CURL_EXE=OFF
    -D BUILD_SHARED_LIBS=OFF)

ExternalProject_Add_StepDependencies(curl build wolfssl)

add_library(${URLAPP_OBJ} OBJECT)
target_sources(${URLAPP_OBJ} PRIVATE source/urlapp.cpp)
add_executable(${URLAPP_EXE})
target_link_libraries(${URLAPP_EXE} ${URLAPP_OBJ})
