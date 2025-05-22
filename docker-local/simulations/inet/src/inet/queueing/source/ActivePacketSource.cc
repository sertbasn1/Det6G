//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#include "inet/queueing/source/ActivePacketSource.h"

#include "../../centralconfigurator/StreamRegistrationRequest.h"
using namespace std;
#include <string>
namespace inet {
namespace queueing {

Define_Module(ActivePacketSource);

void ActivePacketSource::initialize(int stage)
{
    ClockUserModuleMixin::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        initialProductionOffset = par("initialProductionOffset");
        productionIntervalParameter = &par("productionInterval");
        productionTimer = new ClockEvent("ProductionTimer");
        scheduleForAbsoluteTime = par("scheduleForAbsoluteTime");
        streamingEnabled = par("streamingEnabled");

        if(streamingEnabled==false){\
            //SRP like stream registration will be triggered
            cMessage *event = new cMessage("TA msg");
            event->setKind(MSGKIND_SEND_TA);
            scheduleAt(initialProductionOffset.asSimTime(), event);
        }

    }
    else if (stage == INITSTAGE_QUEUEING) {
        checkPacketOperationSupport(outputGate);
        if (!productionTimer->isScheduled())
            scheduleProductionTimerAndProducePacket();
    }
}

void ActivePacketSource::handleMessage(cMessage *message)
{

    if (message->isPacket() && message->arrivedOn("fromCuc")) {
        cPacket *packet = check_and_cast<cPacket *>(message);
         string  sid = packet->par("sid").str();
         bool status = packet->par("status").boolValue() ;
         double offset = message->par("offset");

         cout<<"==========> "<< this->getFullPath()<<" received message from CUC for stream "<< sid<<" as "<<status<<" with offset"<<offset<<endl;

         if (status==true){
             streamingEnabled = true;
             //todo: set a timer here to schedule setting streamingEnabled as true for the next transmission
             scheduleProductionTimerAndProducePacket();
         }

         delete message;
         return;
        }
    if (message->getKind()== MSGKIND_SEND_TA){
        sendStreamRegistrationRequestMessage();
        cout<<"==========> "<< this->getFullPath() <<" sends TA to CUC "<<endl;

        delete message;
        return;
    }

    if (message == productionTimer) {
        if (consumer == nullptr || consumer->canPushSomePacket(outputGate->getPathEndGate())) {
            scheduleProductionTimer(productionIntervalParameter->doubleValue());
            producePacket();
        }
    }
    else
        throw cRuntimeError("Unknown message");
}

void ActivePacketSource::handleParameterChange(const char *name)
{
    if (!strcmp(name, "initialProductionOffset"))
        initialProductionOffset = par("initialProductionOffset");

    if (!strcmp(name, "streamingEnabled"))
        streamingEnabled = par("streamingEnabled");

}

void ActivePacketSource::scheduleProductionTimer(clocktime_t delay)
{
    if (scheduleForAbsoluteTime)
        scheduleClockEventAt(getClockTime() + delay, productionTimer);
    else
        scheduleClockEventAfter(delay, productionTimer);
}

void ActivePacketSource::scheduleProductionTimerAndProducePacket()
{
    if (!initialProductionOffsetScheduled && initialProductionOffset >= CLOCKTIME_ZERO  && streamingEnabled==true) {
        scheduleProductionTimer(initialProductionOffset);
        initialProductionOffsetScheduled = true;
    }
    else if (streamingEnabled==true && (consumer == nullptr || consumer->canPushSomePacket(outputGate->getPathEndGate()))) {
        scheduleProductionTimer(productionIntervalParameter->doubleValue());
        producePacket();
    }
}

void ActivePacketSource::producePacket()
{
    auto packet = createPacket();
    EV_INFO << "Producing packet" << EV_FIELD(packet) << EV_ENDL;

    cout << this->getFullName() <<": Producing packet" << endl;

    emit(packetPushedSignal, packet);
    pushOrSendPacket(packet, outputGate, consumer);
    updateDisplayString();
}

void ActivePacketSource::handleCanPushPacketChanged(cGate *gate)
{
    Enter_Method("handleCanPushPacketChanged");
    if (!productionTimer->isScheduled())
        scheduleProductionTimerAndProducePacket();
}

void ActivePacketSource::handlePushPacketProcessed(Packet *packet, cGate *gate, bool successful)
{
    Enter_Method("handlePushPacketProcessed");
}


void ActivePacketSource::sendStreamRegistrationRequestMessage()
{
    cPacket *packet = new cPacket("StreamRegistrationRequest");
    packet->setKind(MSGKIND_SEND_TA);

    string sid = this->getParentModule()->par("streamId").stringValue();

    std::string input = this->getFullPath();
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;

    while (std::getline(ss, token, '.')) {
        tokens.push_back(token);
    }
    string talker_str = tokens[1];
    string listener_str = (this->getParentModule())->getSubmodule("io")->par("destAddress").stdstringValue();

    int talker_id=-1;
    int listener_id=-1;
    int packet_size = par("packetLength");
    int priority = this->getParentModule()->par("priority").intValue();
    float period = par("productionInterval").doubleValue();
    float max_jitter = this->getParentModule()->par("max_jitter").doubleValue();
    float max_latency = this->getParentModule()->par("max_latency").doubleValue();
    float gamma = this->getParentModule()->par("gamma").doubleValue();


    StreamRegistrationRequest *data = new StreamRegistrationRequest(sid, talker_str, listener_str, talker_id, listener_id,
            packet_size, priority, period, max_jitter, max_latency, gamma);
    packet->addObject(data);

    cModule * base = ((this->getParentModule())->getParentModule())->getParentModule();
    cModule * targetModule = (base->getSubmodule("tsnController"))->getSubmodule("cuc");
    cGate *targetGate = targetModule->gate("userInterfaces$i");
    sendDirect(packet, targetGate);

}

} // namespace queueing
} // namespace inet

