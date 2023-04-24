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

#include "myQ.h"

Define_Module(MyQ);

void MyQ::initialize()
{
    queue.setName("queue");
}

int MyQ::getSize()
{
    return queue.getLength();
}

void MyQ::handleMessage(cMessage *msg)
{
    int ql; //=9;
        ql = queue.getLength();
    if (msg->arrivedOn("rxPackets")){
        queue.insert(msg);
    } else if (msg->arrivedOn("rxScheduling")){
        short nrOfRadioBlocks = msg->getKind();
        delete msg;
        for(short j = 0; j < nrOfRadioBlocks && !queue.isEmpty(); j++){
              msg = (cMessage *)queue.pop();
              send(msg, "txPackets");
        }
    }
    ql = queue.getLength();
    cPar&  qlpar = par("qlp");
    qlpar.setIntValue(ql);
    //next 2 statements only for debug
        int qt = par("qlp").intValue();
        EV << "qt= "<<qt <<endl;
}
