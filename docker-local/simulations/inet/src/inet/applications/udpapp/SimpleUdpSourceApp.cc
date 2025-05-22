///*
// * SimpleUdpSourceApp.cc
// *
// *  Created on: Apr 22, 2025
// *      Author: root
// */
//#include "inet/applications/udpapp/SimpleUdpSourceApp.h"
//
//#include "inet/common/ModuleAccess.h"
//#include "inet/common/packet/Packet.h"
//#include "inet/networklayer/common/L3AddressResolver.h"
//#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
//
//
//namespace inet {
//
//using namespace std;
//Define_Module(SimpleUdpSourceApp);
//
//SimpleUdpSourceApp::~SimpleUdpSourceApp()
//{
//    std::cout << getFullPath() ;
//
//}
//
//
//void SimpleUdpSourceApp::handleMessageWhenUp(cMessage *msg)
//{
//    if (msg->arrivedOn("toFromCuc")){
//        cout<<"Received from CUC" << endl;
//        //socket.processMessage(msg);
//    }
//    else{
//        handleMessageWhenUp(msg);
//    }
//}
//
//void SimpleUdpSourceApp::initialize(int stage) {}
//void SimpleUdpSourceApp::finish() {}
//void SimpleUdpSourceApp::refreshDisplay() const{}
//
//void SimpleUdpSourceApp::socketDataArrived(UdpSocket *socket, Packet *packet){}
//void SimpleUdpSourceApp::socketErrorArrived(UdpSocket *socket, Indication *indication) {}
//void SimpleUdpSourceApp::socketClosed(UdpSocket *socket){}
//
//}
//
