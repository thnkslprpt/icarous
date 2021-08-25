#include "EventHandler.hpp"
#include "Cognition.hpp"

#define MAKE_HANDLER(NAME) std::make_shared<NAME>()

class EngageNominalPlan: public EventHandler<CognitionState_t>{
    retVal_e Initialize(CognitionState_t* state){
        if(state->missionStart > 0){
            state->nextWpId[state->missionPlan] = state->missionStart;
            state->missionStart = -1;
            state->icReady = true;
        }
        return SUCCESS;
    }

    retVal_e Execute(CognitionState_t* state){
       LogMessage(state,"[HANDLER] | Engage nominal plan");
       SetGuidanceFlightPlan(state,state->missionPlan,state->nextWpId[state->missionPlan]);
       return SUCCESS;
    }
};

class TakeoffPhaseHandler: public EventHandler<CognitionState_t>{
   retVal_e Initialize(CognitionState_t* state){
       TakeoffCommand takeoff_command;
       Command cmd = {.commandType=CommandType_e::TAKEOFF_COMMAND};
       cmd.takeoffCommand = takeoff_command;
       state->cognitionCommands.push_back(cmd);
       state->missionStart = -1;
       return SUCCESS;
   }

   retVal_e Terminate(CognitionState_t* state){
       if(state->takeoffComplete == 1){
           state->missionStart = 1;
           //SetGuidanceFlightPlan(state,(char*)state->missionPlan.c_str(),state->nextWpId[state->missionPlan]);
           return SUCCESS;
       }else if(state->takeoffComplete != 0){
           LogMessage(state,"[WARNING] | Takeoff failed");
           return SUCCESS;
       }
       return INPROGRESS;
   }
};

class Vector2Mission: public EventHandler<CognitionState_t>{
   larcfm::Position target;
   double gs;
   retVal_e Initialize(CognitionState_t* state){
       LogMessage(state, "[STATUS] | " + eventName + " | Vectoring to mission");
       if(state->parameters.return2NextWP == 3){
           larcfm::Plan* fp = GetPlan(&state->flightPlans,state->missionPlan);
           if(state->missionPlan == "Plan0"){
               state->nextWpId[state->missionPlan] =state->nextFeasibleWpId;
           }
           target = fp->getPos(state->nextWpId[state->missionPlan]);
           state->nextWpId[state->missionPlan] += 1;
           gs = state->velocity.gs();
       }else{
           target = state->clstPoint;
           larcfm::Plan* fp = GetPlan(&state->flightPlans,state->missionPlan);
           int nextWP = state->nextWpId[state->missionPlan];
           GetNearestPositionOnPlan(fp, state->position, nextWP);
           state->nextWpId[state->missionPlan] =nextWP;
       }
       return SUCCESS;
   }

   retVal_e Execute(CognitionState_t* state){
       
       double trkRef = state->position.track(target) * 180/M_PI;
       double vs = 0.1*(target.alt() - state->position.alt());
       double dist = state->position.distanceH(target);
       double trkCurrent = state->velocity.track("degree");
       double trkCmd;
       double turnRate = 0.1;
       int direction = 1;
       double diff = fmod(360 + fabs(trkCurrent - trkRef),360);
       if(diff >= 45){
           if (state->rightTurnConflict) {
               trkCmd = trkCurrent -2;
           }else if(state->leftTurnConflict){
               trkCmd = trkCurrent + 2;
           }else{
               trkCmd = trkRef;
           }

       }else{
           trkCmd = trkRef;
       }
       
       // speed reduction when near target to facilitate capture
       if (dist < 200) {
          gs = fmin(gs, dist * 0.25);
       }
       SetGuidanceVelCmd(state,trkCmd,gs,vs);
       if ( dist < fmax(10,2.5*gs)){
           return SUCCESS;
       }else{
           return INPROGRESS;
       }
   }

   retVal_e Terminate(CognitionState_t* state){
       ExecuteHandler(MAKE_HANDLER(EngageNominalPlan),"");
       return SUCCESS;
   }
};

