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

package inet.centralconfigurator;
import inet.node.ethernet.EthernetLink;
import inet.node.tsn.TsnDevice;
import inet.linklayer.configurator.gatescheduling.contract.IGateScheduleConfigurator;

module TsnController
{
    parameters:
        @display("i=block/control;bgb=426,241");
    submodules:
        cuc: CentralUserConfig {
            @display("i=block/process;p=151,66;is=l");
        }

        cnc: CentralNetworkConfig {
            @display("i=block/process;p=151,162;is=l");
        }

        //theoratically, this needs to be inside the cnc
        gateScheduleConfigurator: <default("")> like IGateScheduleConfigurator if typename != "" {
            @display("p=325,148;is=s");
        }

    connections allowunconnected:
        cuc.toFromCnc <--> cnc.toFromCuc;




}
