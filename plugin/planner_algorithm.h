struct point {
    double lon;
    double lat;
};


/**
 * 路径规划
 * @param fromLon 开始经度
 * @param fromLat 开始纬度
 * @param toLon 结束经度
 * @param toLat 结束纬度
 * @return list中元素为需要经过的点，list中元素为point的形式，其中有lon（经度）和lat（纬度）的属性
 */
list<point> plan(const string &fromLon, const string &fromLat, string toLon, string toLat);

vector<string> split(const string &s, const string &separator);

void parseHostAndPagePath(const string &url, string &hostUrl, string &pagePath);

string getPageContent(const string &url);