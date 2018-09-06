#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include "EasyCurl.h"
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <math.h>

#include <list>
#include "conversion.h"

#include <json/json.h>

using namespace std;

struct point {
    double lon;
    double lat;
    double angle;
};

string convertDoubleToString(double value){
    string result;
    stringstream ss;
    ss<<value;
    ss>>result;
    return result;
}

std::list<point> route_plan(string fromlon, string fromlat, string tolon, string tolat) {
    // The temp response with lat and lon
    std::vector<point> re;
    // The real response with x and y
    std::list<point> result;

    Json::Value resp;

    string str;
    string error;
    string url = "http://127.0.0.1:5000/route/v1/driving/" + fromlon + "," + fromlat + ";" + tolon + "," + tolat + "?steps=true";
    libcurl_get(url.c_str(),str,error);
    cout<<str<<endl;

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(str.c_str(), root)) {
        cout << "bad json string";
    }

    if (root["code"].asString() == "Ok") {
        Json::Value &routes = root["routes"];
        Json::Value &legs = routes[(unsigned int) 0]["legs"];

        Json::Value leg = legs[(unsigned int) 0];
        Json::Value &steps = leg["steps"];

        int len = steps.size();

        for (unsigned int i = 0; i < len; i++) {
            Json::Value me = steps[i]["maneuver"];
            Json::Value location = me["location"];

            point tempPoint;

            tempPoint.angle = me["bearing_after"].asInt() - me["bearing_before"].asInt();
            tempPoint.lon = location[(unsigned int) 0].asDouble();
            tempPoint.lat = location[(unsigned int) 1].asDouble();
            re.push_back(tempPoint);
        }

        for (int i = 0; i < re.size() - 1; i++) {
            point translatedPointStart;
            conversion::UTM(re[i].lat, re[i].lon, &translatedPointStart.lat, &translatedPointStart.lon);
            point translatedPointEnd;
            conversion::UTM(re[i + 1].lat, re[i + 1].lon, &translatedPointEnd.lat, &translatedPointEnd.lon);
            //calculate the distance of two point and then use the int(distance) as the equal part num
            double x_distance = translatedPointEnd.lat - translatedPointStart.lat;
            double y_distance = translatedPointEnd.lon - translatedPointStart.lon;
            int distance = std::round(std::sqrt(x_distance * x_distance + y_distance * y_distance));
            double x_delta = x_distance / distance;
            double y_delta = y_distance / distance;

            for (int j = 0; j < distance; j++) {
                point translatedPoint;
                translatedPoint.angle = translatedPointStart.angle;
                translatedPoint.lat = translatedPointStart.lat + x_delta * j;
                translatedPoint.lon = translatedPointStart.lon + y_delta * j;
                result.push_back(translatedPoint);
            }
        }
        //add the last point
        point translatedPoint;
        translatedPoint.angle = re[re.size() - 1].angle;
        translatedPoint.lat = re[re.size() - 1].lat;
        translatedPoint.lon = re[re.size() - 1].lon;
        result.push_back(translatedPoint);
        return result;
    } else {
        // error
        string code = root["code"].asString();
        string message = root["message"].asString();

        std::cout << "Code: " << code << "\n";
        std::cout << "Message: " << code << "\n";

        return result;
    }
}

int main(){
    std::list<point> r = route_plan("13.388860", "52.517037", "13.385983", "52.496891");
    
    std::list<point>::iterator itor; //定义迭代器
    
    //从list第一个iterator开始
    itor = r.begin();
    while (itor != r.end())
    {
        std::cout << (*itor++).lon << ",";
        std::cout << (*itor++).lat << std::endl;
    }
    std::cout<<r.size();
    return 0;
}
