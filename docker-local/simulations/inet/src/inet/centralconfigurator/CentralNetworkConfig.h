/*
 * CentralNetworkConfig.h
 *
 *  Created on: May 7, 2025
 *      Author: root
 */

#ifndef INET_CENTRALCONFIGURATOR_CENTRALNETWORKCONFIG_H_
#define INET_CENTRALCONFIGURATOR_CENTRALNETWORKCONFIG_H_
#include <omnetpp.h>
#include "inet/queueing/source/ActivePacketSource.h"
#include "inet/networklayer/configurator/base/NetworkConfiguratorBase.h"
#include "inet/networklayer/common/InterfaceTable.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/common/ModuleAccess.h"
#include "inet/networklayer/ipv4/IIpv4RoutingTable.h"

#include <map>
#include "../centralconfigurator/StreamRegistrationRequest.h"
#include "../centralconfigurator/StreamRegistrationResponse.h"
#include "inet/linklayer/configurator/gatescheduling/common/wTSNGateScheduleConfigurator.h"



using namespace omnetpp;
using namespace std;

namespace inet{

class INET_API CentralNetworkConfig :  public NetworkConfiguratorBase
{
    public:
        int registerStreams(std::vector<StreamRegistrationRequest> requests);
        int findModuleIdFor(const char * devName);
        //void configureGate(cModule *gate,  bool initiallyOpen, simtime_t offset, vector<simtime_t> times);

    protected:
        virtual void initialize() override;
        void printTopology();
        int getTopoIndex(int moduleid);
        void printIndexMapping(); //topo index-moduleid mapping
        std::map<int, int> topoIndices; //topo index-moduleid


};
}


#endif /* INET_CENTRALCONFIGURATOR_CENTRALNETWORKCONFIG_H_ */
