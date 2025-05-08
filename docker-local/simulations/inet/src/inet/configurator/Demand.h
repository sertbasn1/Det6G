/*
 * Demand.h
 *
 *  Created on: May 7, 2025
 *      Author: root
 */

#ifndef INET_CONFIGURATOR_DEMAND_H_
#define INET_CONFIGURATOR_DEMAND_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <iterator>     // std::advance
#include <sstream>
#include <iostream>

using namespace omnetpp;


struct Demand {
    int index;

    int talker;
    int listener;

    int service_type; // 0-7 service priority class

    float max_data; // maximum data rate to reserve links
    float max_latency; // maximum tolerable latency

    Demand() {index = -1; talker = 0; listener = 0; service_type = 0; max_data = 0.0; max_latency = 0.0;}

    Demand(int i, int t, int l, int s, float d, float lt) { index = i; talker = t; listener = l; service_type = s; max_data = d; max_latency = lt;}

    Demand& operator=(const Demand& d) { index = d.index; talker = d.talker; listener = d.listener; service_type = d.service_type; max_data = d.max_data; max_latency = d.max_data; return *this;}

    Demand(const Demand &d) { index = d.index; talker = d.talker; listener = d.listener; service_type = d.service_type; max_data = d.max_data; max_latency = d.max_data; }
};



#endif /* INET_CONFIGURATOR_DEMAND_H_ */
