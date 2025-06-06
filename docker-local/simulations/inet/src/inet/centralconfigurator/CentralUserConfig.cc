/*
 * CentralUserConfig.cc
 *
 *  Created on: May 5, 2025
 *      Author: root
 */

#include "../centralconfigurator/CentralUserConfig.h"

#include "inet/applications/base/ApplicationPacket_m.h"
#include "inet/applications/udpapp/SimpleUdpSinkApp.h"
namespace inet{

Define_Module(CentralUserConfig);


void CentralUserConfig::initialize()
{
    cnc = check_and_cast<CentralNetworkConfig *>((this->getParentModule())->getSubmodule("cnc"));

}

void CentralUserConfig::handleMessage(cMessage *msg)
{
    if(msg->getKind() == MSGKIND_SEND_TA){
        cPacket *pkt = check_and_cast<cPacket *>(msg);
        StreamRegistrationRequest retrievedReq = *(check_and_cast<StreamRegistrationRequest *>(pkt->getObject(0)));

        //add moduleids to talker & listener
        retrievedReq.talker_id = cnc->findModuleIdFor(retrievedReq.talker_str.c_str());
        retrievedReq.listener_id = cnc->findModuleIdFor(retrievedReq.listener_str.c_str());

        cout<<"# CUC => Received a stream request for: "<<endl;
        retrievedReq.printRequest();
        batchOfRequests.push_back(retrievedReq);
        if(batchOfRequests.size()== sizeOfBatch){
            cnc->registerStreams(batchOfRequests);
            batchOfRequests.clear();
        }
        delete msg;
    }
}

void CentralUserConfig::informTalkers(std::vector<StreamStatus> talkerUpdates){

    for(auto s: talkerUpdates){
        cPacket *packet = new cPacket("talker-update");
        packet->setKind(MSGKIND_STREAM_RESPONSE);

        packet->addPar("sid");
        packet->par("sid").setStringValue((s.sid).c_str());

        packet->addPar("status");
        packet->par("status").setBoolValue(s.status);

        packet->addPar("offset");
        packet->par("offset").setDoubleValue(s.talkerOffset);

        cModule * targetDev =  this->getParentModule()->getParentModule()->getSubmodule(s.talker_str.c_str());
        cModule * appModule = targetDev->getSubmodule("app", 0)->getSubmodule("source");

        sendDirect(packet,  appModule->gate("fromCuc"));
        cout<<"# CUC::informTalker => Stream "<< s.sid<<" is "<<s.status<<" with offset:"<<s.talkerOffset<<endl;
    }

}
}



