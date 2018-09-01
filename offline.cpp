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

#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include <cstdlib>

#include <list>

struct point
{
    double lon;
    double lat;
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
    json::Object result;
    
    // Execute routing request, this does the heavy lifting
    const auto status = osrm.Route(params, result);
    
    std::list<point> re;
    if (status == Status::Ok)
    {
        auto &routes = result.values["routes"].get<json::Array>();
        
        // Let's just use the first route
        auto &route = routes.values.at(0).get<json::Object>();
        auto &legs = route.values["legs"].get<json::Array>();
        auto &r1 = legs.values.at(0).get<json::Object>();
        auto &step = r1.values["steps"].get<json::Array>();
        
        // get the steps and record it
        int num = routes.values.size();
        for (int i = 0; i < num; i++)
        {
            auto &si = step.values.at(i).get<json::Object>();
            auto &mean = si.values["maneuver"].get<json::Object>();
            auto &loc = mean.values["location"].get<json::Array>();
            
            point tempPoint;
            tempPoint.lon = loc.values.at(0).get<json::Number>().value;
            tempPoint.lat = loc.values.at(1).get<json::Number>().value;
            
            re.push_back(tempPoint);
        }
        
        return re;
    }
    else if (status == Status::Error)
    {
        // error
        const auto code = result.values["code"].get<json::String>().value;
        const auto message = result.values["message"].get<json::String>().value;
        
        std::cout << "Code: " << code << "\n";
        std::cout << "Message: " << code << "\n";
        
        return re;
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
