/*
 * TsnController.h
 *
 *  Created on: May 5, 2025
 *      Author: root
 */

#ifndef INET_CENTRALCONFIGURATOR_TSNCONTROLLER_H_
#define INET_CENTRALCONFIGURATOR_TSNCONTROLLER_H_

namespace inet {
    class TsnController  {
        public:
            TsnController();
            ~TsnController();
            virtual void initialize();
            //virtual void handleMessage(cMessage *msg);
    };

}


#endif /* INET_CENTRALCONFIGURATOR_TSNCONTROLLER_H_ */
