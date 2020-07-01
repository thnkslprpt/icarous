/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.6
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */
#ifndef _TrajectoryPlanner_proxy_H_
#define _TrajectoryPlanner_proxy_H_

/* -----------------------------------------------------------------------------
 * cproxy.swg
 *
 * Definitions of C specific preprocessor symbols for proxies.
 * ----------------------------------------------------------------------------- */

#ifndef SWIGIMPORT
# ifndef __GNUC__
#   define __DLL_IMPORT __declspec(dllimport)
# else
#   define __DLL_IMPORT __attribute__((dllimport)) extern
# endif
# if !defined (__WIN32__)
#   define SWIGIMPORT extern
# else
#   define SWIGIMPORT __DLL_IMPORT
# endif
#endif

#include <stdio.h>

typedef struct {
  void *obj;
  const char **typenames;
} SwigObj;


// special value indicating any type of exception like 'catch(...)'
#define SWIG_AnyException "SWIG_AnyException"

#include <setjmp.h>

SWIGIMPORT jmp_buf SWIG_rt_env;

SWIGIMPORT struct SWIG_exc_struct {
  int code;
  char *msg;
  SwigObj *klass;
} SWIG_exc;

SWIGIMPORT void SWIG_rt_try(void);
SWIGIMPORT int SWIG_rt_catch(const char *type);
SWIGIMPORT void SWIG_rt_throw(SwigObj *klass, const char * msg);
SWIGIMPORT int SWIG_rt_unhandled(void);
SWIGIMPORT void SWIG_rt_endtry(void);
SWIGIMPORT int SWIG_exit(int code);

#define SWIG_try \
  SWIG_rt_try(); \
  if ((SWIG_exc.code = setjmp(SWIG_rt_env)) == 0) 
#define SWIG_catch(type) else if (SWIG_rt_catch(#type))
#define SWIG_throw(klass) SWIG_rt_throw((SwigObj *) klass, 0);
#define SWIG_throw_msg(klass, msg) SWIG_rt_throw((SwigObj *) klass, msg);
#define SWIG_endtry else SWIG_rt_unhandled(); SWIG_rt_endtry();



#include <stdarg.h>

#define SWIG_MAKE_DELETE(Name,Obj) void Name(Obj *op1, ...) {\
  Obj *obj;\
  va_list vl;\
  va_start(vl, op1);\
  do {\
    obj = va_arg(vl, Obj *);\
    delete_##Obj(obj);\
  } while (obj);\
  va_end(vl);\
}

#define _GRID_ 0
#define _ASTAR_ _GRID_ + 1
#define _RRT_ _ASTAR_ + 1
#define _SPLINES_ _RRT_ + 1

typedef SwigObj PathPlanner;

PathPlanner * new_PathPlanner(double carg1, double carg2);
#include <stdbool.h>
void PathPlanner_InitializeAstarParameters(SwigObj * carg1, bool carg2, double carg3, double carg4, double carg5, /*aaa*/ char * carg6);
void PathPlanner_InitializeRRTParameters(SwigObj * carg1, double carg2, int carg3, double carg4, int carg5, double carg6, /*aaa*/ char * carg7);
void PathPlanner_InitializeBSplinesParameters(SwigObj * carg1, bool carg2, double carg3, int carg4, int carg5);
void PathPlanner_UpdateAstarParameters(SwigObj * carg1, bool carg2, double carg3, double carg4, double carg5, /*aaa*/ char * carg6);
void PathPlanner_UpdateRRTParameters(SwigObj * carg1, double carg2, int carg3, double carg4, int carg5, double carg6, /*aaa*/ char * carg7);
void PathPlanner_UpdateDAAParameters(SwigObj * carg1, /*aaa*/ char * carg2);
int PathPlanner_FindPath(SwigObj * carg1, int carg2, /*aaa*/ char * carg3, /*aaa*/ double * carg4, /*aaa*/ double * carg5, /*aaa*/ double * carg6);
int PathPlanner_GetTotalWaypoints(SwigObj * carg1, /*aaa*/ char * carg2);
void PathPlanner_GetWaypoint(SwigObj * carg1, /*aaa*/ char * carg2, int carg3, /*aaa*/ double * carg4);
void PathPlanner_OutputFlightPlan(SwigObj * carg1, SwigObj * carg2, char * carg3, char * carg4, char * carg5);
void PathPlanner_ClearAllPlans(SwigObj * carg1);
void PathPlanner_InputGeofenceData(SwigObj * carg1, int carg2, int carg3, int carg4, double carg5, double carg6, double (* carg7)[2]);
/*aaaaaa*/SwigObj * PathPlanner_GetGeofence(SwigObj * carg1, int carg2);
void PathPlanner_ClearFences(SwigObj * carg1);
int PathPlanner_InputTraffic(SwigObj * carg1, int carg2, double * carg3, double * carg4);
void PathPlanner_InputFlightPlan(SwigObj * carg1, /*aaa*/ char * carg2, int carg3, /*aaa*/ double * carg4, double carg5);
double PathPlanner_Dist2Waypoint(SwigObj * carg1, /*aaa*/ double * carg2, /*aaa*/ double * carg3);
double PathPlanner_GetInterceptHeadingToPoint(SwigObj * carg1, /*aaa*/ double * carg2, /*aaa*/ double * carg3);
void PathPlanner_ManueverToIntercept(SwigObj * carg1, SwigObj * carg2, int carg3, /*aaa*/ double * carg4, /*aaa*/ double * carg5, double carg6, double carg7, double carg8);
void PathPlanner_ManueverToIntercept_c(SwigObj * carg1, char * carg2, int carg3, /*aaa*/ double * carg4, /*aaa*/ double * carg5, double carg6, double carg7, double carg8);
void PathPlanner_GetExitPoint(SwigObj * carg1, char * carg2, /*aaa*/ double * carg3, int carg4, /*aaa*/ double * carg5);
double PathPlanner_GetApproxElapsedPlanTime(SwigObj * carg1, SwigObj * carg2, /*aaa*/ double * carg3, int carg4);
void delete_PathPlanner(PathPlanner * carg1);
void PathPlanner_PlanToString(SwigObj * carg1,char* carg2,char* carg3,bool carg4,long int carg5);
void PathPlanner_StringToPlan(SwigObj * carg1,char* carg2,char* carg3);
void PathPlanner_CombinePlan(SwigObj * carg1,char* carg2,char* carg3,int carg4);


#endif /* _TrajectoryPlanner_proxy_H_ */
