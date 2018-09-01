#include <pluginlib/class_list_macros.h>
#include "global_planner.h"
#include "planner_algorithm.h"
#include <exception>


//register this planner as a BaseGlobalPlanner plugin
PLUGINLIB_EXPORT_CLASS(global_planner::GlobalPlanner, nav_core::BaseGlobalPlanner)

using namespace std;

//Default Constructor
namespace global_planner {

 GlobalPlanner::GlobalPlanner (){

 }

 bool GlobalPlanner::makePlan(const string &fromLon, const string &fromLat, const string &toLon, const string &toLat,list<point>& plan){
    try
	{
        plan=plan(fromLon,fromLat,toLon,toLat);
		return true;
	}
	catch (exception& e)
	{
		return false;
	}
 }
};