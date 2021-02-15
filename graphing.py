import matplotlib.pyplot as plt
import sys
import argparse
import numpy as np
from simple_chalk import chalk, blue, yellow ,green

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
parser.add_argument("-n", "--homoiops", help="Numbers of threads versus Giga FLOPS",
                    action="store_true")
parser.add_argument("-m", "--homoflops", help="Numbers of threads versus Giga FLOPS",
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

        for j in threadedIOPSResults[i]:

            ops.append(j[0])
            
            avgs.append(j[1])

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

        for j in threadedFLOPSResults[i]:

            ops.append(j[0])
            
            avgs.append(j[1])

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
    plt.xlabel("Threads")
    plt.ylabel("Giga IOPS")

    x = []
    y = []
    iterations = 0
    for j in threadedIOPSResults: 
        iterations = len(threadedIOPSResults[j])
        for n in threadedIOPSResults[j]:
            x.append(int(j))
            y.append((n[0]/n[1])/1e9)


    x = np.array(x)
    y = np.array(y)
    m, b = np.polyfit(x, y, 1)
    plt.title(f"Threads versus IOPS over\n{iterations} iterations per numbers of threads")
    plt.plot(x, y, 'o',marker=".", markersize=5)
    plt.plot(x, m*x+b)


def homoOpsFLOPS():
    plt.figure(6)
    plt.xlabel("Threads")
    plt.ylabel("Giga FLOPS")
    x = []
    y = []
    iterations = 0
    for j in threadedFLOPSResults:
        iterations = len(threadedFLOPSResults[j])
        for n in threadedFLOPSResults[j]:
            x.append(int(j))
            y.append((n[0]/n[1])/1e9)

    x = np.array(x)
    y = np.array(y)
    m, b = np.polyfit(x, y, 1)
    plt.title(f"Threads versus FLOPS over\n{iterations} iterations per numbers of threads")
    plt.plot(x, y, 'o',marker=".", markersize=5)
    plt.plot(x, m*x+b)


def standardDeviation():
    print(chalk.blue("<------------- Standard Deviation ------------->\n"))
    print(chalk.green("<------------- Giga IOPS ------------->\n"))
    for i in threadedIOPSResults: # For all the thread numbers
        std = []
        for j in threadedIOPSResults[i]:
            try:
                std.append((j[0]/j[1])/1e9)
            except ZeroDivisionError:
                std.append(0)
        print(chalk.yellow(str(i)+ " threads: " + str(np.std(std))))
    print(chalk.green("\n<------------- Giga IOPS ------------->\n"))
    for i in threadedFLOPSResults: # For all the thread numbers
        std = []
        for j in threadedFLOPSResults[i]:
            try:
                std.append((j[0]/j[1])/1e9)
            except ZeroDivisionError:
                std.append(0)
        print(chalk.yellow(str(i)+ " threads: " + str(np.std(std))))



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
    if args.homoiops:
        homoOpsIOPS()
    if args.homoflops:
        homoOpsFLOPS()

standardDeviation()
plt.show()