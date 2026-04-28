add_library(Adversary_controller SHARED
    Adversary_controller.cpp
)

target_link_libraries(Adversary_controller
    BaseController
    argos3core_simulator
    argos3plugin_simulator_entities
    argos3plugin_simulator_footbot
)