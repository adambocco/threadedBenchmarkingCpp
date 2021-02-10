
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
#include <sstream>
#include "chalk.h"

using namespace chalk;


void integerOperations(long ops) {for (long i = 0; i < ops; i+=1) {}}

void floatingPointOperations(long ops) {for (double long i = 0; i < ops; i+=1.0) {}}

std::map<long, double> unthreadedTest(std::vector<long> operations, bool iops) {

    std::map<long, double> results;

    for (int i = 0; i < operations.size(); i++) {

// <----------------------------- START TIME -------------------------->
           std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
// <----------------------------- START TIME -------------------------->

         if (iops) {
            integerOperations(operations[i]);
        } 
        else {
            floatingPointOperations(operations[i]);
        }

// <----------------------------- END TIME -------------------------->
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
// <----------------------------- END TIME -------------------------->

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        double totalSeconds = duration/1e6;

        // Append results of time elapsed for all operations to complete
        results[operations[i]] =  totalSeconds;

    }
    return results;
}

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
                } 
                else {
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

void writeOutResults(std::vector<int> &threads,
                    std::vector< std::vector< std::vector<double>>> &threadedIOPSResults,
                    std::vector<long long> &threadedIOPS, 
                    std::vector< std::vector< std::vector<double>>> &threadedFLOPSResults, 
                    std::vector<long long> &threadedFLOPS,
                    std::map<long, double> &unthreadedIOPSResults,
                    std::map<long, double> &unthreadedFLOPSResults,
                    long long unthreadedIOPS,
                    long long unthreadedFLOPS) 
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
    out << "}\n\n";

    out << "unthreadedIOPSResults = {\n";
    for ( const std::pair<const long, double> &entry : unthreadedIOPSResults ) {
        out << "\t" << std::to_string(entry.first) << " : ";
        out << std::to_string(entry.second) << ",\n";
    } 
    out << "}\n\n";

    out << "unthreadedFLOPSResults = {\n";
    for ( const std::pair<const long, double> &entry : unthreadedFLOPSResults ) {
        out << "\t" << std::to_string(entry.first) << " : ";
        out << std::to_string(entry.second) << ",\n";
    } 
    out << "}\n\n";

    out << "unthreadedIOPS = " << std::to_string(unthreadedIOPS);
    out << "\n\n";
    out << "unthreadedFLOPS = " << std::to_string(unthreadedFLOPS);
    out << "\n";

    out.close();
}

