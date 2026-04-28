cmake_minimum_required(VERSION 3.5...3.28)
project(CPFA-ARGoS)

cmake_policy(SET CMP0167 OLD)

# Git metadata fallbacks
add_compile_definitions(
    GIT_BRANCH="unknown"
    GIT_COMMIT_HASH="unknown"
)

# Verbose output
set(CMAKE_VERBOSE_MAKEFILE ON)

# Default build type
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Choose build type: Debug Release RelWithDebInfo MinSizeRel" FORCE)
endif()

# macOS RPATH handling
if(APPLE)
  set(CMAKE_MACOSX_RPATH 0)
endif()

# Module paths
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_PREFIX_PATH $ENV{CMAKE_PREFIX_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ARGoS
find_package(ARGoS REQUIRED)
include_directories(${CMAKE_SOURCE_DIR} ${ARGOS_INCLUDE_DIRS})
link_directories(${ARGOS_LIBRARY_DIR})

# Optional dependencies (kept as-is, may or may not exist in your setup)
include_directories(
  ${CMAKE_SOURCE_DIR}
  ${ARGOS_INCLUDE_DIRS}
  ${GSL_INCLUDE_DIR}
  ${LUA_INCLUDE_DIR}
  source
)

link_directories(${ARGOS_LIBRARY_DIRS} build)

# Subprojects
add_subdirectory(source)

# ----------------------------
# Evolver (optional target)
# ----------------------------
if(BUILD_EVOLVER)

  # Boost (IMPORTANT: legacy FindBoost usage)
  find_package(Boost REQUIRED COMPONENTS filesystem)
  include_directories(${Boost_INCLUDE_DIRS})

  # Executable
  add_executable(cpfa_evolver source/evolver.cpp)

  # Linking
  target_link_libraries(cpfa_evolver
    CPFA_controller
    CPFA_loop_functions
    GA-MPI
    ${MPI_LIBRARIES}
    ${Boost_LIBRARIES}
    argos3core_simulator
  )

endif()