/*
 * wTSNGateScheduleConfigurator.cc
 *
 *  Created on: May 20, 2025
 *      Author: root
 */


#include "inet/linklayer/configurator/gatescheduling/common/wTSNGateScheduleConfigurator.h"

namespace inet {

Define_Module(wTSNGateScheduleConfigurator);

void wTSNGateScheduleConfigurator::initialize(int stage)
{
    topology = new Topology();
       extractTopology(*topology);
    if (stage == INITSTAGE_LOCAL) {
        gateCycleDuration = par("gateCycleDuration");
        configuration = check_and_cast<cValueArray *>(par("configuration").objectValue());
    }

}


void wTSNGateScheduleConfigurator::addSwitches(Input& input) const
{
    for (int i = 0; i < topology->getNumNodes(); i++) {
        auto node = (Node *)topology->getNode(i);
        if (!isDeviceNode(node)) {
            auto switch_ = new Input::Switch();
            switch_->module = node->module;
            input.switches.push_back(switch_);
            input.networkNodes.push_back(switch_);
        }
    }
}

void wTSNGateScheduleConfigurator::addDevices(Input& input) const
{
    for (int i = 0; i < topology->getNumNodes(); i++) {
        auto node = (Node *)topology->getNode(i);
        if (isDeviceNode(node)) {
            auto device = new Input::Device();
            device->module = node->module;
            input.devices.push_back(device);
            input.networkNodes.push_back(device);
        }
    }
}


bool wTSNGateScheduleConfigurator::isDeviceNode(Node *node) const{

    std::string sub = "device";
    if(node->getModule()->getFullPath().find(sub) != std::string::npos) {
        return true;
    }
    return false;
}


void wTSNGateScheduleConfigurator::addFlows(Input& input) const
{
    int flowIndex = 0;
    EV_DEBUG << "Computing flows from configuration" << EV_FIELD(configuration) << EV_ENDL;
    for (int k = 0; k < configuration->size(); k++) {
        auto entry = check_and_cast<cValueMap *>(configuration->get(k).objectValue());
        for (int i = 0; i < topology->getNumNodes(); i++) {
            auto sourceNode = (Node *)topology->getNode(i);
            cModule *source = sourceNode->module;
            for (int j = 0; j < topology->getNumNodes(); j++) {
                auto destinationNode = (Node *)topology->getNode(j);
                cModule *destination = destinationNode->module;
                PatternMatcher sourceMatcher(entry->get("source").stringValue(), true, false, false);
                PatternMatcher destinationMatcher(entry->get("destination").stringValue(), true, false, false);
                if (sourceMatcher.matches(sourceNode->module->getFullPath().c_str()) &&
                    destinationMatcher.matches(destinationNode->module->getFullPath().c_str()))
                {

                    int pcp = entry->get("pcp").intValue();
                    int gateIndex = entry->get("gateIndex").intValue();
                    b packetLength = b(entry->get("packetLength").doubleValueInUnit("b"));
                    b cutthroughSwitchingHeaderSize = entry->containsKey("cutthroughSwitchingHeaderSize") ? b(entry->get("cutthroughSwitchingHeaderSize").doubleValueInUnit("b")) : b(0);
                    simtime_t packetInterval = entry->get("packetInterval").doubleValueInUnit("s");
                    simtime_t maxLatency = entry->containsKey("maxLatency") ? entry->get("maxLatency").doubleValueInUnit("s") : -1;
                    simtime_t maxJitter = entry->containsKey("maxJitter") ? entry->get("maxJitter").doubleValueInUnit("s") : 0;
                    bps datarate = packetLength / s(packetInterval.dbl());

                    auto startDevice = input.getDevice(source);
                    auto endDevice = input.getDevice(destination);
                    auto startApplication = new Input::Application();
                    auto startApplicationModule = startDevice->module->getModuleByPath((std::string(".") + std::string(entry->get("application").stringValue())).c_str());

                    if (startApplicationModule == nullptr)
                        throw cRuntimeError("Cannot find flow start application, path = %s", entry->get("application").stringValue());
                    startApplication->module = startApplicationModule;
                    startApplication->device = startDevice;
                    startApplication->pcp = pcp;
                    startApplication->packetLength = packetLength;
                    startApplication->packetInterval = packetInterval;
                    startApplication->maxLatency = maxLatency;
                    startApplication->maxJitter = maxJitter;
                    input.applications.push_back(startApplication);
                    EV_DEBUG << "Adding flow from configuration" << EV_FIELD(source) << EV_FIELD(destination) << EV_FIELD(pcp) << EV_FIELD(packetLength) << EV_FIELD(packetInterval, packetInterval.ustr()) << EV_FIELD(datarate) << EV_FIELD(maxLatency, maxLatency.ustr()) << EV_FIELD(maxJitter, maxJitter.ustr()) << EV_ENDL;


                    auto flow = new Input::Flow();
                    flow->name = entry->containsKey("name") ? entry->get("name").stringValue() : (std::string("flow") + std::to_string(flowIndex++)).c_str();
                    flow->gateIndex = gateIndex;
                    flow->cutthroughSwitchingHeaderSize = cutthroughSwitchingHeaderSize;
                    flow->startApplication = startApplication;
                    flow->endDevice = endDevice;
                    float gamma = entry->get("gamma").doubleValue();
                    flow->gamma = gamma;

                    flow->flowStartTime = entry->get("flowStartTime").doubleValueInUnit("s");
                    flow->flowEndTime = entry->get("flowEndTime").doubleValueInUnit("s");

                    cout<<"======"<<endl;
                    cValueArray *pathFragments;
                    if (entry->containsKey("pathFragments"))
                        pathFragments = check_and_cast<cValueArray *>(entry->get("pathFragments").objectValue());
                    else {
                        auto pathFragment = new cValueArray();
                        for (auto node : computeShortestNodePath(sourceNode, destinationNode)){
                            pathFragment->add(node->module->getFullName());

//                            std::string full = node->module->getFullPath();
//                            std::string toRemove = "simpleTsn.";
//
//                            size_t pos = full.find(toRemove);
//                            if (pos != std::string::npos) {
//                                full.erase(pos, toRemove.length());
//                            }
//                            pathFragment->add(full);
                            //cout<<"Fragment "<<full<<endl;


                        }
                        //cout<<"======"<<endl;
                        pathFragments = new cValueArray();
                        pathFragments->add(pathFragment);
                    }

//                    for (int l = 0; l < pathFragments->size(); l++) {
//                         auto path = new Input::PathFragment();
//                         auto pathFragment = check_and_cast<cValueArray *>(pathFragments->get(l).objectValue());
//                         for (int m = 0; m < pathFragment->size(); m++) {
//                             for (auto networkNode : input.networkNodes) {
//                                 auto name = pathFragment->get(m).stdstringValue();
//                                 int index = name.find('.');
//                                 auto nodeName = index != std::string::npos ? name.substr(0, index) : name;
//                                 cout<<"name is "<<name<<" nodeName is "<<nodeName<< " networkNode->module->getFullName() "<< networkNode->module->getFullName() <<endl;
//                                 auto interfaceName = index != std::string::npos ? name.substr(index + 1) : "";
//                                 if (networkNode->module->getFullName() == nodeName) {
//                                     if (m != pathFragment->size() - 1) {
//                                         auto startNode = networkNode;
//                                         auto endNodeName = pathFragment->get(m + 1).stdstringValue();
//                                         int index = endNodeName.find('.');
//                                         endNodeName = index != std::string::npos ? endNodeName.substr(0, index) : endNodeName;
//                                         cout<<"Fragment is "<<startNode->module->getFullName() <<" to "<< endNodeName;
//                                         auto outputPort = *std::find_if(startNode->ports.begin(), startNode->ports.end(), [&] (const auto& port) {
//                                             return port->endNode->module->getFullName() == endNodeName && (interfaceName == "" || interfaceName == check_and_cast<NetworkInterface *>(port->module)->getInterfaceName());
//                                         });
//                                         path->outputPorts.push_back(outputPort);
//                                         path->inputPorts.push_back(outputPort->otherPort);
//                                     }
//                                     path->networkNodes.push_back(networkNode);
//                                     break;
//                                 }
//                             }
//                         }
//                         flow->pathFragments.push_back(path);
//                     }
                     if (!entry->containsKey("pathFragments"))
                         delete pathFragments;
                     input.flows.push_back(flow);
                }
            }
        }
    }

    std::sort(input.flows.begin(), input.flows.end(), [] (const Input::Flow *r1, const Input::Flow *r2) {
        return r1->startApplication->pcp > r2->startApplication->pcp;
    });
}

std::vector<StreamStatus> wTSNGateScheduleConfigurator::triggerGateScheduling(std::vector<StreamRegistrationRequest> requests){

    auto  input =  new Input();
    addSwitches(*input);
    addDevices(*input);
    GateScheduleConfiguratorBase::addPorts(*input);

    cout<<"wTSNGateScheduleConfigurator::triggerGateScheduling 3"<<endl;

    //GateScheduleConfiguratorBase::addFlows(*input);
    addFlows(*input);

    cout<<"wTSNGateScheduleConfigurator::triggerGateScheduling 2"<<endl;

     //more info: https://doc.omnetpp.org/inet/api-4.4.0/neddoc/inet.linklayer.configurator.gatescheduling.base.GateScheduleConfiguratorBase.html
     writeInputToFile(*input, "test.json");

    //Output * out = computeGateScheduling(*input);

    //TODO
    //1: Gate schedules


    //2: application start times
    std::vector<StreamStatus> talkerUpdates;
    StreamStatus s = StreamStatus("1","device1",true,0);
    talkerUpdates.push_back(s);
    return talkerUpdates;

}

wTSNGateScheduleConfigurator::Output *wTSNGateScheduleConfigurator::computeGateScheduling(const Input& input) const{
      //opt call
//      Py_Initialize();
//      auto output = get_optimal_assignments();
//      Py_Finalize();

    auto output = get_simulated_optimal_assignments();

    return output;
}

wTSNGateScheduleConfigurator::Output * wTSNGateScheduleConfigurator::get_simulated_optimal_assignments() const{
    auto output = new Output();
    //TODO: parse returned values from file
    //output->gateSchedules = gateSchedules;
    //output->applicationStartTimes = applicationStartTimes;
    return output;
}


wTSNGateScheduleConfigurator::Output * wTSNGateScheduleConfigurator::get_optimal_assignments() const {
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append(\".\")");

        PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pList, *pList_assignments;

        pName = PyUnicode_DecodeFSDefault("omnettest");
        pModule = PyImport_Import(pName);       // loaded script

        if(pModule == NULL)
            throw std::invalid_argument("Script cannot be loaded.\n");

        pDict = PyModule_GetDict(pModule);
        pFunc = PyDict_GetItemString(pDict, "main"); // load function

        PyObject* pResult = PyObject_CallObject(pFunc, nullptr); // call function

        //TODO: parse returned values
        return new Output();
}



} // namespace inet



