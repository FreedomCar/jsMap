#include "ros/ros.h"
#include "beginner_tutorials/AddTwoInts.h"   //在devel/include目录中，由AddTwoInts.srv文件自动生成的
#include "style.h"
#include "example.cpp"
#include <list>
bool find_way(beginner_tutorials::AddTwoInts::Request  &req,
	beginner_tutorials::AddTwoInts::Response &res)
{

	res.result = plan(req.a, req.b, req.c, req.d);
	//res.sum = req.a + req.b;
	//ROS_INFO("request: x=%ld, y=%ld", (long int)req.a, (long int)req.b);
	//ROS_INFO("sending back response: [%ld]", (long int)res.sum);
	return true;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "find_way_server");
	ros::NodeHandle n;

	ros::ServiceServer service = n.advertiseService("find_way", find_way);
	//一旦检测到有find_way服务，就调用find_way函数
	ROS_INFO("Ready to find_way.");
	ros::spin();

	return 0;
}