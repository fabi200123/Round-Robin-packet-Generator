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
import User;
import Sink;
import Scheduler;
import FLC;

network RadioSch
{
parameters:
int usersCount;
int nrOfChannels;
double networkLoad;
//= default(3);

submodules:
scheduler: Scheduler {
    gateSize = usersCount;
    NrOfChannels = nrOfChannels;
//@display("p=120,40");
}
sink: Sink {
gateSize = usersCount;
NrOfChannels = nrOfChannels;
NetworkLoad = networkLoad;
//@display("p=120,200");
}
user[usersCount]: User {
//@display("p=40,120");
}
flc: FLC{
    //@display("p=40,120");
}
gen: GeneratorFLC{
    //@display("p=40,120");
}
connections: 
for i = 0..usersCount - 1 {
scheduler.txScheduling[i] --> user[i].rxScheduling;
user[i].txPackets --> sink.rxPackets[i];
}
gen.out --> flc.in;
flc.out --> sink.in;
sink.x0 --> flc.fromSink;
}