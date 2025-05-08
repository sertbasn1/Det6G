import csv

file="/simulations/simResults/test1/results/results.csv"
#run,type,module,name,attrname,attrvalue,value,count,sumweights,mean,stddev,min,max,underflows,overflows,binedges,binvalues

with open(file) as file_obj:
    reader_obj = csv.reader(file_obj)
    for row in reader_obj:
        if row[1] == 'scalar' and row[2] \
            == 'simpleTsn.device1.app[0].io' and row[3] \
            == 'packetSent:count':
            #print(row)
            print(row[6])
        if row[1] == 'scalar' and row[2] \
            == 'simpleTsn.device2.app[0].io' and row[3] \
            == 'packetReceived:count':
            #print(row)
            print(row[6])
        elif row[1] == 'histogram' and row[2] \
            == 'simpleTsn.device2.app[0].sink' and row[3] \
            == 'packetJitter:histogram':
            #print(row)
            print(row[16])
