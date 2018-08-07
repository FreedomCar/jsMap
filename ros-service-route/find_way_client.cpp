#include "ros/ros.h"
#include "beginner_tutorials/AddTwoInts.h"
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

#include <list>
int main(int argc, char **argv)
{
	ros::init(argc, argv, "find_way_client");
	if (argc != 3)
	{
		ROS_INFO("usage: find_way_client X Y");
		return 1;
	}

	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<beginner_tutorials::find_way>("find_way");
	//����һ��find_way����
	beginner_tutorials::find_way srv;
	srv.request.a = atof(argv[1]);
	srv.request.b = atof(argv[2]);
	srv.request.c = atof(argv[3]);
	srv.request.d = atof(argv[4]);
	//��charת��Ϊfloat
	if (client.call(srv))
	{    
		// ����õĽ�� ������������ԭ����ͬ�������result��һ��float���龭γ����������  result=srv.response.result
		//ROS_INFO("Sum: %ld", (long int)srv.response.sum);
		//std::list<point> result0 = (std::list<point>)srv.response.result;


	}
	else
	{
		ROS_ERROR("Failed to call service find_way");
		return 1;
	}
	return 0;
}