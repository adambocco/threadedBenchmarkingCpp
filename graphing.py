import matplotlib.pyplot as plt
import sys
import argparse
import numpy as np

from results import threadedIOPSResults, threadedFLOPSResults, threadedIOPS, threadedFLOPS

parser = argparse.ArgumentParser()
parser.add_argument("-a", "--all", help="Display all graph representations of results",
                    action="store_true")
parser.add_argument("-o", "--optimeiops", help="Operations versus time for all threads (IOPS)",
                    action="store_true")
parser.add_argument("-t", "--optimeflops", help="Operations versus time for all threads (IOPS)",
                    action="store_true")
parser.add_argument("-i", "--iops", help="Numbers of threads versus IOPS",
                    action="store_true")
parser.add_argument("-f", "--flops", help="Numbers of threads versus FLOPS",
                    action="store_true")
args = parser.parse_args()


def threadIOPS():
    plt.figure(1)
    plt.xlabel("Threads")
    plt.ylabel("IOPS")
    plt.title("Numbers of Concurrent Threads vs IOPS")
    x = []
    y = []
    for j in threadedIOPS: # For all the functions
        x.append(int(j))
        y.append(int(threadedIOPS[j]))

    x = np.array(x)
    y = np.array(y)
    plt.plot(x, y, '-xb')



def threadFLOPS():
    plt.figure(2)
    plt.xlabel("Threads")
    plt.ylabel("FLOPS")
    plt.title("Numbers of Concurrent Threads vs FLOPS")
    x = []
    y = []
    for j in threadedFLOPS: # For all the functions
        x.append(int(j))
        y.append(int(threadedFLOPS[j]))
    x = np.array(x)
    y = np.array(y)
    plt.plot(x, y, '-xb')




def iops():
    plt.figure(3)
    plt.title("Operations vs Time for Multiple Threads (IOPS)")

    plt.xlabel("Operations")
    plt.ylabel("Time")

    avgs = []
    numOfOperations = 0
    for i in threadedIOPSResults: # For all the thread numbers

        ops = []
        avgs = []

        for j in threadedIOPSResults[i]:

            ops.append(j[0])
            
            avgs.append(j[1])
        
        x = np.array(ops)
        y = np.array(avgs)

        plt.plot(x, y, 'o',marker=".", markersize=3)

        # Get slope and y-intercept of linear regression line
        m, b = np.polyfit(x, y, 1)
        print("Standard deviation: ", np.std(y))
        # Add linear regression line to graph
        plt.plot(x, m*x+b, label=str(i))
    plt.legend(fancybox=True, loc=2, title="# of Threads")


def flops():
    plt.figure(4)
    plt.title("Operations vs Time for Multiple Threads (FLOPS)")

    plt.xlabel("Operations")
    plt.ylabel("Time")

    avgs = []
    numOfOperations = 0
    for i in threadedFLOPSResults: # For all the thread numbers

        ops = []
        avgs = []

        for j in threadedFLOPSResults[i]:

            ops.append(j[0])
            
            avgs.append(j[1])
        
        x = np.array(ops)
        y = np.array(avgs)

        plt.plot(x, y, 'o',marker=".", markersize=3)

        # Get slope and y-intercept of linear regression line
        m, b = np.polyfit(x, y, 1)
        print("Standard deviation: ", np.std(y))
        # Add linear regression line to graph
        plt.plot(x, m*x+b, label=str(i))
    plt.legend(fancybox=True, loc=2, title="# of Threads")



if args.all:
    threadIOPS()
    threadFLOPS()
    iops()
    flops()
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