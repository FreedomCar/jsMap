#include "osrm/match_parameters.hpp"
#include "osrm/nearest_parameters.hpp"
#include "osrm/route_parameters.hpp"
#include "osrm/table_parameters.hpp"
#include "osrm/trip_parameters.hpp"

#include "osrm/coordinate.hpp"
#include "osrm/engine_config.hpp"
#include "osrm/json_container.hpp"

#include "osrm/osrm.hpp"
#include "osrm/status.hpp"

#include "conversion.h"

#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include <cstdlib>

#include <list>
#include <vector>

struct point
{
    double lon;
    double lat;
    double angle;
};

std::list<point> plan(double fromlon, double fromlat, double tolon, double tolat)
{
    using namespace osrm;
    
    // Configure based on a .osrm base path, and no datasets in shared mem from osrm-datastore
    EngineConfig config;
    
    config.storage_config = {"test/data/monaco.osrm"};
    config.use_shared_memory = false;
    
    // We support two routing speed up techniques:
    // - Contraction Hierarchies (CH): requires extract+contract pre-processing
    // - Multi-Level Dijkstra (MLD): requires extract+partition+customize pre-processing
    //
    // config.algorithm = EngineConfig::Algorithm::CH;
    config.algorithm = EngineConfig::Algorithm::MLD;
    
    // Routing machine with several services (such as Route, Table, Nearest, Trip, Match)
    const OSRM osrm{config};
    
    // The following shows how to use the Route service; configure this service
    RouteParameters params;
    
    // Route in monaco
    params.coordinates.push_back({util::FloatLongitude{fromlon}, util::FloatLatitude{fromlat}});
    params.coordinates.push_back({util::FloatLongitude{tolon}, util::FloatLatitude{tolat}});
    
    // Response is in JSON format
    json::Object jsonResult;
    
    // Execute routing request, this does the heavy lifting
    const auto status = osrm.Route(params, jsonResult);
    
    // The temp response with lat and lon
    std::vector<point> re;
    // The real response with x and y
    std::list<point> result;
    if (status == Status::Ok)
    {
        /**auto &waypionts = result.values["waypoints"].get<json::Array>();
         int num = waypionts.values.size();
         for(int i = 0; i < num; i++){
         auto &point = waypionts.values.at[i].get<json::Object>();
         auto &loc = point.values["location"].get<json::Array>();
         
         point tempPoint;
         tempPoint.lon = loc.values.at(0).get<json::Number>().value;
         tempPoint.lat = loc.values.at(1).get<json::Number>().value;
         re.push_back(tempPoint);
         }**/
        
        auto &routes = jsonResult.values["routes"].get<json::Array>();
        
        // Let's just use the first route
        auto &route = routes.values.at(0).get<json::Object>();
        auto &legs = route.values["legs"].get<json::Array>();
        auto &r1 = legs.values.at(0).get<json::Object>();
        auto &step = r1.values["steps"].get<json::Array>();
        
        // get the steps and record it
        int num = step.values.size();
        for (int i = 0; i < num; i++)
        {
            auto &si = step.values.at(i).get<json::Object>();
            auto &mean = si.values["maneuver"].get<json::Object>();
            auto &loc = mean.values["location"].get<json::Array>();
            
            point tempPoint;
            tempPoint.angle = mean.values["bearing_after"].get<json::Number>().value -
            mean.values["bearing_before"].get<json::Number>().value;
            tempPoint.lon = loc.values.at(0).get<json::Number>().value;
            tempPoint.lat = loc.values.at(1).get<json::Number>().value;
            re.push_back(tempPoint);
        }
        
        for (int i = 0; i < re.size() - 1; i++)
        {
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
            
            for (int j = 0; j < distance; j++)
            {
                point translatedPoint;
                translatedPoint.angle = translatedPointStart.angle;
                translatedPoint.lat=translatedPointStart.lat+x_delta*j;
                translatedPoint.lon=translatedPointStart.lon+y_delta*j;
                result.push_back(translatedPoint);
            }
        }
        //add the last point
        point translatedPoint;
        translatedPoint.angle = re[re.size()-1].angle;
        translatedPoint.lat=re[re.size()-1].lat;
        translatedPoint.lon=re[re.size()-1].lon;
        result.push_back(translatedPoint);
        return result;
    }
    else if (status == Status::Error)
    {
        // error
        const auto code = jsonResult.values["code"].get<json::String>().value;
        const auto message = jsonResult.values["message"].get<json::String>().value;
        
        std::cout << "Code: " << code << "\n";
        std::cout << "Message: " << code << "\n";
        
        return result;
    }
}

int main()
{
    std::list<point> r = plan(116.481028, 39.989643, 116.465302, 40.004717);
    
    std::list<point>::iterator itor; //定义迭代器
    
    //从list第一个iterator开始
    itor = r.begin();
    while (itor != r.end())
    {
        std::cout << (*itor++).lon << ",";
        std::cout << (*itor++).lat << std::endl;
    }
    return 0;
    
}
