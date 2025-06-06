/*
 * CentralNetworkConfig.cc
 *
 *  Created on: May 5, 2025
 *      Author: root
 */
#include "../centralconfigurator/CentralNetworkConfig.h"

#include "../centralconfigurator/CentralUserConfig.h"
namespace inet{

Define_Module(CentralNetworkConfig);

void CentralNetworkConfig::initialize()
{
    topology = new Topology();
    extractTopology(*topology);
    printTopology();
    printIndexMapping();
}


void CentralNetworkConfig::printTopology(){
    cout<<"======Extracted _topology======="<<endl;
    cout<<topology->getNumNodes()<<" nodes has been detected"<< endl;
    for (int i = 0; i < topology->getNumNodes(); ++i) {
        const Topology::Node *node = topology->getNode(i);
        int numOfOut = node->getNumOutLinks();

        cout << "Node("<< node->getModule()->getId() << ")"<< node->getModule()->getFullName() << " with "<< numOfOut << " outgoing links." << endl;

        for (int i=0; i<numOfOut; i++) {
            Topology::Link * link = node->getLinkOut(i);

            cout << "  --> connects to Node("<< link->getLinkOutRemoteNode()->getModule()->getId() << ")" << link->getLinkOutRemoteNode()->getModule()->getFullName()
                       << " via gate: " << link->getLinkOutLocalGate()->getFullName() << endl;
        }

        topoIndices[i] = node->getModule()->getId();
    }
    cout<<"====== ====== ====== ======="<<endl;

    //Test find module by id
//    cModule *mod = this->getSimulation()->getModule(11);
//    if (mod)
//        cout << "Found module: " << mod->getFullPath() << endl; //simpleTsn.device2
//    else
//        cout << "Module not found!" << endl;
//

}

void CentralNetworkConfig::printIndexMapping() {
    cout<<"TopoIndex\tModuleId"<<endl;
    for (const auto& pair : topoIndices) {
            cout << pair.first << "\t" << pair.second << "\t"<< this->getSimulation()->getModule(pair.second)->getFullPath() << endl;
        }

}

int CentralNetworkConfig::findModuleIdFor(const char * devName) {
    for (const auto& pair : topoIndices) {
             if (strcmp(this->getSimulation()->getModule(pair.second)->getFullName(),devName)==0){
                 return this->getSimulation()->getModule(pair.second)->getId();
             }
        }
    return -1;
}


int CentralNetworkConfig::getTopoIndex(int moduleid) {
    for(int i=0;i<topology->getNumNodes();i++){
        if(moduleid==topology->getNode(i)->getModuleId()){
            return i;
        }
    }
    return -1;
}

int CentralNetworkConfig::registerStreams(std::vector<StreamRegistrationRequest> requests){
    cout<<"# CNC::registerStreams is trying to embed following stream(s): ";

    for(auto r:requests)
        cout<< r.sid << "\t";
    cout<<endl;

   // OPT Call
    wTSNGateScheduleConfigurator * gateConfModule = check_and_cast<wTSNGateScheduleConfigurator *>((this->getParentModule())->getSubmodule("gateScheduleConfigurator"));
    std::vector<StreamStatus> talkerUpdates = gateConfModule->triggerGateScheduling(requests);

    cout<<"TON"<<endl;


    if(talkerUpdates.size()){
        CentralUserConfig *cuc = check_and_cast<CentralUserConfig *>((this->cSimpleModule::getParentModule())->getSubmodule("cuc"));
        cuc->informTalkers(talkerUpdates);
    }

    return 0;
}


//void CentralNetworkConfig::configureGate(cModule *gate,  bool initiallyOpen, simtime_t offset, vector<simtime_t> times){
//    int gateIndex = gate->getIndex();
//    gate->par("initiallyOpen") = initiallyOpen;
//    gate->par("offset") = offset.dbl();
//
//    cValueArray *durations = new cValueArray();
//
//    for(simtime_t t:times){
//        durations->add(cValue(t.dbl(), "s"));
//    }
//    cPar& durationsPar = gate->par("durations");
//    durationsPar.copyIfShared();
//    durationsPar.setObjectValue(durations);
//
//}

}