class ReturnToMission: public EventHandler<CognitionState_t>{
   retVal_e Initialize(CognitionState_t* state){
        if(PrimaryPlanCompletionTrigger(state)){
            return SHUTDOWN;
        }
        LogMessage(state, "[STATUS] | " + eventName + " | Return to mission");
        state->numSecPaths++;
        std::string pathName = "Plan" + std::to_string(state->numSecPaths);
        larcfm::Plan* fp = GetPlan(&state->flightPlans,state->missionPlan);

        larcfm::Position positionA,positionB;
        larcfm::Velocity velocityA,velocityB;
        velocityA = state->velocity;
        positionA = state->position;
        if(state->parameters.return2NextWP == 0){
            int wpID = state->nextWpId[fp->getID()];
            double gs  = fp->gsIn(wpID);
            double trk = fp->trkIn(wpID)*180/M_PI;
            double vs  = fp->vsIn(wpID);
            positionB = state->clstPoint;
            velocityB = larcfm::Velocity::makeTrkGsVs(trk,"degree",gs,"m/s",vs,"m/s");
        }else{
            if(state->missionPlan == "Plan0"){
                state->nextWpId[state->missionPlan] = state->nextFeasibleWpId;
            }
            int index = state->nextWpId[state->missionPlan];
            positionB = GetNextWP(fp,index);
            velocityB = GetNextWPVelocity(fp,index);
            if(state->activePlan->getID() == state->missionPlan){
                state->nextWpId[state->missionPlan] += 1;
            }
        }
        FindNewPath(state,pathName,positionA, velocityA, positionB, velocityB);
        SendStatus(state,(char*)"IC:Computing secondary path",6);
        state->request = REQUEST_PROCESSING;
        return SUCCESS;
   }

   retVal_e Execute(CognitionState_t* state){
        if(state->request == REQUEST_RESPONDED){
            state->request = REQUEST_NIL;
            std::string pathName = "Plan" + std::to_string(state->numSecPaths);
            SetGuidanceFlightPlan(state,pathName,1);
           return SUCCESS;
        }else{
            std::string pathName = "Plan" + std::to_string(state->numSecPaths);
            larcfm::Plan *fp = GetPlan(&state->flightPlans,pathName);
            if(fp != nullptr){
                state->request = REQUEST_RESPONDED;
            } 
           return INPROGRESS;
        }
   }
}; 

class ReturnToNextFeasibleWP:public EventHandler<CognitionState_t>{
    retVal_e Initialize(CognitionState_t* state){
        if(PrimaryPlanCompletionTrigger(state)){
            return SHUTDOWN;
        }

        if(state->missionPlan == "Plan0"){
            state->nextWpId[state->missionPlan] = state->nextFeasibleWpId;
        }
        int index = state->nextWpId[state->missionPlan];

        LogMessage(state, "[STATUS] | " + eventName + " | Return to next feasible waypoint");
        state->numSecPaths++;
        std::string pathName = "Plan" + std::to_string(state->numSecPaths);
        larcfm::Position positionB;
        larcfm::Plan* fp = GetPlan(&state->flightPlans,state->missionPlan);
        positionB = GetNextWP(fp,index);

        larcfm::Position positionA = state->position;
        larcfm::Velocity velocityA = state->velocity;
        larcfm::Velocity velocityB = GetNextWPVelocity(fp,index);
        FindNewPath(state,pathName,positionA, velocityA, positionB, velocityB);
        SendStatus(state,(char*)"IC:Computing secondary path",6);
        state->nextWpId[state->missionPlan] += 1;
        state->request = REQUEST_PROCESSING;
        return SUCCESS;
    }

    retVal_e Execute(CognitionState_t* state){
        if(state->request == REQUEST_RESPONDED){
            state->request = REQUEST_NIL;
            std::string pathName = "Plan" + std::to_string(state->numSecPaths);
            SetGuidanceFlightPlan(state,pathName,1);
           return SUCCESS;
        }else{
            std::string pathName = "Plan" + std::to_string(state->numSecPaths);
            larcfm::Plan *fp = GetPlan(&state->flightPlans,pathName);
            if(fp != nullptr){
                state->request = REQUEST_RESPONDED;
            } 
           return INPROGRESS;
        }
    }
};

