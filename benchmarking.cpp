
// To compile in bash "g++ -std=c++17 benchmarking.cpp -o bm -lpthread"
// To run "./bm" 
#include <iostream>
#include <string>
#include <string.h>
#include <climits>
#include <cmath>
#include <float.h>
#include <map>
#include <vector>
#include <array>
#include <fstream>
#include <stack>
#include <algorithm>
#include <thread>
#include <chrono> 
#include "chalk.h"

using namespace chalk;


void integerOperations(long ops) {for (int i = 0; i < ops; i+=1) {}}

void floatingPointOperations(long ops) {for (double i = 0; i < ops; i+=1.0) {}}


// Run threaded tests for all combinations of threads and operations
std::vector< std::vector< std::vector<double> > > threadedTest(std::vector<long> operations, std::vector<int> threads, bool iops) {

    // Instantiate return vector
    std::vector< std::vector< std::vector<double> >> threadedResults;

    // For all numbers of threads
    for (int i = 0; i < threads.size(); i++) {
        std::vector< std::vector<double> > threadResults;

        // For all numbers of operations
        for (int j = 0; j < operations.size(); j++) {
            std::thread threadList[threads[i]];


// <----------------------------- START TIME -------------------------->
           std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
// <----------------------------- START TIME -------------------------->

            // Assign each thread a share of the workload
            long ops = operations[j]/threads[i];
            
            // Start each thread depending on running FLOPS or IOPS test
            for (int k = 0; k < threads[i]; k++ ) {
                if (iops) {
                    threadList[k] = std::thread(integerOperations, ops);
                } else {
                    threadList[k] = std::thread(floatingPointOperations, ops);
                }
            }

            // Wait for all threads to finish
            for (int k = 0; k < threads[i]; k++ ) {
                threadList[k].join();
            }

// <----------------------------- END TIME -------------------------->
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
// <----------------------------- END TIME -------------------------->

            // Calculate time elapsed in seconds
            auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            double totalSeconds = duration1/1e6;

            // Append results of time elapsed for all operations to complete
            std::vector<double> opsTime = {(double)operations[j], totalSeconds};
            threadResults.push_back(opsTime);
        }
        threadedResults.push_back(threadResults);
    }
    return threadedResults;
}

void writeOutResults(std::vector<int> threads,
                    std::vector< std::vector< std::vector<double>>>threadedIOPSResults,
                    std::vector<long long> threadedIOPS, 
                    std::vector< std::vector< std::vector<double>>>threadedFLOPSResults, 
                    std::vector<long long> threadedFLOPS) 
{

    // Setup output 
    std::ofstream out;
    out.open("results.py");

    out << "threadedIOPSResults = {\n";
    for (int i = 0; i < threads.size(); i++) {
        out << "\t" << std::to_string(threads[i]) << " : [\n";
        for (int j = 0; j < threadedIOPSResults[i].size(); j++) {
            out << "\t\t[ " << std::to_string(threadedIOPSResults[i][j][0]) << ", " << std::to_string(threadedIOPSResults[i][j][1]) << " ],\n";
        }
        out << "\t],\n";
    }
    out << "}\n\n";

    out << "threadedFLOPSResults = {\n";
    for (int i = 0; i < threads.size(); i++) {
        out << "\t" << std::to_string(threads[i]) << " : [\n";
        for (int j = 0; j < threadedFLOPSResults[i].size(); j++) {
            out << "\t\t[ " << std::to_string(threadedFLOPSResults[i][j][0]) << ", " << std::to_string(threadedFLOPSResults[i][j][1]) << " ],\n";
        }
        out << "\t],\n";
    }
    out << "}\n\n";

    out << "threadedIOPS = {\n";
    for (int i = 0; i < threads.size(); i++) {
        out << "\t" << std::to_string(threads[i]) << " : ";
        out << threadedIOPS[i] << ",\n";
    }
    out << "}\n\n";

    out << "threadedFLOPS = {\n";
    for (int i = 0; i < threads.size(); i++) {
        out << "\t" << std::to_string(threads[i]) << " : ";
        out << threadedFLOPS[i] << ",\n";
    }
    out << "}\n";

    out.close();
}

