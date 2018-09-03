#include <iostream>
#include <netdb.h>
#include <list>
#include <vector>

using namespace std;

struct point {
    double lon;
    double lat;
    double angle;
};


/**
 * 路径规划
 * @param fromLon 开始经度
 * @param fromLat 开始纬度
 * @param toLon 结束经度
 * @param toLat 结束纬度
 * @return list中元素为需要经过的点，list中元素为point的形式，其中有lon（经度）和lat（纬度）的属性
 */
list<point> route_plan(const string &fromLon, const string &fromLat, const string &toLon, const string &toLat);

vector<string> split(const string &s, const string &separator);

void parseHostAndPagePath(const string &url, string &hostUrl, string &pagePath);

string getPageContent(const string &url);