class LandPhaseHandler: public EventHandler<CognitionState_t>{
   retVal_e Execute(CognitionState_t* state){
       SendStatus(state, (char *)"IC: Landing", 6);
       LandCommand land_command;
       Command cmd = {.commandType = CommandType_e::LAND_COMMAND};
       cmd.landCommand = land_command;
       state->cognitionCommands.push_back(cmd);
       state->missionStart = -2;
       state->activePlan = nullptr;
       LogMessage(state,"[FLIGHT_PHASES] | LANDING -> IDLE");
       return SUCCESS;
   }
};

class TrafficConflictHandler: public EventHandler<CognitionState_t>{
 public:
    double startTime;

    retVal_e Initialize(CognitionState_t* state){
         state->resolutionStartSpeed = state->velocity.gs();
         state->prevResSpeed  = state->resolutionStartSpeed;
         state->prevResTrack  = state->hdg;
         state->prevResAlt    = larcfm::Units::to(larcfm::Units::m,state->position.alt());
         state->prevResVspeed = larcfm::Units::to(larcfm::Units::mps,state->velocity.vs());
         state->preferredSpeed = state->prevResSpeed;
         state->preferredTrack = state->prevResTrack;
         state->preferredAlt   = state->prevResAlt;
         state->trafficConflictStartTime = state->utcTime;
         state->resType = GetResolutionType(state);

         int ind = state->resType;
         if (state->validResolution[ind]){
             
             if (state->resType == SPEED_RESOLUTION) {
                 LogMessage(state, "[STATUS] | "+ eventName +" | Resolving traffic conflict with speed resolution");
             }
             else if (state->resType == ALTITUDE_RESOLUTION) {
                 LogMessage(state, "[STATUS] | "+ eventName +" | Resolving traffic conflict with altitude resolution");
             }
             else if (state->resType == TRACK_RESOLUTION) {
                 LogMessage(state, "[STATUS] | "+ eventName +" | Resolving traffic conflict with track resolution");
                 LogMessage(state, "[MODE] | Guidance Vector Request");
             }
             else if(state->resType == VERTICALSPEED_RESOLUTION ){
                 LogMessage(state, "[STATUS] | "+ eventName +" | Resolving traffic conflict with vertical speed resolution");
                 LogMessage(state, "[MODE] | Guidance Vector Request");
             }
         }else{
             return RESET;
         }

         return SUCCESS;
    }