int main(int argc, char const *argv[]) {

    // Define tests inputs: Number of threads running concurrently / For how many operations
    std::vector<int> threads;
    std::vector<long> operations;   
    long totalOps = 0;

    // Handle command line arguments
    bool useDefaults = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        std::cout << arg << std::endl;
        if (arg.compare("--default") == 0 || arg.compare("-d")) {
            useDefaults = true;
            // Default test values
            threads = {1, 2, 4, 8};
            for (int i = 1; i < 5; i+=1) {
                operations.push_back(i*1e7);
                totalOps += (i*1e7);
            }
        }
    }
    if (!useDefaults) {
        std::string userInput = "";
        std::cout << fg::Blue.Wrap("Enter one or more number of threads to test ( ',' comma delimited; EX: 1,2,4,8 ) :\n");
        std::cin >> userInput;
        std::stringstream ss1(userInput);

        for (int i; ss1 >> i;) {
            threads.push_back(i);    
            if (ss1.peek() == ',')
                ss1.ignore();
        }
        userInput = "";
        std::cout << fg::Blue.Wrap("Enter one or more number of operations to test ( ',' comma delimited; EX: 1000,10000,50000,100000 ) :\n");
        std::cin >> userInput;
        std::stringstream ss2(userInput);
        for (long i; ss2 >> i;) {
            operations.push_back(i);    
            totalOps += i;
            if (ss2.peek() == ',')
                ss2.ignore();
        }
    }
    std::cout << totalOps << std::endl;

    // Run threaded tests for IOPS and FLOPS
    std::vector< std::vector< std::vector<double> >> threadedResultsIOPS = threadedTest(operations, threads, true);
    std::vector< std::vector< std::vector<double> >> threadedResultsFLOPS = threadedTest(operations, threads, false);

    std::map<long, double> unthreadedIOPSResults = unthreadedTest(operations, true);
    std::map<long, double> unthreadedFLOPSResults = unthreadedTest(operations, false);


    long long unthreadedIOPS, unthreadedFLOPS;
    double sumTime = 0;
    std::vector<double> threadedIOPSTime;
    std::vector<double> threadedFLOPSTime;

    std::cout << fmt::Bold.Wrap(fg::Yellow.Wrap("\n<------------------Detailed Results------------------>\n"));
    std::cout << fmt::Bold.Wrap(fg::Blue.Wrap("\n<------------------Unthreaded Results------------------>\n"));
    std::cout << fmt::Bold.Wrap(fg::Green.Wrap("<------------------IOPS------------------>\n"));

    for ( const std::pair<const long, double> &entry : unthreadedIOPSResults ) {
        
            std::cout << 
            fg::Yellow.Wrap(std::to_string(entry.first)) + " operations - "  << 
            fg::Yellow.Wrap(std::to_string(entry.second)) + " seconds" <<
            std::endl;

            sumTime += entry.second;
    } 
    unthreadedIOPS = totalOps / sumTime;
    sumTime = 0;

    std::cout << std::endl;

    std::cout << fmt::Bold.Wrap(fg::Green.Wrap("<------------------FLOPS------------------>\n"));
    for ( const std::pair<const long, double> &entry : unthreadedFLOPSResults ) {
        
            std::cout << 
            fg::Yellow.Wrap(std::to_string(entry.first)) + " operations - "  << 
            fg::Yellow.Wrap(std::to_string(entry.second)) + " seconds" <<
            std::endl;

            sumTime += entry.second;
    } 
    unthreadedFLOPS = totalOps / sumTime;

    std::cout << fmt::Bold.Wrap(fg::Blue.Wrap("\n<------------------Threaded Results------------------>\n")) << std::endl;

    std::cout << fmt::Bold.Wrap(fg::Green.Wrap("<------------------IOPS------------------>")) << std::endl;
    for (int i = 0; i < threadedResultsIOPS.size(); i++) {
        std::cout << fmt::Bold.Wrap(fg::Green.Wrap(std::to_string(threads[i]) + " threads:")) << std::endl;
        sumTime = 0;
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
        sumTime = 0;
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

    std::cout << fmt::Bold.Wrap(fg::Yellow.Wrap("\n<------------------Summary------------------>\n")) << std::endl;
    // Print averages
    std::cout << fmt::Bold.Wrap(fg::Blue.Wrap("<------------------Unthreaded Results------------------>\n")) << std::endl;

    std::cout << fg::Yellow.Wrap("IOPS: ") << unthreadedIOPS << std::endl;
    std::cout << fmt::Bold.Wrap(fg::Yellow.Wrap("Giga IOPS: ")) << fmt::Bold.Wrap(std::to_string(unthreadedIOPS/1e9)) << std::endl;
    std::cout << fg::Yellow.Wrap("FLOPS: ") << unthreadedFLOPS << std::endl;
    std::cout << fmt::Bold.Wrap(fg::Yellow.Wrap("Giga FLOPS: ")) << fmt::Bold.Wrap(std::to_string(unthreadedFLOPS/1e9)) << std::endl;


    std::cout << fmt::Bold.Wrap(fg::Blue.Wrap("\n<------------------Threaded Results------------------>\n")) << std::endl;
    std::cout << fmt::Bold.Wrap(fg::Green.Wrap("<------------------IOPS------------------>")) << std::endl;

    for (int i = 0; i < threadedIOPSTime.size(); i++) {
        long long x = totalOps/threadedIOPSTime[i];
        threadedIOPS.push_back(x);
        std::cout << fmt::Bold.Wrap(fg::Green.Wrap(std::to_string(threads[i]) + " threads:")) << std::endl;
        std::cout << fg::Yellow.Wrap("IOPS: ") << x << std::endl;
        std::cout << fmt::Bold.Wrap(fg::Yellow.Wrap("Giga IOPS: ")) << fmt::Bold.Wrap(std::to_string(x/1e9)) << std::endl;
    }

    std::cout << std::endl;
    std::cout << fmt::Bold.Wrap(fg::Green.Wrap("<------------------FLOPS------------------>")) << std::endl;

    for (int i = 0; i < threadedFLOPSTime.size(); i++) {
        long long x = totalOps/threadedFLOPSTime[i];
        threadedFLOPS.push_back(x);
        std::cout << fmt::Bold.Wrap(fg::Green.Wrap(std::to_string(threads[i]) + " threads:")) << std::endl;
        std::cout << fg::Yellow.Wrap("FLOPS: ") << x << std::endl;
        std::cout << fmt::Bold.Wrap(fg::Yellow.Wrap("Giga FLOPS: ")) << fmt::Bold.Wrap(std::to_string(x/1e9)) << std::endl;
    }

    writeOutResults(threads, threadedResultsIOPS, threadedIOPS,
                     threadedResultsFLOPS, threadedFLOPS, unthreadedIOPSResults,
                     unthreadedFLOPSResults, unthreadedIOPS, unthreadedFLOPS);
    return 0;
}

