/*
 * StreamRegistrationResponse.h
 *
 *  Created on: May 14, 2025
 *      Author: root
 */

#ifndef INET_CENTRALCONFIGURATOR_STREAMREGISTRATIONRESPONSE_H_
#define INET_CENTRALCONFIGURATOR_STREAMREGISTRATIONRESPONSE_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <iterator>     // std::advance
#include <sstream>
#include <iostream>

using namespace omnetpp;

struct StreamRegistrationResponse {
    string sid;
    float data;
    std::vector<int> path;

    StreamRegistrationResponse() { sid = ""; data = 0.0;}

    StreamRegistrationResponse(string s, float d, std::vector<int> p) { sid = s; data = d; path = p; }

    bool operator==(StreamRegistrationResponse d) {
        if(sid == d.sid &&  data == d.data && path==d.path)
            return true;
        return false;
    }
};

struct StreamStatus{
    string sid;
    string talker_str;
    bool status;
    double talkerOffset;

    StreamStatus(string id, string tstr, bool s,  double t) { sid = id; talker_str=tstr; status = s; talkerOffset = t; }
};


#endif /* INET_CENTRALCONFIGURATOR_STREAMREGISTRATIONRESPONSE_H_ */
