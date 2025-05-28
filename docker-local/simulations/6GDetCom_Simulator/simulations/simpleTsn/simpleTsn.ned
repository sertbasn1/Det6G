package d6g.simulations.simpleTsn;

import d6g.devices.DetCom;
import d6g.devices.tsntranslator.TTChannel;
import d6g.devices.tsntranslator.TsnTranslator;
import d6g.distribution.histogram.Histogram;
import d6g.distribution.histogram.HistogramContainer;
import d6g.networks.DetComNetworkBase;
import inet.node.ethernet.EthernetLink;
import inet.node.tsn.TsnDevice;
import inet.node.tsn.TsnSwitch;
import inet.common.scenario.ScenarioManager;
import inet.centralconfigurator.TsnController;
import inet.node.ethernet.Eth100M;
import inet.common.misc.ThruputMeteringChannel;

// Includes a HistogramContainer.
network simpleTsn extends DetComNetworkBase
{
    @display("bgb=1068.972,773.33203");
    types:
        channel controlplane extends ThruputMeteringChannel
        {
            delay = 1us;
            datarate = 1Gbps;
            thruputDisplayFormat = "u";
        }

    submodules:
        tsnController: TsnController {
            @display("p=620.84406,60.684002");
        }

        scenarioManager: ScenarioManager {
            @display("p=720,659");
        }

        histogramContainer: HistogramContainer {
            @display("p=416,659");
        }

        bridge1: TsnSwitch { // A DetCom Adapter.
            @display("p=526,239");
        }
        bridge2: TsnSwitch { // A TSN switch with a bridge function.
            @display("p=709,239");
        }
        bridge3: TsnSwitch { // A TSN switch with a bridge function.
            @display("p=625,398");
        }
        device1: TsnDevice {
            @display("p=319,240");
        }
        device2: TsnDevice { // A TSN device acting as a talker (data sender).
            @display("p=960.05206,239.62401");
        }
        device3: TsnDevice { // A TSN device acting as a talker (data sender).
            @display("p=627.06805,533.708");
        }
        detCom1: DetCom { // DetCom module (deterministic communication).
            @display("p=422,240");
        }
        detCom2: DetCom { // DetCom module (deterministic communication).
            @display("p=834.01605,241.18001");            
        }

    connections:
        device1.ethg++ <--> EthernetLink <--> detCom1.dsttg++;
        detCom1.nwttg <--> EthernetLink <--> bridge1.ethg++;

        device2.ethg++ <--> EthernetLink <--> detCom2.dsttg++;
        detCom2.nwttg <--> EthernetLink <--> bridge2.ethg++;

        device3.ethg++ <--> EthernetLink <--> bridge3.ethg++;

        bridge1.ethg++ <--> EthernetLink <--> bridge2.ethg++;
        bridge1.ethg++ <--> EthernetLink <--> bridge3.ethg++;
        bridge3.ethg++ <--> EthernetLink <--> bridge2.ethg++;



}
