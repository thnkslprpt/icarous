#include "Cognition.hpp"

/**
 * - A nominal path has id "Plan0"
 * - A resolution path has id "Plani" where i > 0. e.g. Plan1,Plan2,... 
 * - A ditch path has id "DitchPath"
 * - Although a ditching path is also a resolution, it is treated differently due to integration with safe2ditch.
 * - After executing a resolution plan, transition back to nominal path (to the next feasible waypoint)
 * - A feasible waypoint is a waypoint that is reachable.
 * - There can be multiple resolution paths during the course of a mission.
 * - The Event Manager framework that is being used only enables attaching a unique handler to a given trigger.
 * - However, we can define multiple triggers for the same conflict if we want to attach different handlers to the same conflict.
 *   you may want to do this if you want different behaviors (that can be configured by the user) for a given conflict. 
 */


/** 
 * - Trigger takeoff based on mission start
 * - Ignore takeoff command if in a fence violation
 * - Trigger takeoff only after departure time
 */
bool TakeoffTrigger(CognitionState_t* state){
     
    return state->missionStart == 0 && 
           !state->keepInConflict &&
           !state->keepOutConflict &&
           state->utcTime >= state->scenarioTime;
}

/**
 * - Trigger to transition out of a successful takeoff
 * - Can be used to activate the nominal mission plan.
 */
bool NominalDepartureTrigger(CognitionState_t* state){
    
    return state->missionStart > 0;
}

/**
 * - Check for fence violations
 * - Ignore violations if projected point of violation is not on flightplan.
 * - Only trigger if violation is imminent within the threshold.
 */
bool FenceConflictTrigger(CognitionState_t* state){
    
    return state->planProjectedFenceConflict && state->timeToFenceViolation < state->parameters.planLookaheadTime &&
           !state->trafficConflict && state->icReady;
}

/**
 * - Check for completion of resolution plan.
 */
bool SecondaryPlanCompletionTrigger(CognitionState_t* state){
    
    if(state->activePlan != nullptr){
        std::string planID = state->activePlan->getID();
        return (planID != "Plan0" &&
                planID != "DitchPath" &&
                state->nextWpId[planID] >= state->activePlan->size()) && state->icReady;
    }else{
        return false;
    }
}

/**
 * - Check for completion of nominal path
 */
bool PrimaryPlanCompletionTrigger(CognitionState_t* state){
    
    if(state->activePlan != nullptr){
        std::string planID = state->activePlan->getID();
        return (planID == "Plan0" && state->nextWpId[planID] >= state->activePlan->size());
    } else {
        return false;
    }
}

/**
 * Check for flight plan deviations greater than the defined threshold
 */
bool FlightPlanDeviationTrigger(CognitionState_t* state){
    if(state->activePlan == nullptr){
        return false;
    }

    if(state->activePlan->getID() == "Plan0"){
        if(state->nextWpId["Plan0"] >= state->activePlan->size()){
            return false;
        }
    }
    state->XtrackConflict = (state->xtrackDeviation > state->parameters.allowedXtrackDeviation) &&
                            !state->trafficConflict && state->icReady;
    return state->XtrackConflict;
}

/**
 * - Detect a well clear violation (detected by daidalus)
 * - Note that this trigger is ignored if the search resolution is requested (by a user).
 * - A search resolution is an alternative way to deal with imminent well clear violations.
 */
bool TrafficConflictVectorResTrigger(CognitionState_t* state){
    bool conflict = false;

    if(state->parameters.resolutionType == SEARCH_RESOLUTION){
        return false;
    }
    conflict = conflict || state->allTrafficConflicts[SPEED_RESOLUTION];
    conflict = conflict || state->allTrafficConflicts[TRACK_RESOLUTION];
    conflict = conflict || state->allTrafficConflicts[ALTITUDE_RESOLUTION];
    conflict = conflict || state->allTrafficConflicts[VERTICALSPEED_RESOLUTION];

    // TODO: Introduce a parameter to enforce checks to make sure if the
    // projected conflict is on the current flightplan. Current, the usage
    // of reutrn2NextWP is a hack (replace with proper parameter)
    if(state->parameters.verifyPlanConflict){
        conflict = conflict && state->planProjectedTrafficConflict;
    }

    state->trafficConflict = conflict && state->icReady;
    return state->trafficConflict;
}

/**
 * Detect a well clear violation (detected by daidalus)
 * This trigger is ignore if a search resolution is not requested (by a user)
 */
bool TrafficConflictPathResTrigger(CognitionState_t* state){
    bool conflict = false;
    if(state->parameters.resolutionType != SEARCH_RESOLUTION){
        return false;
    }
    conflict = conflict || state->allTrafficConflicts[SPEED_RESOLUTION];
    conflict = conflict || state->allTrafficConflicts[TRACK_RESOLUTION];
    conflict = conflict || state->allTrafficConflicts[ALTITUDE_RESOLUTION];
    conflict = conflict || state->allTrafficConflicts[VERTICALSPEED_RESOLUTION];
    conflict = conflict && state->planProjectedTrafficConflict;

    state->trafficConflict = conflict;
    // Check for conflicts that are imminent due to the current flightplan
    // These are conflicts that DAIDALUS hasn't detected yet.
    if(!conflict && state->planProjectedTrafficConflict){
       if(state->timeToTrafficViolation3 < state->parameters.planLookaheadTime){ 
          state->trafficConflict = true; 
       }
    }
    return state->trafficConflict && state->icReady;

}

/**
 * Trigger a ditching action because of a traffic conflict. 
 * This was introduced to support Safe2ditch. 
 * Since the other traffic triggers will also be activated, it is important to assign
 * the right priority for this event.
 */
bool TrafficConflictDitchTrigger(CognitionState_t* state){
    return state->trafficConflict && state->parameters.resolutionType == DITCH_RESOLUTION;
}

/**
 * Detect a merging in process.
 * This is to ensure we don't perform traffic resolutions while doing a merge
 */
bool MergingActivityTrigger(CognitionState_t* state){
    return state->mergingActive == 1 && state->icReady;
}

/**
 * Start the ditching process (due to an external ditch request)
 */
bool DitchingTrigger(CognitionState_t* state){
    return state->ditch && state->icReady;
}

/**
 * Monitor the arrival of the TOD point of the ditch path. To start post TOD actions.
 */
bool DitchSiteTODTrigger(CognitionState_t* state){
    if(state->activePlan != nullptr){
        std::string planID = state->activePlan->getID();
        return (planID == "DitchPath" &&
                state->nextWpId[planID] >= state->activePlan->size()) && state->icReady;
    }else{
        return false;
    }
}