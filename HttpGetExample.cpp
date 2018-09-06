#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include <cstdlib>
#include "EasyCurl.h"

#include <list>
#include "conversion.h"

struct point
{
    double lon;
    double lat;
};


int main()
{
    std::string response;
    std::string error;
    libcurl_get("http://localhost:3000/account",response,error);
    std::cout<<response<<std::endl;
    std::cout<<error<<std::endl;
    return 0;
}
