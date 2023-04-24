//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Sink.h"
Define_Module(Sink);

void Sink::initialize()
{
    lifetimeSignal = registerSignal("lifetime");
    NetworkLoad = par("NetworkLoad").doubleValue();
    NrUsers = par("gateSize").intValue();
    NrOfChannels = par("NrOfChannels").intValue();
    NrPackets = 0;
    totalDelay = 0;
    delay = 0;
}

void Sink::handleMessage(cMessage *msg)
{
    simtime_t lifetime = simTime() - msg->getCreationTime();
    int user0 = 0;//(int)getParentModule()->getParentModule()->par("user_HP");
    for(int i = 0; i < NrUsers; i++){
        if (msg->arrivedOn("rxPackets",user0)){
            lifetimeStats[0].collect(lifetime);
            NrPackets++;
            totalDelay += lifetime.dbl();
        }
        else if (msg->arrivedOn("rxPackets",i)) {
            lifetimeStats[i].collect(lifetime);
        }
    }

    if(msg->arrivedOn("in")){
        if(NrPackets > 0)
            delay = (int)(totalDelay * 10  / NrPackets);
        else delay = 0;
        EV << "Total Delay: " << totalDelay << " Delay: " << delay << " NrPackets: " << NrPackets << endl;
        NrPackets = 0;
        totalDelay = 0;
    }

    EV << "Received " << msg->getName() << ", lifetime: " << lifetime << "s" << endl;
    emit(lifetimeSignal, lifetime);
    if(msg->arrivedOn("in")){

    }
    cMessage *msgr = new cMessage("sink");
    msgr->setKind((int) delay);
    send(msgr, "x0");

    delete msg;
}

void Sink::finish(){
    EV << "finish() from sink!\n";
}
