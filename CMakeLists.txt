cmake_minimum_required(VERSION 3.16)

option(URLAPP_BUILD_DEPENDENCIES "Build dependencies" ON)

set(URLAPP_OPENSSL_TAG  "openssl-3.0.1" CACHE STRING "OpenSSL git tag")
set(URLAPP_CURL_TAG     "curl-7_81_0"   CACHE STRING "curl git tag")

project(urlapp-root)

include(ExternalProject)

if(URLAPP_BUILD_DEPENDENCIES)
  ExternalProject_Add(
    openssl
    GIT_REPOSITORY    https://github.com/openssl/openssl.git
    GIT_TAG           ${URLAPP_OPENSSL_TAG}
    BUILD_IN_SOURCE   ON
    CONFIGURE_COMMAND
      ${CMAKE_COMMAND} -E env "CC=${CMAKE_C_COMPILER}"
      ${CMAKE_COMMAND} -E env "CXX=${CMAKE_CXX_COMPILER}"
      ./config no-shared --prefix=${CMAKE_CURRENT_SOURCE_DIR}/thirdparty --openssldir=${CMAKE_CURRENT_SOURCE_DIR}/thirdparty
    BUILD_COMMAND     $(MAKE)
    INSTALL_COMMAND   $(MAKE) install)

  ExternalProject_Add(
    curl
    GIT_REPOSITORY    https://github.com/curl/curl.git
    GIT_TAG           ${URLAPP_CURL_TAG}
    BUILD_IN_SOURCE   ON
    CONFIGURE_COMMAND autoreconf -fi
    COMMAND
      ${CMAKE_COMMAND} -E env "CC=${CMAKE_C_COMPILER}"
      ${CMAKE_COMMAND} -E env "CXX=${CMAKE_CXX_COMPILER}"
      ${CMAKE_COMMAND} -E env "PKG_CONFIG_PATH=${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/lib64/pkgconfig"
      ./configure --prefix=${CMAKE_CURRENT_SOURCE_DIR}/thirdparty --without-zlib --with-openssl --disable-shared
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
  ExternalProject_Add_StepDependencies(curl build openssl)
  ExternalProject_Add_StepDependencies(urlapp build curl)
endif()
