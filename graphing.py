import matplotlib.pyplot as plt
import sys
import argparse
import numpy as np

from results import threadedIOPSResults, threadedFLOPSResults, threadedIOPS, threadedFLOPS

parser = argparse.ArgumentParser()
parser.add_argument("-a", "--all", help="Display all graph representations of results",
                    action="store_true")
parser.add_argument("-o", "--optimeiops", help="Operations versus time for all threads (Giga IOPS)",
                    action="store_true")
parser.add_argument("-t", "--optimeflops", help="Operations versus time for all threads (Giga FLOPS)",
                    action="store_true")
parser.add_argument("-i", "--iops", help="Numbers of threads versus Giga IOPS",
                    action="store_true")
parser.add_argument("-f", "--flops", help="Numbers of threads versus Giga FLOPS",
                    action="store_true")
args = parser.parse_args()


def threadIOPS():
    plt.figure(1)
    plt.xlabel("Threads")
    plt.ylabel("Giga IOPS")
    plt.title("Numbers of Concurrent Threads vs Giga IOPS")
    x = []
    y = []
    for j in threadedIOPS: 
        x.append(int(j))
        y.append(threadedIOPS[j]/1e9)

    x = np.array(x)
    y = np.array(y)
    plt.plot(x, y, '-xb')


def threadFLOPS():
    plt.figure(2)
    plt.xlabel("Threads")
    plt.ylabel("Giga FLOPS")
    plt.title("Numbers of Concurrent Threads vs Giga FLOPS")
    x = []
    y = []
    for j in threadedFLOPS: 
        x.append(int(j))
        y.append(threadedFLOPS[j]/1e9)
    x = np.array(x)
    y = np.array(y)
    plt.plot(x, y, '-xb')


def iops():
    plt.figure(3)
    plt.title("Operations vs Time for Multiple Threads (IOPS)")

    plt.xlabel("Operations (Integer)")
    plt.ylabel("Time (Seconds)")

    avgs = []
    numOfOperations = 0
    for i in threadedIOPSResults: # For all the thread numbers

        ops = []
        avgs = []

        tempForStd = []
        for j in threadedIOPSResults[i]:

            ops.append(j[0])
            
            avgs.append(j[1])
            try:
                tempForStd.append((j[0]/j[1])/1e9)
            except ZeroDivisionError:
                tempForStd.append(0)
        print("Standard deviation of Figure 3 GigaIOPS on thread : "+str(i)+ " :: ", np.std(tempForStd))
        x = np.array(ops)
        y = np.array(avgs)

        plt.plot(x, y, 'o',marker=".", markersize=5)

        # Get slope and y-intercept of linear regression line
        m, b = np.polyfit(x, y, 1)

        # Add linear regression line to graph
        plt.plot(x, m*x+b, label=str(i))
    plt.legend(fancybox=True, loc=2, title="# of Threads")


def flops():
    plt.figure(4)
    plt.title("Operations vs Time for Multiple Threads (FLOPS)")

    plt.xlabel("Operations (Floating-Point)")
    plt.ylabel("Time (Seconds)")

    avgs = []
    numOfOperations = 0
    for i in threadedFLOPSResults: # For all the thread numbers

        ops = []
        avgs = []

        tempForStd = []
        for j in threadedFLOPSResults[i]:

            ops.append(j[0])
            
            avgs.append(j[1])
            try:
                tempForStd.append((j[0]/j[1])/1e9)
            except ZeroDivisionError:
                tempForStd.append(0)
        print("Standard deviation of Figure 3 GigaFLOPS on thread : "+str(i)+ " :: ", np.std(tempForStd))


        x = np.array(ops)
        y = np.array(avgs)

        plt.plot(x, y, 'o',marker=".", markersize=5)

        # Get slope and y-intercept of linear regression line
        m, b = np.polyfit(x, y, 1)
        # Add linear regression line to graph
        plt.plot(x, m*x+b, label=str(i))
    plt.legend(fancybox=True, loc=2, title="# of Threads")


def homoOpsIOPS():
    plt.figure(5)
    plt.xlabel("Giga IOPS")
    plt.ylabel("Threads")
    plt.title("Threads versus IOPS \n20 iterations of 1e10 operations")
    x = []
    y = []

    for j in threadedIOPSResults: 
        tempForStd = []
        for n in threadedIOPSResults[j]:
            y.append(int(j))
            x.append((n[0]/n[1])/1e9)
            tempForStd.append((n[0]/n[1])/1e9)
        stddev = np.array(tempForStd).std()
        print("Standard deviation for homoOpsIOPS on IOPS, threads: "+str(j)+" :::   " + str(stddev))
    x = np.array(x)
    y = np.array(y)
    m, b = np.polyfit(x, y, 1)
    plt.plot(x, y, 'o',marker=".", markersize=5)
    plt.plot(x, m*x+b)


def homoOpsFLOPS():
    plt.figure(6)
    plt.xlabel("Giga FLOPS")
    plt.ylabel("Threads")
    plt.title("Threads versus FLOPS \n20 iterations of 1e10 operations")
    x = []
    y = []
    for j in threadedFLOPSResults:
        tempForStd = [] 
        for n in threadedFLOPSResults[j]:
            y.append(int(j))
            x.append((n[0]/n[1])/1e9)
            tempForStd.append((n[0]/n[1])/1e9)
        stddev = np.array(tempForStd).std()
        print("Standard deviation for homoOpsIOPS on FLOPS, threads: "+str(j)+" :::   " + str(stddev))
    x = np.array(x)
    y = np.array(y)
    m, b = np.polyfit(x, y, 1)
    plt.plot(x, y, 'o',marker=".", markersize=5)
    plt.plot(x, m*x+b)





if args.all:
    threadIOPS()
    threadFLOPS()
    iops()
    flops()
    homoOpsIOPS()
    homoOpsFLOPS()

else:
    if args.optimeiops:
        threadIOPS()
    if args.optimeflops:
        threadFLOPS()
    if args.iops:
        iops()
    if args.flops:
        flops()


plt.show()