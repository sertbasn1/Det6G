///*
// * SimpleUdpSourceApp.h
// *
// *  Created on: May 15, 2025
// *      Author: root
// */
//
//#ifndef INET_APPLICATIONS_UDPAPP_SIMPLEUDPSOURCEAPP_H_
//#define INET_APPLICATIONS_UDPAPP_SIMPLEUDPSOURCEAPP_H_
//
//#include "inet/applications/base/ApplicationBase.h"
//#include "inet/transportlayer/contract/udp/UdpSocket.h"
//#include "inet/applications/udpapp/SimpleUdpSinkApp.h"
//namespace inet {
//
//
//class INET_API SimpleUdpSourceApp : public ApplicationBase, public UdpSocket::ICallback
//{
//  protected:
//    virtual void handleMessageWhenUp(cMessage *msg) override;
//    virtual int numInitStages() const override { return 0; }
//    virtual void initialize(int stage) override;
//    virtual void finish() override;
//    virtual void refreshDisplay() const override;
//
//    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
//    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
//    virtual void socketClosed(UdpSocket *socket) override;
//  public:
//      SimpleUdpSourceApp() {}
//      virtual ~SimpleUdpSourceApp();
//
//};
//
//}
//#endif /* INET_APPLICATIONS_UDPAPP_SIMPLEUDPSOURCEAPP_H_ */
