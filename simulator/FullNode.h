#ifndef FULLNODE_H
#define FULLNODE_H

#include <omnetpp.h>
#include <map>
#include <string>
#include <vector>
#include <climits>
#include <queue>
#include <algorithm>

using namespace omnetpp;

class FullNode : public cSimpleModule {
    protected:
        // Protected data structures
        std::map<std::string, std::string> _myPreImages; // paymentHash to preImage
        std::map<std::string, std::string> _myInFlights; // paymentHash to nodeName (who owes me)
        std::map<std::string, std::string> _myPayments; //paymentHash to status (PENDING, COMPLETED, FAILED, or CANCELED)
        std::map<std::string, cMessage *> _myStoredMessages; // paymentHash to baseMsg (for finding reverse path)
        std::map<std::string, cModule*> _senderModules; // paymentHash to Module

        // Landmark routing data structures
        std::map<std::string, std::vector<std::string>> _landmarkPaths; // landmark to path
        std::map<std::string, std::map<std::string, std::vector<std::string>>> _nodeToLandmarkPaths; // node to [landmark to path]

        // Omnetpp functions
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void refreshDisplay() const;
        virtual void finish() override;

        // Routing functions
        virtual std::vector<std::string> getPath(std::map<std::string, std::string> parents, std::string target);
        virtual std::string minDistanceNode(std::map<std::string, double> distances, std::map<std::string, bool> visited);
        virtual std::vector<std::string> dijkstraWeightedShortestPath(std::string src, std::string target, std::map<std::string, std::vector<std::pair<std::string, std::vector<double> > > > graph);

        // Landmark routing functions
        virtual void selectLandmarks();
        virtual void buildLandmarkPaths();
        virtual std::vector<std::string> findPathViaLandmarks(std::string src, std::string dst);
        virtual std::string selectBestLandmark(std::string src, std::string dst);

        // Message handlers
        virtual void initHandler(cMessage *msg);
        virtual void invoiceHandler(cMessage *msg);
        virtual void updateAddHTLCHandler(cMessage *msg);
        virtual void updateFulfillHTLCHandler(cMessage *msg);
        virtual void updateFailHTLCHandler(cMessage *msg);
        virtual void paymentRefusedHandler(cMessage *msg);
        virtual void commitSignedHandler(cMessage *msg);
        virtual void revokeAndAckHandler(cMessage *msg);

        // HTLC senders
        virtual void sendFirstFulfillHTLC(cMessage *htlc, std::string firstHop);
        virtual void sendFirstFailHTLC(cMessage *htlc, std::string firstHop);

        // HTLC committers
        virtual void commitUpdateAddHTLC(cMessage *htlc, std::string neighbor);
        virtual void commitUpdateFulfillHTLC(cMessage *htlc, std::string neighbor);
        virtual void commitUpdateFailHTLC(cMessage *htlc, std::string neighbor);
        virtual void commitHTLC(cMessage *htlc, std::string neighbor);

        // Statistics
        virtual void initPerModuleStatistics();

        // Util functions
        virtual bool tryUpdatePaymentChannel(std::string nodeName, double value, bool increase);
        virtual bool hasCapacityToForward(std::string nodeName, double value);
        virtual bool tryCommitTxOrFail(std::string, bool);
        virtual cMessage* generateInvoice(std::string srcName, double value);
        virtual void setInFlight(cMessage *htlc, std::string nextHop);
        virtual bool isInFlight(cMessage *htlc, std::string nextHop);
        virtual std::vector<cMessage *> getSortedPendingHTLCs(std::vector<cMessage *> HTLCs, std::string neighbor);
        virtual std::string createHTLCId(std::string paymentHash, int htlcType);

    public:
        // Public data structures
        bool _isFirstSelfMessage;
        cTopology *_localTopology;
        int localCommitCounter;
        typedef std::map<std::string, int> RoutingTable;  // neighborName to gateIndex
        RoutingTable rtable;
        std::map<std::string, PaymentChannel> _paymentChannels; // neighborName to PaymentChannel
        std::map<std::string, int> _signals; // myName to signal

        // Statistic-related variables
        int _countCompleted = 0;
        int _countFailed = 0;
        int _countCanceled = 0;
        double _paymentGoodputSent = 0;
        double _paymentGoodputAll = 0;
};

#endif