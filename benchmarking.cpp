
// To compile in bash "g++ -std=c++17 benchmarking.cpp -o bm -lpthread"
// To run "./bm" 
#include <utility>
#include <thread>
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
#include <chrono> 
#include <sstream>
#include "chalk.h"

using namespace chalk;

// // ALTERNATIVE TESTING - WHILE LOOP 
// void integerOperations(unsigned long long ops) {
//     unsigned long long accumulator = 0;
//     while (accumulator < ops) {
//         accumulator += 1L;
//     }
// }

// void floatingpointOperations(unsigned long long ops) {
//     double long accumulator = 0;
//     while (accumulator < ops) {
//         accumulator += 1.0L;
//     }
// }


void integerOperations(unsigned long long ops) {for (unsigned long long i = 0; i < ops; i+=1L) {}}

void floatingpointOperations(unsigned long long ops) {for (double long i = 0; i < ops; i+=1.0L) {}}



std::vector<std::pair<unsigned long long, double long>> unthreadedTest(const std::vector<unsigned long long> operations, bool iops, bool verbose) {
    if (verbose) {
        std::cout << fmt::Bold.Wrap(fg::Blue.Wrap("\n<------------------Unthreaded Results------------------>\n"));
        if (iops) {
            std::cout << fmt::Bold.Wrap(fg::Green.Wrap("<------------------IOPS------------------>\n"));
        }
        else {
            std::cout << fmt::Bold.Wrap(fg::Green.Wrap("<------------------FLOPS------------------>\n"));
        }
    }

    std::vector<std::pair<unsigned long long, double long>> results;

    for (long i = 0; i < operations.size(); i++) {

// <----------------------------- START TIME -------------------------->
           std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
// <----------------------------- START TIME -------------------------->

         if (iops) {
            integerOperations(operations[i]);
        } 
        else {
            floatingpointOperations(operations[i]);
        }

// <----------------------------- END TIME -------------------------->
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
// <----------------------------- END TIME -------------------------->

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        double long totalSeconds = duration/1e6;

        // Append results of time elapsed for all operations to complete
        std::pair<unsigned long long, double long> testResults = {operations[i], totalSeconds};
        results.push_back(testResults);

        if (verbose) {
            std::cout << fg::Green.Wrap(std::to_string(operations[i])) + " operations : " + fg::Blue.Wrap(std::to_string(totalSeconds)) + " seconds\n";
        }
    }
    return results;
}


// Run threaded tests for all combinations of threads and operations
std::vector<std::vector<std::pair<unsigned long long, double long>>> threadedTest(const std::vector<unsigned long long> operations, const std::vector<long> threads, const bool iops, const bool verbose) {

    if (verbose) {
        std::cout << fmt::Bold.Wrap(fg::Blue.Wrap("\n<------------------Threaded Results------------------>\n"));
        if (iops) {
            std::cout << fmt::Bold.Wrap(fg::Green.Wrap("<------------------IOPS------------------>\n"));
        }
        else {
            std::cout << fmt::Bold.Wrap(fg::Green.Wrap("<------------------FLOPS------------------>\n"));
        }
    }

    // Instantiate return vector
    std::vector< std::vector< std::pair<unsigned long long, double long> >> threadedResults;

    // For all numbers of threads
    for (long i = 0; i < threads.size(); i++) {
        std::vector< std::pair<unsigned long long, double long> > threadResults;

        if (verbose) {
            std::cout << fg::Yellow.Wrap(std::to_string(threads[i]) + " threads") << std::endl;
        }
        // For all numbers of operations
        for (long j = 0; j < operations.size(); j++) {
            std::thread threadList[threads[i]];


// <----------------------------- START TIME -------------------------->
           std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
// <----------------------------- START TIME -------------------------->

            // Assign each thread a share of the workload
            unsigned long long ops = operations[j]/threads[i];
            
            // Start each thread depending on running FLOPS or IOPS test
            for (long k = 0; k < threads[i]; k++ ) {

                if (iops) {
                    threadList[k] = std::thread(integerOperations, ops);
                } 
                else {
                    threadList[k] = std::thread(floatingpointOperations, ops);
                }
            }

            // Wait for all threads to finish
            for (long k = 0; k < threads[i]; k++ ) {
                threadList[k].join();
            }

// <----------------------------- END TIME -------------------------->
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
// <----------------------------- END TIME -------------------------->

            // Calculate time elapsed in seconds
            auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            double long totalSeconds = duration1/1e6;

            // Append results of time elapsed for all operations to complete
            std::pair<unsigned long long, double long> opsTime = {operations[j], totalSeconds};

            if (verbose) {
                std::cout << fg::Green.Wrap(std::to_string(operations[j])) + " operations : " + fg::Blue.Wrap(std::to_string(totalSeconds)) + " seconds\n";
            }

            threadResults.push_back(opsTime);
        }
        threadedResults.push_back(threadResults);
    }
    return threadedResults;
}

