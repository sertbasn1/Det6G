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
    if (stage == INITSTAGE_LOCAL) {
        gateCycleDuration = par("gateCycleDuration");
        configuration = check_and_cast<cValueArray *>(par("configuration").objectValue());
    }

}

std::vector<StreamStatus> wTSNGateScheduleConfigurator::triggerGateScheduling(std::vector<StreamRegistrationRequest> requests){
    // TODO: Create Inout structure as in omnet implementation
    auto  input =  new Input();

    Output * out = computeGateScheduling(*input);

    //TODO: check implementation details
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



