cmake_minimum_required(VERSION 3.16)

option(URLAPP_BUILD_DEPENDENCIES "Build dependencies" ON)

set(URLAPP_WOLFSSL_TAG  "v5.4.0-stable" CACHE STRING "wolfSSL git tag")
set(URLAPP_CURL_TAG     "curl-7_84_0"   CACHE STRING "curl git tag")

project(urlapp-root)

include(ExternalProject)

if(URLAPP_BUILD_DEPENDENCIES)
  if("${CMAKE_STATIC_LIBRARY_PREFIX_CXX}${CMAKE_STATIC_LIBRARY_SUFFIX_CXX}" STREQUAL "")
    if(MSVC)
      set(CMAKE_STATIC_LIBRARY_PREFIX_CXX "")
      set(CMAKE_STATIC_LIBRARY_SUFFIX_CXX ".lib")
    else()
      set(CMAKE_STATIC_LIBRARY_PREFIX_CXX "lib")
      set(CMAKE_STATIC_LIBRARY_SUFFIX_CXX ".a")
    endif()
  endif()

  set(
    WOLFSSL_TARGET_FILE
    ${CMAKE_SOURCE_DIR}/thirdparty/lib/${CMAKE_STATIC_LIBRARY_PREFIX_CXX}wolfssl${CMAKE_STATIC_LIBRARY_SUFFIX_CXX})

  ExternalProject_Add(
    wolfssl
    GIT_REPOSITORY  https://github.com/wolfSSL/wolfssl.git
    GIT_TAG         ${URLAPP_WOLFSSL_TAG}
    CMAKE_ARGS
      -D CMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/thirdparty
      -D WOLFSSL_OPENSSLEXTRA=yes
      -D WOLFSSL_EXAMPLES=no
      -D WOLFSSL_CRYPT_TESTS=no
      -D BUILD_SHARED_LIBS=OFF)

  ExternalProject_Add(
    curl-fix
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/cmake/curl-fix
    CMAKE_ARGS
      -D CMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/thirdparty)

  ExternalProject_Add(
    curl
    GIT_REPOSITORY  https://github.com/curl/curl.git
    GIT_TAG         ${URLAPP_CURL_TAG}
    CMAKE_ARGS
      -D CMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/thirdparty
      -D WolfSSL_INCLUDE_DIR=${CMAKE_SOURCE_DIR}/thirdparty/include
      -D WolfSSL_LIBRARY=${WOLFSSL_TARGET_FILE}
      -D CURL_USE_WOLFSSL=ON
      -D CURL_ZLIB=OFF
      #-D CMAKE_USE_WOLFSSL=ON
      #-D CMAKE_ZLIB=OFF
      -D BUILD_CURL_EXE=OFF
      -D BUILD_SHARED_LIBS=OFF)
endif()

ExternalProject_Add(
  urlapp
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/source
  CMAKE_ARGS
    -D CMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/install
    -D THIRDPARTY_PREFIX=${CMAKE_SOURCE_DIR}/thirdparty)

if(URLAPP_BUILD_DEPENDENCIES)
  ExternalProject_Add_StepDependencies(curl-fix build wolfssl)
  ExternalProject_Add_StepDependencies(curl build curl-fix wolfssl)
  ExternalProject_Add_StepDependencies(urlapp build curl)
endif()
