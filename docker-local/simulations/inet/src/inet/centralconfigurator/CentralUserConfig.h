/*
 * CentralUserConfig.h
 *
 *  Created on: May 7, 2025
 *      Author: root
 */

#ifndef INET_CENTRALCONFIGURATOR_CENTRALUSERCONFIG_H_
#define INET_CENTRALCONFIGURATOR_CENTRALUSERCONFIG_H_

#include <omnetpp.h>
#include "inet/queueing/source/ActivePacketSource.h"
#include <iostream>
#include <vector>
#include "../centralconfigurator/CentralNetworkConfig.h"
#include "../centralconfigurator/StreamRegistrationRequest.h"

#define MSGKIND_STREAM_RESPONSE         10

using namespace omnetpp;
using namespace std;

namespace inet{

class CentralUserConfig : public cSimpleModule {
    public:
        CentralNetworkConfig *cnc;
        int numOfDemands = 0;
        void informTalkers(std::vector<StreamStatus> talkerUpdates);

    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        std::vector<StreamRegistrationRequest> batchOfRequests;
        int sizeOfBatch=1;
};
}



#endif /* INET_CENTRALCONFIGURATOR_CENTRALUSERCONFIG_H_ */
