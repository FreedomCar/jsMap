#include <cmath>
#include <ros/ros.h>
#include <tf/transform_datatypes.h>

using namespace std;
tf::Quaternion transfer(double X, double Y, double Z)
{

    X = X * 180 / 3.1415;
    Y = Y * 180 / 3.1415;
    Z = Z * 180 / 3.1415;

    double x = sin(Y / 2) * sin(Z / 2) * cos(X / 2) + cos(Y / 2) * cos(Z / 2) * sin(X / 2);
    double y = sin(Y / 2) * cos(Z / 2) * cos(X / 2) + cos(Y / 2) * sin(Z / 2) * sin(X / 2);
    double z = cos(Y / 2) * sin(Z / 2) * cos(X / 2) - sin(Y / 2) * cos(Z / 2) * sin(X / 2);
    double w = cos(Y / 2) * cos(Z / 2) * cos(X / 2) - sin(Y / 2) * sin(Z / 2) * sin(X / 2);

    return tf::Quaternion(x, y, z, w);
  }
