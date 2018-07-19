/*
 * 直接调用plan方法，参数（开始经度，开始纬度，结束经度，结束纬度
 * 返回值为一个list；list中元素为需要经过的点，list中元素为point的形式，其中有lon（经度）和lat（纬度）的属性
 * 调用示例可见main
 */


#include <iostream>
#include <netdb.h>
#include <list>
#include <vector>

using namespace std;

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


int main() {
    list<point> r = plan("130.481028", "39.989643", "130.465302", "40.004717");

    list<point>::iterator itor;  //定义迭代器


    //从list第一个iterator开始
    itor = r.begin();
    while (itor != r.end()) {
        cout << (*itor++).lon << ",";
        cout << (*itor++).lat << endl;
    }
    return 0;
}

vector<string> split(const string &s, const string &separator) {
    vector<string> result;
    typedef string::size_type string_size;
    string_size i = 0;

    while (i != s.size()) {
        //找到字符串中首个不等于分隔符的字母；
        int flag = 0;
        while (i != s.size() && flag == 0) {
            flag = 1;
            for (string_size x = 0; x < separator.size(); ++x)
                if (s[i] == separator[x]) {
                    ++i;
                    flag = 0;
                    break;
                }
        }

        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        string_size j = i;
        while (j != s.size() && flag == 0) {
            for (string_size x = 0; x < separator.size(); ++x)
                if (s[j] == separator[x]) {
                    flag = 1;
                    break;
                }
            if (flag == 0)
                ++j;
        }
        if (i != j) {
            result.push_back(s.substr(i, j - i));
            i = j;
        }
    }
    return result;
}


void parseHostAndPagePath(const string &url, string &hostUrl, string &pagePath) {
    hostUrl = url;
    pagePath = "/";
    auto pos = static_cast<int>(hostUrl.find("http://"));
    if (-1 != pos)
        hostUrl = hostUrl.replace(static_cast<unsigned long>(pos), 7, "");
    pos = static_cast<int>(hostUrl.find("https://"));
    if (-1 != pos)
        hostUrl = hostUrl.replace(static_cast<unsigned long>(pos), 8, "");
    pos = static_cast<int>(hostUrl.find("/"));
    if (-1 != pos) {
        pagePath = hostUrl.substr(static_cast<unsigned long>(pos));
        hostUrl = hostUrl.substr(0, static_cast<unsigned long>(pos));
    }
}

string getPageContent(const string &url) {
    struct hostent *host;
    string hostUrl, pagePath;
    parseHostAndPagePath(url, hostUrl, pagePath);
    if (nullptr == (host = gethostbyname(hostUrl.c_str()))) {
        cout << "gethostbyname error\n" << endl;
        exit(1);
    }

    struct sockaddr_in pin{};
    int port = 80;
    bzero(&pin, sizeof(pin));
    pin.sin_family = AF_INET;
    pin.sin_port = htons(port);
    pin.sin_addr.s_addr = ((struct in_addr *) (host->h_addr))->s_addr;
    int iSock;
    if ((iSock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        cout << "open socket error\n" << endl;
        exit(1);
    }

    string requestHeader;
    requestHeader = "GET " + pagePath + " HTTP/1.1\r\n";
    requestHeader += "Host: " + hostUrl + "\r\n";
    requestHeader += "Accept: */*\r\n";
    requestHeader += "User-Agent: Mozilla/4.0(compatible)\r\n";
    requestHeader += "connection:Keep-Alive\r\n";
    requestHeader += "\r\n";

    if (connect(iSock, (const sockaddr *) &pin, sizeof(pin)) == -1) {
        cout << "connect error\n" << endl;
        exit(1);
    }
    if (send(iSock, requestHeader.c_str(), requestHeader.size(), 0) == -1) {
        cout << "send error\n" << endl;
        exit(1);
    }

    struct timeval timeout = {1, 0};
    setsockopt(iSock, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(struct timeval));
    char c;
    bool flag = true;
    while (recv(iSock, &c, 1, 0) > 0) {
        if ('\r' == c) {
            continue;
        } else if ('\n' == c) {
            if (!flag)
                break;
            flag = false;
        } else {
            flag = true;
        }
    }

    int len, BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    string pageContent;
    while ((len = static_cast<int>(recv(iSock, buffer, static_cast<size_t>(BUFFER_SIZE - 1), 0))) > 0) {
        buffer[len] = '\0';
        pageContent += buffer;
    }

    return pageContent;
}


list<point> plan(const string &fromLon, const string &fromLat, string toLon, string toLat) {
    list<point> lst;
    string result =
            "https://restapi.amap.com/v3/direction/driving?origin=" + fromLon + "," + fromLat + "&destination=" +
            toLon + "," + toLat + "&extensions=all&output=xml&key=9e312862ef2950b0474dbd7a62797047";
    string r = getPageContent(result);
    int pos1 = 0;
    int pos2 = 0;
    auto end = static_cast<int>(r.find("</response>"));
    pos1 = static_cast<int>(r.find("</duration><polyline>"));
    pos2 = static_cast<int>(r.find("</polyline><action>"));
    while (pos1 != (-1)) {

        string temp = r.substr(static_cast<unsigned long>(pos1 + 21), static_cast<unsigned long>(pos2 - pos1 - 21));
        vector<string> pointVector = split(temp, ";");

        for (const auto &i : pointVector) {

            vector<string> num = split(i, ",");
            double lon = stof(num[0]);
            double lat = stof(num[1]);
            point tempPoint;
            tempPoint.lon = lon;
            tempPoint.lat = lat;
            lst.push_back(tempPoint);
        }
        r = r.substr(static_cast<unsigned long>(pos2 + 19), static_cast<unsigned long>(end + pos2));
        pos1 = static_cast<int>(r.find("</duration><polyline>"));
        pos2 = static_cast<int>(r.find("</polyline><action>"));
    }
    return lst;
}