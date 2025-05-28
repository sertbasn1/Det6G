/*
 * wTSNGateScheduleConfigurator.h
 *
 *  Created on: May 20, 2025
 *      Author: root
 */

#ifndef INET_LINKLAYER_CONFIGURATOR_GATESCHEDULING_COMMON_WTSNGATESCHEDULECONFIGURATOR_H_
#define INET_LINKLAYER_CONFIGURATOR_GATESCHEDULING_COMMON_WTSNGATESCHEDULECONFIGURATOR_H_


#include "inet/linklayer/configurator/gatescheduling/base/GateScheduleConfiguratorBase.h"
#include "inet/linklayer/configurator/gatescheduling/common/TSNschedGateScheduleConfigurator.h"
#include "inet/centralconfigurator/StreamRegistrationRequest.h"
#include "inet/centralconfigurator/StreamRegistrationResponse.h"
#include "inet/common/PatternMatcher.h"
#include "inet/queueing/gate/PeriodicGate.h"
#include <Python.h>
#include <iostream>
#include <string>
using namespace std;
namespace inet {

class INET_API wTSNGateScheduleConfigurator : public TSNschedGateScheduleConfigurator
{
    protected:
        virtual void initialize(int stage) override;

        virtual Output *computeGateScheduling(const Input& input) const override;
        Output *  get_optimal_assignments() const;
        Output *  get_simulated_optimal_assignments() const;
        virtual void addSwitches(Input& input) const override;
        virtual bool isDeviceNode(Node *node) const;

        virtual void addDevices(Input& input) const override;
        virtual void addFlows(Input& input) const override;

    public:
        std::vector<StreamStatus> triggerGateScheduling(std::vector<StreamRegistrationRequest> requests);

};

}

#endif /* INET_LINKLAYER_CONFIGURATOR_GATESCHEDULING_COMMON_WTSNGATESCHEDULECONFIGURATOR_H_ */
