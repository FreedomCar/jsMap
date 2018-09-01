/** include the libraries you need in your planner here */
 /** for global path planner interface */
 #include <ros/ros.h>
 #include <nav_core/base_global_planner.h>
 #include <geometry_msgs/PoseStamped.h>
 #include <angles/angles.h>
 #include <base_local_planner/world_model.h>
 #include <base_local_planner/costmap_model.h>

 using std::string;

 #ifndef GLOBAL_PLANNER_CPP
 #define GLOBAL_PLANNER_CPP

 namespace global_planner {

 class GlobalPlanner : public nav_core::BaseGlobalPlanner {
 public:

  GlobalPlanner();

  bool makePlan(const string &fromLon, const string &fromLat, const string &toLon, const string &toLat,list<point>& plan);
  };
 };
 #endif