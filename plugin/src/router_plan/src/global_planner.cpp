#include <pluginlib/class_list_macros.h>
#include "router.cpp"
#include "global_planner.h"
#include "trans.h"
#include <exception>
#include <string>
#include <strstream>


//register this planner as a BaseGlobalPlanner plugin
PLUGINLIB_EXPORT_CLASS(global_planner::GlobalPlanner, nav_core::BaseGlobalPlanner)

using namespace std;

//Default Constructor
namespace global_planner {

    GlobalPlanner::GlobalPlanner() {

    }

    GlobalPlanner::GlobalPlanner(std::string name, costmap_2d::Costmap2DROS *costmap_ros) {
        initialize(name, costmap_ros);
    }

    void GlobalPlanner::initialize(std::string name, costmap_2d::Costmap2DROS *costmap_ros) {

    }

    bool GlobalPlanner::makePlan(const geometry_msgs::PoseStamped &start, const geometry_msgs::PoseStamped &goal, std::vector<geometry_msgs::PoseStamped> &plan) {
	try{
	    plan.push_back(start);

	    strstream ss;
            string str_start_x;
            string str_start_y;
            string str_goal_x;
            string str_goal_y;
	    ss << start.pose.position.x;
	    ss >> str_start_x;
	    ss.clear();
            ss << start.pose.position.y;
	    ss >> str_start_y;
	    ss.clear();
            ss << goal.pose.position.x;
	    ss >> str_goal_x;
	    ss.clear();
            ss << goal.pose.position.y;
	    ss >> str_goal_y;
	    ss.clear();
            //char str_start_x[256];
            //char str_start_y[256];
            //char str_goal_x[256];
            //char str_goal_y[256];
            //sprintf(str_start_x,"%lf",start.pose.position.x);
            //sprintf(str_start_y,"%lf",start.pose.position.y);
            //sprintf(str_goal_x,"%lf",goal.pose.position.x);
            //sprintf(str_goal_y,"%lf",goal.pose.position.y);
            list<point> temp_plan = route_plan(str_start_x, str_start_y, str_goal_x, str_goal_y);
	    //route_plan(str_start_x, str_start_y, str_goal_x, str_goal_y);


            for (list<point>::iterator i = temp_plan.begin(); i != temp_plan.end(); i++) {
                geometry_msgs::PoseStamped new_goal = goal;
                new_goal.pose.position.x = (*i).lon;
                new_goal.pose.position.y = (*i).lat;
		tf::Quaternion four = transfer(0, (*i).angle, 0);
		new_goal.pose.orientation.x = four.getX();
		new_goal.pose.orientation.y = four.getY();
		new_goal.pose.orientation.z = four.getZ();
		new_goal.pose.orientation.w = four.getW();
                plan.push_back(new_goal);
            }

            plan.push_back(goal);
	    return true;
	}
	catch (exception& e)
	{
	    return false;
	}
    }
}

