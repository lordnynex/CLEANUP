# ----------------------------------------------------------------------------
#  CMake file for luacv
# ----------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8)
set(TARGET luacv)
project(${TARGET})

set(LUACV_VERSION "0.2.0")
string(REGEX MATCHALL "[0-9]" LUACV_VERSION_PARTS "${LUACV_VERSION}")
list(GET LUACV_VERSION_PARTS 0 LUACV_VERSION_MAJOR)
list(GET LUACV_VERSION_PARTS 1 LUACV_VERSION_MINOR)
list(GET LUACV_VERSION_PARTS 2 LUACV_VERSION_SUBMINOR)

file(GLOB LIB_SRCS "objects/*.cpp*" "*.cpp*")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
find_package(Lua51 REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/objects"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${LUA_INCLUDE_DIR}"
    )

link_directories(
   "${CMAKE_CURRENT_SOURCE_DIR}/objects"
   "${CMAKE_CURRENT_SOURCE_DIR}"
   "${LUA_LIBRARIES}"
   "${OpenCV_LIB_DIR}"
   )

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -O3 -fpic -Werror -Wextra -pedantic-errors -lopencv_core -lopencv_features2d")
  add_definitions(-D LUACV_VERSION_MAJOR=${LUACV_VERSION_MAJOR} -D LUACV_VERSION_MINOR=${LUACV_VERSION_MINOR} -D LUACV_VERSION_SUBMINOR=${LUACV_VERSION_SUBMINOR} )

  if(${CMAKE_SYSTEM_PROCESSOR} MATCHES x86_64*)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long")
  endif()
  if(${CMAKE_SYSTEM_PROCESSOR} MATCHES amd64*)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long")
  endif()

  if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lm")
  endif()

elseif(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _SCL_SECURE_NO_WARNINGS /Gy /Zi")
  add_definitions(
    -DLUACV_VERSION_MAJOR=${LUACV_VERSION_MAJOR}
    -DLUACV_VERSION_MINOR=${LUACV_VERSION_MINOR}
    -DLUACV_VERSION_SUBMINOR=${LUACV_VERSION_SUBMINOR}
    -D__VERSION__="MS Visual C++"
    )

  if(MSVC80)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wp64")
  endif()
  if(MSVC90)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D _BIND_TO_CURRENT_CRT_VERSION=1 /D _BIND_TO_CURRENT_VCLIBS_VERSION=1")
  endif()

  set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} /LTCG /GL")
  string(REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  string(REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
  string(REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()

add_library(${TARGET} SHARED ${LIB_SRCS})

set_target_properties(${TARGET} PROPERTIES
  SOVERSION ${LUACV_VERSION}
  OUTPUT_NAME "${TARGET}"
  PREFIX ""
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  LINKER_LANGUAGE CXX
  )
if(WIN32)
  set_target_properties(${TARGET} PROPERTIES SUFFIX ".dll")
else()
  set_target_properties(${TARGET} PROPERTIES SUFFIX ".so")
endif()

target_link_libraries(${TARGET} ${LUA_LIBRARIES} ${OpenCV_LIBS})