    retVal_e Execute(CognitionState_t *state) {

        int resolutionType = state->resType;
      
        // If track, gs and vs are in recovery, don't allow combined resolution
        if (state->recovery[TRACK_RESOLUTION] && 
            state->recovery[SPEED_RESOLUTION] && 
            state->recovery[VERTICALSPEED_RESOLUTION]) {
            if (resolutionType == TRACK_SPEED_VS_RESOLUTION) {
                resolutionType = TRACK_RESOLUTION;
            }
        }
        if(state->missionPlan == "Plan0"){
            state->nextWpId[state->missionPlan] = state->nextFeasibleWpId;
        }
        int index = state->nextWpId[state->missionPlan];
        larcfm::Position target = GetPlan(&state->flightPlans,state->missionPlan)->getPos(index);

        switch (resolutionType) {

        case SPEED_RESOLUTION: {
            if (fabs(state->preferredSpeed - state->prevResSpeed) >= 0.1) {
                SetGuidanceSpeedCmd(state,state->activePlan->getID(), state->preferredSpeed);
                state->prevResSpeed = state->preferredSpeed;
            }
            state->returnSafe = ComputeTargetFeasibility(state,target);
            break;
        }

        case ALTITUDE_RESOLUTION:
        {
            double climb_rate = 0.0;
            double diff;

            double current_alt = state->position.alt();
            double alt_pref = state->preferredAlt;
            if(state->todAltitude > 0){
                alt_pref = state->todAltitude;
            }
            bool newTargetAlt = fabs(alt_pref - state->prevResAlt) > 1e-3;
            bool prevTargetReached = fabs(current_alt - state->prevResAlt) < 10;
            /* 
             * Implement an altitude resolution with the following criteria:
             * Check if the previous target altitude has already been reached 
             * before implementing a new updated resolution.
             */
            if ((newTargetAlt && prevTargetReached) || state->newAltConflict)
            {
                state->newAltConflict = false;
                SetGuidanceAltCmd(state,state->activePlan->getID(), alt_pref, 1);
                state->prevResAlt = alt_pref;
            }

            state->returnSafe = ComputeTargetFeasibility(state,target);

            break;
        }

        case TRACK_RESOLUTION:
        {
            double speed = state->resolutionStartSpeed;
            double climb_rate = 0;
            SetGuidanceVelCmd(state,state->preferredTrack, speed, climb_rate);
            state->prevResTrack = state->preferredTrack;
            state->returnSafe = ComputeTargetFeasibility(state,target);
            if(state->parameters.return2NextWP == 0 || state->parameters.return2NextWP == 2){
                state->returnSafe &= ComputeTargetFeasibility(state,state->clstPoint);
                state->closestPointFeasible = state->returnSafe;
            }
            break;
        }

        case VERTICALSPEED_RESOLUTION:
        {
            double speed = state->resolutionStartSpeed;
            double res_up = state->resVUp;
            double res_down = state->resVDown;
            // If there is a valid up resolution, execute up resolution.
            // else execute the down resolution. If 0 vertical speed is possible,
            // that is preferred over the up or down resolutions.
            if (!std::isinf(res_up) && !std::isnan(res_up))
            {
                if (res_up >= 1e-3)
                {
                    SetGuidanceVelCmd(state,state->hdg, speed, -res_up);
                    state->prevResVspeed = res_up;
                }
                else
                {
                    SetGuidanceVelCmd(state,state->hdg, speed, 0.0);
                    state->prevResVspeed = 0.0;
                }
            }
            else if (!std::isinf(res_down) && !std::isnan(res_down))
            {
                if (res_down <= -1e-3)
                {
                    SetGuidanceVelCmd(state,state->hdg, speed, -res_down);
                    state->prevResVspeed = res_down;
                }
                else
                {
                    SetGuidanceVelCmd(state,state->hdg, speed, 0.0);
                    state->prevResVspeed = 0.0;
                }
            }
            else
            {
                SetGuidanceVelCmd(state,state->hdg, speed, state->prevResVspeed);
            }

            uint8_t val;
            if (state->vsBandsNum > 0)
            {
                val = 0;
            }
            else
            {
                val = 1;
            }

            state->returnSafe = (bool)val;
            break;
        }

        case TRACK_SPEED_VS_RESOLUTION:
        {
            double outTrack = state->preferredTrack;
            double outSpeed = state->preferredSpeed;
            double outVS = state->preferredVSpeed;
            if (state->recovery[TRACK_RESOLUTION])
            {
                outTrack = std::fmod(360 + state->velocity.trk() * 180 / M_PI, 360);
            }

            if (state->recovery[SPEED_RESOLUTION])
            {
                outSpeed = state->velocity.gs();
            }

            if (state->recovery[VERTICALSPEED_RESOLUTION])
            {
                outVS = state->velocity.vs();
            }

            SetGuidanceVelCmd(state,outTrack, outSpeed, outVS);
            state->returnSafe = ComputeTargetFeasibility(state,target);
        }

        default:
        {

            break;
        }
        }

        bool conflict = state->trafficConflict || (!state->returnSafe);
        if(conflict){
            return INPROGRESS;
        }else{
            startTime = state->utcTime;
            return SUCCESS;
        }
    }

    retVal_e Terminate(CognitionState_t* state){
        
        if(state->utcTime - startTime < state->parameters.persistenceTime){
            return INPROGRESS;
        }
        std::string planid = state->activePlan->getID();
        if(state->resType == SPEED_RESOLUTION){
            SetGuidanceSpeedCmd(state,state->activePlan->getID(),state->resolutionStartSpeed);
            SetGuidanceFlightPlan(state,planid,state->nextWpId[planid]);
            LogMessage(state,"[HANDLER] | Engage nominal plan");
        }else if(state->resType == ALTITUDE_RESOLUTION){
            double alt = state->activePlan->getPos(state->nextWpId[planid]).alt();
            if(fabs(state->prevResAlt - alt) > 1e-3){
                SetGuidanceAltCmd(state,planid,alt,1);
                state->prevResAlt = alt;
                SetGuidanceFlightPlan(state,planid,state->nextWpId[planid]);
                LogMessage(state,"[HANDLER] | Engage nominal plan");
            }
        }else{
            if(state->parameters.return2NextWP < 2){
                ExecuteHandler(MAKE_HANDLER(ReturnToMission),"PostTrafficConflict");
            }else{
                ExecuteHandler(MAKE_HANDLER(Vector2Mission),"PostTrafficConflict",priority-0.6);
            }
        }
        std::string message = " against ";
        for(auto id: state->conflictTraffics){
            if (id != ""){
                message += id + ", ";
            }
        }
        SendStatus(state,(char*)"IC:traffic conflict resolved",6);
        LogMessage(state,"[RESOLVED] | Traffic conflict resolved"+message);
        state->conflictTraffics.clear();
        return SUCCESS;
    }

};


