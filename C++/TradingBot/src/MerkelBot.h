#pragma once
#include "OrderBook.h"
#include "Wallet.h"
#include "OrderBookEntry.h"
#include "CSVReader.h"


	
class MerkelBot  
{
	private:

    	std::vector<std::map<std::string , double >> EMAs;
		std::string currentTime;
        OrderBook orderBook{"20200601.csv"};
        Wallet wallet;
		std::string previousTime;
		std::string TimeBeforepreviousTime;
		

	public:
		// empty constructor function
		MerkelBot();
		// this function will be called by main.cpp to start the bots processing
		void BotInit();
		// updates currentTime and matches asks to bids
        void gotoNextTimeFrame();
		// checks if the application can open the orderbook
		void accessOrderBook();
		// return a map of type string and double by taking a timestamp as a parameter
		// this will calculate the Simple moving Average of the bids/asks in orderbook
		std::map<std::string,double> CalculateRates(std::string timestamp);
		// uses an algorithm to calculate the Exponential moving average by taking 
		// a map of type string and double as parameters 
		// This will also decide when to create bids or asks 
		void CalculateEMA(std::map<std::string,double> rates);
		// takes a price , product and timestamp as parameters. 
		// It then creates an orderBookEntry of type bid and inserts the order into the orderBook.
		void GenerateBid(double price,std::string product,std::string timestamp);
		// takes a price , product and timestamp as parameters. 
		// It then creates an orderBookEntry of type ask and inserts the order into the orderBook.
		void GenerateAsks(double price,std::string product,std::string timestamp);
		// this function takes the product and timestamp as parameters .
		// this will look at orders made 2 timestamps back that were unsuccessful and return
		// the amount of the orders back to the wallet by withdrawing the orders.
		void WithdrawOrders(std::string product , std::string timestamp );
		// uses to create log of all bids and asks even if they are unsuccessful
		// takes a filename , product , and timestamp as arguments
		void BidsAndOffersLog(std::string csvFilename ,std::string product , std::string timestamp);
		// keeps track of all assets and updates assets with every timeframe
		// takes a filename as parameter
		void AssetLog(std::string csvFilename);
		// logs successful orders of type bid and ask 
		// takes a filename n product , price , timestamp and orderbookType as parameters
		void SuccessfullOrderLog(std::string csvFilename , std::string product , double price,  std::string timestamp , OrderBookType type);

};

