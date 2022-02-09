cmake_minimum_required(VERSION 3.16)

option(URLAPP_BUILD_DEPENDENCIES "Build dependencies" ON)

set(URLAPP_WOLFSSL_TAG  "v5.1.1-stable" CACHE STRING "wolfSSL git tag")
set(URLAPP_CURL_TAG     "curl-7_81_0"   CACHE STRING "curl git tag")

project(urlapp-root)

include(ExternalProject)

if(URLAPP_BUILD_DEPENDENCIES)
  ExternalProject_Add(
    wolfssl
    GIT_REPOSITORY    https://github.com/wolfSSL/wolfssl.git
    GIT_TAG           ${URLAPP_WOLFSSL_TAG}
    BUILD_IN_SOURCE   ON
    CONFIGURE_COMMAND ./autogen.sh
    COMMAND
      ${CMAKE_COMMAND} -E env "CC=${CMAKE_C_COMPILER}"
      ${CMAKE_COMMAND} -E env "CXX=${CMAKE_CXX_COMPILER}"
      ./configure --prefix=${CMAKE_CURRENT_SOURCE_DIR}/thirdparty --enable-static --disable-shared --enable-opensslextra --disable-examples --disable-crypttests
    BUILD_COMMAND     $(MAKE)
    INSTALL_COMMAND   $(MAKE) install)

  ExternalProject_Add(
    curl
    GIT_REPOSITORY    https://github.com/curl/curl.git
    GIT_TAG           ${URLAPP_CURL_TAG}
    BUILD_IN_SOURCE   ON
    CONFIGURE_COMMAND ./buildconf
    COMMAND
      ${CMAKE_COMMAND} -E env "CC=${CMAKE_C_COMPILER}"
      ${CMAKE_COMMAND} -E env "CXX=${CMAKE_CXX_COMPILER}"
      ${CMAKE_COMMAND} -E env "CPPFLAGS=-I${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/include -I${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/include/wolfssl"
      ${CMAKE_COMMAND} -E env "LDFLAGS=-L${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/lib"
      ${CMAKE_COMMAND} -E env "LIBS=-lm"
      ./configure --prefix=${CMAKE_SOURCE_DIR}/thirdparty --disable-shared --without-zlib --with-wolfssl
    BUILD_COMMAND     $(MAKE)
    INSTALL_COMMAND   $(MAKE) install)
endif()

ExternalProject_Add(
  urlapp
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source
  CMAKE_ARGS
    -D CMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/install
    -D THIRDPARTY_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

if(URLAPP_BUILD_DEPENDENCIES)
  ExternalProject_Add_StepDependencies(curl build wolfssl)
  ExternalProject_Add_StepDependencies(urlapp build curl)
endif()
