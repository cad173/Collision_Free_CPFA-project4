#ifndef CPFA_LOOP_FUNCTIONS_H
#define CPFA_LOOP_FUNCTIONS_H

#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/core/simulator/entity/floor_entity.h>
#include <source/CPFA/CPFA_controller.h>
#include <argos3/plugins/simulator/entities/cylinder_entity.h>
#include <set>

using namespace argos;
using namespace std;

static const size_t GENOME_SIZE = 7; // There are 7 parameters to evolve

class CPFA_loop_functions : public argos::CLoopFunctions
{

	friend class CPFA_controller;
	friend class CPFA_qt_user_functions;

	public:

		CPFA_loop_functions();
	   
		void Init(argos::TConfigurationNode &t_tree);
		void Reset();
		void PreStep();
		void PostStep();
		bool IsExperimentFinished();
		void PostExperiment();
		argos::CColor GetFloorColor(const argos::CVector2 &c_pos_on_floor);

		// GA Functions
		
		/* Configures the robot controller from the genome */
		void ConfigureFromGenome(Real* pf_genome);
		/* Calculates the performance of the robot in a trial */
		Real Score();
	
		/**
		 * Returns the current trial.
		 */
		UInt32 GetTrial() const;
	
		/**
		 * Sets the current trial.
		 * @param un_trial The trial number.
		 */
		void SetTrial(UInt32 un_trial);
	
		/* public helper functions */
		void UpdatePheromoneList();
		void SetFoodDistribution();

		argos::Real getSimTimeInSeconds();

		std::vector<argos::CColor>   TargetRayColorList;

		unsigned int getNumberOfRobots();
    void increaseNumDistributedFoodByOne();
		double getProbabilityOfSwitchingToSearching();
		double getProbabilityOfReturningToNest();
		double getUninformedSearchVariation();
		double getRateOfInformedSearchDecay();
		double getRateOfSiteFidelity();
		double getRateOfLayingPheromone();
		double getRateOfPheromoneDecay();

		argos::CRange<argos::Real> getForageRangeX() { return ForageRangeX; }
		argos::CRange<argos::Real> getForageRangeY() { return ForageRangeY; }
		std::vector<Pheromone>& getPheromoneList()   { return PheromoneList; }
		argos::CVector2 getNestPosition()            { return NestPosition; }


		void setScore(double s);

		/* ── Robot reputation ─────────────────────────────────────────────────
		 * Each robot accumulates a reputation based on pheromone outcomes.
		 * badReports  = times a robot returned empty after following this robot's pheromone
		 * totalReports = good + bad returns from this robot's pheromones
		 *
		 * A robot is flagged as adversary only when BOTH conditions hold:
		 *   totalReports >= ROBOT_MIN_REPORTS (enough evidence)
		 *   badReports / totalReports > ROBOT_BAD_RATIO (consistently bad)
		 *
		 * This lets friendly robots survive occasional honest mistakes (their
		 * ratio stays low) while adversaries, who only place fake trails, quickly
		 * accumulate a near-1.0 bad ratio and get permanently ignored.
		 * ──────────────────────────────────────────────────────────────────── */
		/* ── Robot reputation ─────────────────────────────────────────────────
		 * Two signals are tracked per robot:
		 *
		 *  suspiciousPheromones  — distinct pheromones that crossed the IsSuspicious()
		 *                          threshold (majority of visitors returned empty).
		 *                          This is the primary flagging signal because it
		 *                          ignores robots that accidentally found real food
		 *                          while travelling to a fake pheromone location.
		 *
		 *  emptyReturns/totalReturns — raw return stats kept for display only.
		 *
		 * A robot is flagged as adversary only when:
		 *   suspiciousPheromones >= ROBOT_SUSPICIOUS_THRESHOLD
		 *
		 * Friendly robots with accidents rarely accumulate 2+ fully-suspicious
		 * pheromones.  Adversaries, who place nothing but fakes, quickly do.
		 * ──────────────────────────────────────────────────────────────────── */
		struct RobotReputation {
			size_t suspiciousPheromones = 0; // distinct pheromones that became suspicious
			size_t emptyReturns         = 0; // display only
			size_t totalReturns         = 0; // display only
			bool   flagged              = false;
		};

