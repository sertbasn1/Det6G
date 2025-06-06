/*
 * StreamRegistrationRequest.h
 *
 *  Created on: May 7, 2025
 *      Author: root
 */

#ifndef INET_CENTRALCONFIGURATOR_STREAMREGISTRATIONREQUEST_H_
#define INET_CENTRALCONFIGURATOR_STREAMREGISTRATIONREQUEST_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <iterator>     // std::advance
#include <sstream>
#include <iostream>

using namespace omnetpp;


class StreamRegistrationRequest : public cObject {
public:
    string sid;

    string talker_str;
    string listener_str;
    int talker_id; //module ids used by the controller
    int listener_id;

    int packetSize;
    int priority; // 0-7 service priority class
    float period ;
    float max_jitter;
    float max_latency; // maximum tolerable latency
    float gamma ;

    StreamRegistrationRequest() {
        sid = "";
        talker_str="";
        listener_str="";
        talker_id=-1;
        listener_id=-1;
        packetSize=0;
        priority = 0;
        period=0.0;
        max_jitter = 0.0;
        max_latency = 0.0;
        gamma=0.0;
    }

    StreamRegistrationRequest(string st, string tstr, string lstr, int tid, int lid, int ps, int s, float p, float d, float lt, float g) {
        sid = st;
        talker_str=tstr;
        listener_str=lstr;
        talker_id= tid;
        listener_id=lid;
        packetSize = ps;
        priority = s;
        period = p;
        max_jitter = d;
        max_latency = lt;
        gamma = g;
    }

    StreamRegistrationRequest& operator=(const StreamRegistrationRequest& d) {
        sid = d.sid;
        talker_str=d.talker_str;
        listener_str = d.listener_str;
        talker_id = d.talker_id;
        listener_id = d.listener_id;
        packetSize=d.packetSize;
        priority = d.priority;
        period = d.period;
        max_jitter = d.max_jitter;
        max_latency = d.max_latency;
        gamma = d.gamma;
        return *this;
    }

    StreamRegistrationRequest(const StreamRegistrationRequest &d) {
        sid = d.sid;
        talker_str=d.talker_str;
        listener_str = d.listener_str;
        talker_id = d.talker_id;
        listener_id = d.listener_id;
        packetSize = d.packetSize;
        priority = d.priority;
        period = d.period;
        max_jitter = d.max_jitter;
        max_latency = d.max_latency;
        gamma = d.gamma;
    }

    virtual StreamRegistrationRequest *dup() const override {
            return new StreamRegistrationRequest(*this);
        }

    void printRequest(){
        cout<<"\tStream "<<sid<<
                        " from "<<talker_str<<"("<<talker_id<<")" <<" to "<<listener_str<<" ("<<listener_id<<")" <<
                        "\t packet_size: "<<packetSize<<
                        "\t priority: "<< priority<<
                        "\t period: "<< period<<
                        "\t max_jitter: "<< max_jitter<<
                        "\t max_latency: "<< max_latency<<
                        "\t gamma: "<< gamma<<endl;

    }
};




#endif /* INET_CENTRALCONFIGURATOR_STREAMREGISTRATIONREQUEST_H_ */
