#include "ros/ros.h"
#include "beginner_tutorials/AddTwoInts.h"   //��devel/includeĿ¼�У���AddTwoInts.srv�ļ��Զ����ɵ�
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
	//һ����⵽��find_way���񣬾͵���find_way����
	ROS_INFO("Ready to find_way.");
	ros::spin();

	return 0;
}