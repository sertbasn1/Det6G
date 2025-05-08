#!/bin/bash
# parameters to run
#test ids ($1)

ROOTDIR=/simulations
PROJECT_PATH="$ROOTDIR/6GDetCom_Simulator/simulations/simpleTsn"
RES_PATH="$ROOTDIR/simResults"
LIBPATH="$ROOTDIR/6GDetCom_Simulator"

RUNCOMMAND="../../src/6GDetCom_Simulator -r 0 -m -u Cmdenv -n ..:../../src:../../../inet/examples:../../../inet/showcases:../../../inet/src:../../../inet/tests/validation:../../../inet/tests/networks:../../../inet/tutorials -x inet.common.selfdoc;inet.linklayer.configurator.gatescheduling.z3;inet.emulation;inet.showcases.visualizer.osg;inet.examples.emulation;inet.showcases.emulation;inet.transportlayer.tcp_lwip;inet.applications.voipstream;inet.visualizer.osg;inet.examples.voipstream --image-path=../../../inet/images -l ../../../inet/src/INET omnetpp.ini"

MAKECOMMAND="make"

#Clear $ROOTDIR/simResults and $ROOTDIR/6GDetCom_Simulator/simulations/simpleTsn/results
rm -r $RES_PATH
mkdir -p $RES_PATH
rm -r $PROJECT_PATH/results
mkdir -p $PROJECT_PATH/results

#run all tests specified by the user
arr=( $1 )
for i in "${arr[@]}";
do
    #CURRENTTESTFILE=$1_flows_test$i.ini
    #CURRENTSCENARIOFILE=$1_scenario_test$i.xml
    echo "Current running test: test$i"

    #run the simulation
    cd "$PROJECT_PATH"
    $RUNCOMMAND

    #Copy the simulationresults to permanent folder, clear results in project
    mkdir -p $RES_PATH/test$i
    cp -r "$PROJECT_PATH/results"  "$RES_PATH/test$i"
    rm -r $PROJECT_PATH/results
    mkdir -p $PROJECT_PATH/results
    
    echo "Results are prepearing.."
    sleep 3
    #extract results
    cd "$RES_PATH/test$i/results"  
    opp_scavetool x *.sca -o  results.csv

    cd $ROOTDIR

done