void writeOutResults(std::vector<long> &threads,
                    std::vector< std::vector< std::pair<unsigned long long, double long>>> &threadedIOPSResults,
                    std::vector<unsigned long long> &threadedIOPS, 
                    std::vector< std::vector< std::pair<unsigned long long, double long>>> &threadedFLOPSResults, 
                    std::vector<unsigned long long> &threadedFLOPS,
                    std::vector<std::pair<unsigned long long, double long>> &unthreadedIOPSResults,
                    std::vector<std::pair<unsigned long long, double long>> &unthreadedFLOPSResults,
                    unsigned long long unthreadedIOPS,
                    unsigned long long unthreadedFLOPS) 
{

    // Setup output 
    std::ofstream out;
    out.open("results.py");

    out << "threadedIOPSResults = {\n";
    for (long i = 0; i < threads.size(); i++) {
        out << "\t" << std::to_string(threads[i]) << " : [\n";
        for (long j = 0; j < threadedIOPSResults[i].size(); j++) {
            out << "\t\t[ " << std::to_string(threadedIOPSResults[i][j].first) << ", " << std::to_string(threadedIOPSResults[i][j].second) << " ],\n";
        }
        out << "\t],\n";
    }
    out << "}\n\n";

    out << "threadedFLOPSResults = {\n";
    for (long i = 0; i < threads.size(); i++) {
        out << "\t" << std::to_string(threads[i]) << " : [\n";
        for (long j = 0; j < threadedFLOPSResults[i].size(); j++) {
            out << "\t\t[ " << std::to_string(threadedFLOPSResults[i][j].first) << ", " << std::to_string(threadedFLOPSResults[i][j].second) << " ],\n";
        }
        out << "\t],\n";
    }
    out << "}\n\n";

    out << "threadedIOPS = {\n";
    for (long i = 0; i < threads.size(); i++) {
        out << "\t" << std::to_string(threads[i]) << " : ";
        out << threadedIOPS[i] << ",\n";
    }
    out << "}\n\n";

    out << "threadedFLOPS = {\n";
    for (long i = 0; i < threads.size(); i++) {
        out << "\t" << std::to_string(threads[i]) << " : ";
        out << threadedFLOPS[i] << ",\n";
    }
    out << "}\n\n";

    out << "unthreadedIOPSResults = {\n";
    for ( const std::pair<const unsigned long long, double long> &entry : unthreadedIOPSResults ) {
        out << "\t" << std::to_string(entry.first) << " : ";
        out << std::to_string(entry.second) << ",\n";
    } 
    out << "}\n\n";

    out << "unthreadedFLOPSResults = {\n";
    for ( const std::pair<const unsigned long long, double long> &entry : unthreadedFLOPSResults ) {
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

std::pair<bool, bool> handleArgs(int argc, char const *argv[]) {
    // ( default inputs , verbose )
    std::pair<bool, bool> default_verbose = {false, false};
    for (long i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg.compare("--default") == 0) {
            default_verbose.first = true;    
        } 
        else if (arg.compare("--verbose") == 0) {
            default_verbose.second = true;
        }
        else if (arg.compare("-d") == 0) {
            default_verbose.first = true;   
        }
        else if (arg.compare("-v") == 0) {
            default_verbose.second = true;
        } 
        else if (arg.compare("-dv") == 0 || arg.compare("-vd") == 0) {
            default_verbose.first = true; 
            default_verbose.second = true;  
        } 
        else {
            std::cout << fg::Red.Wrap("Invalid options entered: \n\t'-d' or '--default' for default inputs\n\t'-v' or '--verbose' for all results to be printed to console\n");
            exit(0);
        }
    }
    return default_verbose;
}

unsigned long long handleUserInput(std::vector<long> &threads, std::vector<unsigned long long> &operations) {
    unsigned long long totalOps = 0;
    std::string userInput = "";
    std::cout << fg::Blue.Wrap("Enter one or more number of threads to test ( ',' comma delimited; EX: 1,2,4,8 ) :\n");
    std::cin >> userInput;
    std::stringstream ss1(userInput);
    std::cout << "Threads: ";
    for (unsigned long long i; ss1 >> i;) {
        threads.push_back(i);    
        std::cout << i <<",";
        if (ss1.peek() == ',')
            ss1.ignore();
    }
    userInput = "";
    std::cout << fg::Blue.Wrap("\nEnter one or more number of operations to test ( ',' comma delimited; EX: 1000,10000,50000,100000 ) :\n");
    std::cin >> userInput;
    std::stringstream ss2(userInput);
    for (unsigned long long i; ss2 >> i;) {
        operations.push_back(i);   
        totalOps += i;
        if (ss2.peek() == ',')
            ss2.ignore();
    }
    std::cout << "Total Operations: " << totalOps << std::endl; 
    return totalOps;
}

int main(int argc, char const *argv[]) {

    // Define tests inputs: Number of threads running concurrently / For how many operations
    std::vector<long> threads;
    std::vector<unsigned long long> operations;   
    unsigned long long totalOps = 0;

    std::pair<bool, bool> default_verbose = handleArgs(argc, argv);
    bool useDefaults = default_verbose.first;
    bool verbose = default_verbose.second;


    if (useDefaults) {
    // <------------DEFAULT VALUES------------->
        threads = {1, 2, 4, 8};
        for (unsigned long long g = (unsigned long long)1e8; g <=(unsigned long long)5e8; g+=(unsigned long long)1e8) {
            operations.push_back(g);
            totalOps += g;
        }
    // ^------------DEFAULT VALUES-------------^
    }
    else {
        totalOps = handleUserInput(threads, operations);
    }

    if (verbose) {
        std::cout << fmt::Bold.Wrap(fg::Yellow.Wrap("\n<------------------Detailed Results------------------>\n"));
    }

    // Run unthreaded tests for IOPS and FLOPS
    std::vector<std::pair<unsigned long long, double long>> unthreadedIOPSResults = unthreadedTest(operations, true, verbose);
    std::vector<std::pair<unsigned long long, double long>> unthreadedFLOPSResults = unthreadedTest(operations, false, verbose);



    // Run threaded tests for IOPS and FLOPS
    std::vector< std::vector< std::pair<unsigned long long, double long> >> threadedResultsIOPS = threadedTest(operations, threads, true, verbose);
    std::vector< std::vector< std::pair<unsigned long long, double long> >> threadedResultsFLOPS = threadedTest(operations, threads, false, verbose);

    unsigned long long unthreadedIOPS, unthreadedFLOPS;
    double long sumTime = 0;
    std::vector<double long> threadedIOPSTime;
    std::vector<double long> threadedFLOPSTime;

    for ( const std::pair<const unsigned long long, double long> &entry : unthreadedIOPSResults ) {
       sumTime += entry.second;
    } 
    unthreadedIOPS = totalOps / sumTime;

    sumTime = 0;

    for ( const std::pair<const unsigned long long, double long> &entry : unthreadedFLOPSResults ) {
        sumTime += entry.second;
    } 
    unthreadedFLOPS = totalOps / sumTime;


    for (long i = 0; i < threadedResultsIOPS.size(); i++) {
        sumTime = 0;
        for (long j = 0; j < threadedResultsIOPS[i].size(); j++) {
            sumTime += threadedResultsIOPS[i][j].second;
        }
        threadedIOPSTime.push_back(sumTime);
    }

    for (long i = 0; i < threadedResultsFLOPS.size(); i++) {
        sumTime = 0;
        for (long j = 0; j < threadedResultsFLOPS[i].size(); j++) {
            sumTime += threadedResultsFLOPS[i][j].second;
        }
        threadedFLOPSTime.push_back(sumTime);
    }

    std::vector<unsigned long long> threadedIOPS;
    std::vector<unsigned long long> threadedFLOPS;

    std::cout << fmt::Bold.Wrap(fg::Yellow.Wrap("\n<------------------Summary------------------>\n")) << std::endl;
    // Prlong averages
    std::cout << fmt::Bold.Wrap(fg::Blue.Wrap("<------------------Unthreaded Results------------------>\n")) << std::endl;

    std::cout << fg::Yellow.Wrap("IOPS: ") << unthreadedIOPS << std::endl;
    std::cout << fmt::Bold.Wrap(fg::Yellow.Wrap("Giga IOPS: ")) << fmt::Bold.Wrap(std::to_string(unthreadedIOPS/1e9)) << std::endl;
    std::cout << fg::Yellow.Wrap("FLOPS: ") << unthreadedFLOPS << std::endl;
    std::cout << fmt::Bold.Wrap(fg::Yellow.Wrap("Giga FLOPS: ")) << fmt::Bold.Wrap(std::to_string(unthreadedFLOPS/1e9)) << std::endl;


    std::cout << fmt::Bold.Wrap(fg::Blue.Wrap("\n<------------------Threaded Results------------------>\n")) << std::endl;
    std::cout << fmt::Bold.Wrap(fg::Green.Wrap("<------------------IOPS------------------>")) << std::endl;

    for (long i = 0; i < threadedIOPSTime.size(); i++) {
        unsigned long long x = totalOps/threadedIOPSTime[i];
        threadedIOPS.push_back(x);
        std::cout << fmt::Bold.Wrap(fg::Green.Wrap(std::to_string(threads[i]) + " threads:")) << std::endl;
        std::cout << fg::Yellow.Wrap("IOPS: ") << x << std::endl;
        std::cout << fmt::Bold.Wrap(fg::Yellow.Wrap("Giga IOPS: ")) << fmt::Bold.Wrap(std::to_string(x/1e9)) << std::endl;
    }

    std::cout << std::endl;
    std::cout << fmt::Bold.Wrap(fg::Green.Wrap("<------------------FLOPS------------------>")) << std::endl;

    for (long i = 0; i < threadedFLOPSTime.size(); i++) {
        unsigned long long x = totalOps/threadedFLOPSTime[i];
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

