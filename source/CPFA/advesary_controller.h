#ifndef ADVESARY_CONTROLLER_H
#define ADVESARY_CONTROLLER_H

// Include libs
#include <source/Base/BaseController.h>
#include <source/Base/Pheromone.h>
#include <source/CPFA/CPFA_loop_functions.h>
/* Definition of the LEDs actuator */
#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>

using namespace std;
using namespace argos;

class CPFA_loop_functions;

class advesary_controller : public BaseController
{
    public:

        advesary_controller();

        // CCI_controller inheritance functions
        void Init(argos::TConfigurationNode &node); // used to read settings from XML file
        void ControlStep();
        void Reset();

        void SetLoopFunctions(CPFA_loop_functions* lf);

    private:
    
        string controllerID;
        // simulation loop functions
        CPFA_loop_functions* LoopFunctions; 
        argos::CRandom::CRNG* RNG;
        CCI_LEDsActuator* m_pcLEDs; // setting robot different color

        // how many times do we inject a false phermone
        int InjectionRate;
        // attractiveness of each pheromone
        size_t FakeDensity;
        // how fast the phermone trail fades
        Real FakeDecayRate;
        // track number of ticks between injections
        int tickCounter = 0;

        void InjectFalsePheromone();

};

#endif