class MergingHandler: public EventHandler<CognitionState_t>{
    bool mergingSpeedChange = false;
    retVal_e Execute(CognitionState_t* state){
        if(state->mergingActive == 2 || state->mergingActive == 0){
            return SUCCESS;
        }else{
            if(state->mergingActive == 3){
                mergingSpeedChange = true;
            }
            return INPROGRESS;
        }
    }

    retVal_e Terminate(CognitionState_t* state){
        if(mergingSpeedChange){
            SetGuidanceFlightPlan(state,state->missionPlan,state->nextWpId[state->missionPlan]);
        }
        LogMessage(state,"[FLIGHT_PHASES] | MERGING -> CRUISE");
        return SUCCESS;
    }

};

class RequestDitchSite: public EventHandler<CognitionState_t>{
    retVal_e Execute(CognitionState_t* state){
        SetDitchSiteRequestCmd(state);
        LogMessage(state,"[STATUS] | Sending Ditch Request Signal");
        return SUCCESS;
    }
};

class ProceedToDitchSite: public EventHandler<CognitionState_t>{
   
    retVal_e Initialize(CognitionState_t* state){
        // Find a path to TOD
        larcfm::Position positionA = state->position.mkAlt(state->todAltitude);
        double trk = std::fmod(2*M_PI + state->ditchSite.track(positionA),2*M_PI);
        larcfm::Position positionB = state->ditchSite.linearDist2D(trk,state->todAltitude).mkAlt(state->todAltitude);
        double trkGoal = positionA.track(positionB)*180/M_PI;
        larcfm::Velocity velocityA = state->velocity;
        larcfm::Velocity velocityB = larcfm::Velocity::makeTrkGsVs(trkGoal,"degree",3,"m/s",0,"m/s");
        FindNewPath(state,"DitchPath",positionA, velocityA, positionB, velocityB);
        SendStatus(state,(char*)"IC:Computing ditch path",6);
        state->request = REQUEST_PROCESSING;
        state->ditch = false;
        return SUCCESS;
    }

    retVal_e Execute(CognitionState_t* state){
        if(state->request == REQUEST_RESPONDED){
            state->request = REQUEST_NIL;
            SetGuidanceFlightPlan(state,"DitchPath",1);
            state->missionPlan = "DitchPath";
           return SUCCESS;
        }else{
            larcfm::Plan *fp = GetPlan(&state->flightPlans,"DitchPath");
            if(fp != nullptr){
                state->request = REQUEST_RESPONDED;
            } 
           return INPROGRESS;
        }
        
    }

};

class ProceedFromTODtoLand: public EventHandler<CognitionState_t>{
   retVal_e Initialize(CognitionState_t* state){
       LogMessage(state,"[STATUS] | Reached TOD, proceeding to land");
       SetGuidanceP2P(state,state->ditchSite,1.5);
       return SUCCESS;
   }

   retVal_e Execute(CognitionState_t* state){
       if(state->ditch){
           // if we receive the ditch signal again while tryin to land
           // assume current ditch site is infeasible.
           return SUCCESS; 
       }else if(state->p2pComplete){
           state->activePlan = nullptr;
           return SUCCESS;
       }else{
           return INPROGRESS;
       }
   }

   retVal_e Terminate(CognitionState_t* state){
       LogMessage(state,"[STATUS] | Execute land handler");
       ExecuteHandler(MAKE_HANDLER(LandPhaseHandler),"PostTODtoLand");
       return SUCCESS;
   }

};