int main(int argc, char const *argv[]) {

    // Define tests inputs: Number of threads running concurrently / For how many operations
    std::vector<int> threads = {1, 2, 4, 8};
    std::vector<long> operations;

    long totalOps = 0;
    for (int i = 1; i < 1000; i++) {
        operations.push_back(i*1e5);
        totalOps += (i*1e5);
    }

    // Run threaded tests for IOPS and FLOPS
    std::vector< std::vector< std::vector<double> >> threadedResultsIOPS = threadedTest(operations, threads, true);
    std::vector< std::vector< std::vector<double> >> threadedResultsFLOPS = threadedTest(operations, threads, false);

    std::cout << fmt::Bold.Wrap(fg::Blue.Wrap("\n<------------------Results------------------>\n")) << std::endl;

    std::vector<double> threadedIOPSTime;
    std::vector<double> threadedFLOPSTime;

    std::cout << fmt::Bold.Wrap(fg::Green.Wrap("<------------------IOPS------------------>")) << std::endl;
    for (int i = 0; i < threadedResultsIOPS.size(); i++) {
        std::cout << fmt::Bold.Wrap(fg::Green.Wrap(std::to_string(threads[i]) + " threads:")) << std::endl;
        double sumTime = 0;
        for (int j = 0; j < threadedResultsIOPS[i].size(); j++) {

            std::cout << 
            fg::Yellow.Wrap(std::to_string((long)threadedResultsIOPS[i][j][0])) + " operations - "  << 
            fg::Yellow.Wrap(std::to_string(threadedResultsIOPS[i][j][1])) + " seconds" <<
            std::endl;

            sumTime += threadedResultsIOPS[i][j][1];
        }
        threadedIOPSTime.push_back(sumTime);
    }
    std::cout << std::endl;

    std::cout << fmt::Bold.Wrap(fg::Green.Wrap("<------------------FLOPS------------------>")) << std::endl;
    for (int i = 0; i < threadedResultsFLOPS.size(); i++) {
        std::cout << fmt::Bold.Wrap(fg::Green.Wrap(std::to_string(threads[i]) + " threads:")) << std::endl;
        double sumTime = 0;
        for (int j = 0; j < threadedResultsFLOPS[i].size(); j++) {
            
            std::cout << 
            fg::Yellow.Wrap(std::to_string((long)threadedResultsFLOPS[i][j][0])) + " operations - "  << 
            fg::Yellow.Wrap(std::to_string(threadedResultsFLOPS[i][j][1])) + " seconds" <<
            std::endl;

            sumTime += threadedResultsFLOPS[i][j][1];
        }
        threadedFLOPSTime.push_back(sumTime);
    }

    std::vector<long long> threadedIOPS;
    std::vector<long long> threadedFLOPS;

    std::cout << fmt::Bold.Wrap(fg::Blue.Wrap("\n<------------------Summary------------------>\n")) << std::endl;
    // Print averages
    for (int i = 0; i < threadedIOPSTime.size(); i++) {
        long long x = totalOps/threadedIOPSTime[i];
        threadedIOPS.push_back(x);
        std::cout << fmt::Bold.Wrap(fg::Green.Wrap(std::to_string(threads[i]) + " threads:")) << std::endl;
        std::cout << fg::Yellow.Wrap("IOPS: ") << x << std::endl;
        std::cout << fmt::Bold.Wrap(fg::Yellow.Wrap("Giga IOPS: ")) << fmt::Bold.Wrap(std::to_string(x/1e9)) << std::endl;
    }
    std::cout << std::endl;
    for (int i = 0; i < threadedFLOPSTime.size(); i++) {
        long long x = totalOps/threadedFLOPSTime[i];
        threadedFLOPS.push_back(x);
        std::cout << fmt::Bold.Wrap(fg::Green.Wrap(std::to_string(threads[i]) + " threads:")) << std::endl;
        std::cout << fg::Yellow.Wrap("FLOPS: ") << x << std::endl;
        std::cout << fmt::Bold.Wrap(fg::Yellow.Wrap("Giga FLOPS: ")) << fmt::Bold.Wrap(std::to_string(x/1e9)) << std::endl;
    }

    writeOutResults(threads, threadedResultsIOPS, threadedIOPS, threadedResultsFLOPS, threadedFLOPS);
    return 0;
}

