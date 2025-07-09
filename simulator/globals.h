#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <omnetpp.h>
#include <fstream>
#include <string>
#include <map>
#include <random>
#include <tuple>
#include <cstdlib>
#include <math.h>
#include <iostream>

using namespace omnetpp;
using namespace std;

// Constants
#define PREIMAGE_SIZE 32
#define COMMITMENT_BATCH_SIZE 3
#define ENABLE_FEES 1

// Landmark routing configuration
#define ENABLE_LANDMARK_ROUTING true
#define LANDMARKSELECT_RANDOM 0
#define LANDMARKSELECT_HIGHESTDEGREE 1
#define NUM_LANDMARKS 3
#define LANDMARK_SELECTION LANDMARKSELECT_HIGHESTDEGREE

// Message types
#define TRANSACTION_INIT 0
#define INVOICE 1
#define UPDATE_ADD_HTLC 2
#define UPDATE_FULFILL_HTLC 3
#define UPDATE_FAIL_HTLC 4
#define COMMITMENT_SIGNED 5
#define REVOKE_AND_ACK 6
#define PAYMENT_REFUSED 7

// Global structures
extern cTopology *globalTopology;
extern std::map<std::string, std::vector<std::tuple<std::string, double, simtime_t>> > pendingPayments;
extern std::map<std::string, std::map<std::string, std::tuple <double, double, double, int, double, double, cGate*, cGate*> > > nameToPCs;
extern std::map<std::string, std::vector<std::pair<std::string, std::vector<double> > > > adjMatrix;
extern std::vector<std::string> landmarks;

// Global statistics
//extern

#endif
