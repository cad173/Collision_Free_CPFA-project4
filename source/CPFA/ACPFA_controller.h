#ifndef ACPFA_CONTROLLER_H
#define ACPFA_CONTROLLER_H

#include "CPFA_controller.h"

class CPFA_loop_functions;

class ACPFA_controller : public CPFA_controller
{
public:
  ACPFA_controller();

  void Init(argos::TConfigurationNode &node) override;

protected:

  void ControlStep() override;

  void FakePheromoneTrail();

  // Adversary should not follow pheromone trails, so we override this function to do nothing
  bool SetTargetPheromone() override;

  // Fidelity should not be used by the adversary as we do not want it to return to the same location, so we override these functions to do nothing
  void SetFidelityList() override;

  // Adversary will return whether they have food or not and place false pheromones
  void Returning() override;


private:


double fakePheromoneStrength; // set strength of fake pheromones
int fakePheromoneDecay; // set decay time of fake pheromones (in control steps)
int fakePheromoneCounter = 0;
int fakePheromoneTimer; // sets time between fake pheromone placements

};
#endif