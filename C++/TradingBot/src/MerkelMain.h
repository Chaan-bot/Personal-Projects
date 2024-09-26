#pragma once
#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"


class MerkelMain
{
    public:
        MerkelMain();
        /** Call this to start the sim */
        // add a BotInit to switch between manual and automatic trading 
        void init();
    private: 
        void printMenu();
        void printHelp();
        void printMarketStats();
        void enterAsk();
        void enterBid();
        void printWallet();
        void gotoNextTimeframe();
        int getUserOption();
        void processUserOption(int userOption);

        std::string currentTime;

        OrderBook orderBook{"empty.csv"};

        Wallet wallet;

};