		static const size_t ROBOT_SUSPICIOUS_THRESHOLD = 2;

		/* Call when a robot returns from a pheromone (foundEmpty = returned without food).
		 * newlySuspicious = pheromone just crossed IsSuspicious() for the first time. */
		void RecordPheromoneReturn(const std::string& robotID,
		                           bool foundEmpty,
		                           size_t newlySuspicious);
		bool IsRobotFlagged(const std::string& robotID) const;

		std::map<std::string, RobotReputation> robotReputation;
		/* ground truth: populated at Init from ACPFA controller type */
		std::set<std::string> adversaryRobots;

		argos::CRandom::CRNG* RNG;
                size_t NumDistributedFood;
		size_t MaxSimTime;
		size_t ResourceDensityDelay;
		size_t RandomSeed;
		size_t SimCounter;
		size_t MaxSimCounter;
		size_t VariableFoodPlacement;
		size_t OutputData;
		size_t DrawDensityRate;
		size_t DrawIDs;
		size_t DrawTrails;
		size_t DrawTargetRays;
		size_t FoodDistribution;
		size_t FoodItemCount;
		size_t PowerlawFoodUnitCount;
		size_t NumberOfClusters;
		size_t ClusterWidthX;
		size_t ClusterWidthY;
		size_t PowerRank;
                size_t ArenaWidth;
                size_t SimTime; 
                Real curr_time_in_minutes; 
                Real last_time_in_minutes; 
  
		/* CPFA variables */
		argos::Real ProbabilityOfSwitchingToSearching;
		argos::Real ProbabilityOfReturningToNest;
		argos::CRadians UninformedSearchVariation;
		argos::Real RateOfInformedSearchDecay;
		argos::Real RateOfSiteFidelity;
		argos::Real RateOfLayingPheromone;
		argos::Real RateOfPheromoneDecay;

		/* physical robot & world variables */
		argos::Real FoodRadius;
		argos::Real FoodRadiusSquared;
		argos::Real NestRadius;
		argos::Real NestRadiusSquared;
		argos::Real NestElevation;
		argos::Real SearchRadiusSquared;

		/* list variables for food & pheromones */
		std::vector<argos::CVector2> FoodList;
		std::vector<argos::CColor>   FoodColoringList;
		vector<argos::CVector2> CollectedFoodList;
                map<string, argos::CVector2> FidelityList; 
		std::vector<Pheromone>   PheromoneList; 
		std::vector<argos::CRay3>    TargetRayList;
		argos::CRange<argos::Real>   ForageRangeX;
		argos::CRange<argos::Real>   ForageRangeY;
  
                Real   CollisionTime;
                size_t currCollisionTime; 
                size_t lastCollisionTime; 
                size_t lastNumCollectedFood;
                size_t currNumCollectedFood;
                size_t Num_robots;
      
                vector<size_t>		ForageList;
		argos::CVector2 NestPosition;
	private:

		/* private helper functions */
		void RandomFoodDistribution();
		void ClusterFoodDistribution();
		void PowerLawFoodDistribution();
    bool IsOutOfBounds(argos::CVector2 p, size_t length, size_t width);
		bool IsCollidingWithNest(argos::CVector2 p);
		bool IsCollidingWithFood(argos::CVector2 p);
		double score;
		int PrintFinalScore;

		/* voting robustness counters — updated by PostStep() */
		size_t totalRejections;           // pheromones voted out (fake ones caught)
		size_t totalFalsePositiveRejections; // real pheromones wrongly voted out
		size_t ticksToFirstRejection;     // 0 = no rejection occurred
};

#endif /* CPFA_LOOP_FUNCTIONS_H */
