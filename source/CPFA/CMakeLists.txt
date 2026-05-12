cmake_minimum_required(VERSION 3.5)
project(CPFA_PROJECT)

find_package(ARGoS REQUIRED)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/source
)

link_directories(${CMAKE_BINARY_DIR})

###############################################
# Loop Functions
###############################################
add_library(CPFA_loop_functions SHARED
    CPFA_loop_functions.h
    CPFA_loop_functions.cpp
    CPFA_qt_user_functions.h
    CPFA_qt_user_functions.cpp
)

target_link_libraries(CPFA_loop_functions
    CPFA_controller
    argos3core_simulator
    argos3plugin_simulator_entities
    argos3plugin_simulator_footbot
    argos3plugin_simulator_genericrobot
    argos3plugin_simulator_qtopengl
    ${ARGOS_QTOPENGL_LIBRARIES}
)

###############################################
# Base CPFA Controller
###############################################
add_library(CPFA_controller SHARED
    CPFA_controller.h
    CPFA_controller.cpp
)

target_link_libraries(CPFA_controller
    BaseController
    Pheromone
    Nest
    argos3core_simulator
    argos3plugin_simulator_footbot
    argos3plugin_simulator_genericrobot
)

# CPFA_controller calls CPFA_loop_functions methods but can't link against it
# directly (circular dependency). ARGoS resolves both as runtime plugins, so
# we defer symbol resolution to load time on macOS.
if(APPLE)
    target_link_options(CPFA_controller PRIVATE -undefined dynamic_lookup)
endif()

###############################################
# Adversary Controller (FIXED)
###############################################
add_library(ACPFA_controller SHARED
    ACPFA_controller.h
    ACPFA_controller.cpp
)

target_link_libraries(ACPFA_controller
    CPFA_controller
    BaseController
    Pheromone
    Nest
    argos3core_simulator
    argos3plugin_simulator_footbot
    argos3plugin_simulator_genericrobot
)