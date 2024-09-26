#include "MerkelBot.h" 
#include <fstream>
#include <iostream>
#include <stdio.h>
	
MerkelBot::MerkelBot()
{
	
}

void MerkelBot::BotInit()
{
    // below i am using the wallet class that is unchanged to insert currencies for my bot to trade with.
    // i am using the insertCurrency function that can be found in the Wallet class
    wallet.insertCurrency("ETH", 10);
    wallet.insertCurrency("DOGE", 10);
    wallet.insertCurrency("USDT", 10);
    wallet.insertCurrency("BTC", 10);

    // setting the currentTime to the earliest value of time that the orderbook can retrieve
    // Using the orderBook class and the function getEarliestTime() inside the orderBook class 
    currentTime = orderBook.getEarliestTime();
    
    // this while loop will loop through time until the next time is equal to the first time of the orderbook
    // I am using the function getNextTime from the orderBook class and parsing the currentTime which initially will
    // be the earliest/first time of the orderBook
    while( orderBook.getNextTime(currentTime)!=orderBook.getEarliestTime()){

        // this function is called to calculate the rates at the currentTime and then using the aquired rates
        // to determine the Exponential moving Average , which then makes decisions for the bot eg. creating a bid
        // ,withrawing a order and creating an ask
        CalculateEMA(CalculateRates(currentTime));
        AssetLog("AssetLog.csv");
        // this function is called to move current time to the next time and matching asks and bids that the 
        // bot has made at the currentTime  
        gotoNextTimeFrame();
    }
    
    std::cout << wallet.toString() << std::endl;
}

void MerkelBot::accessOrderBook(){

std::string csvFilename{"20200601.csv"};
std::ifstream csvFile{csvFilename};
    // logical statement that is used to check if the file has opened successfully
    if (csvFile.is_open()){
        std::cout<<"File opened successful ! "<<std::endl;
    }
    else{
        std::cout<<"File opened Unsuccessful ! "<<std::endl;
    }
}

void MerkelBot::gotoNextTimeFrame(){

    // a for loop that loops through all the products of the orderBook by using 
    // the function getKnownProducts that can be found in the orderBook class
    for (std::string p : orderBook.getKnownProducts())
    {
        // create a vector of type OrderBookEntry and use the function matchAsksToBid  
        // which can be found in the orderBook class which will take the currentTime and product as parameters
        std::vector<OrderBookEntry> sales =  orderBook.matchAsksToBids(p, currentTime);

        // A for loop that loops through all the sales
        for (OrderBookEntry& sale : sales)
        {
            // logical statement that checks that the OrderBookEntry parameter 'username' is equal to simuser
            if (sale.username == "simuser")
            {
                // update the wallet after aks and bids have been performed
                wallet.processSale(sale);
            }
        }  
    }
    // create a time 2 timestamps back
    TimeBeforepreviousTime=previousTime;
    // time 1 timestamp back
    previousTime=currentTime;
    // set the currentTime to the next time in the orderBook by using the function getNextTime
    // which can be found in the orderBook Class
    currentTime = orderBook.getNextTime(currentTime);
}

std::map<std::string,double> MerkelBot::CalculateRates(std::string timestamp){

    // create a map variable that takes the product as a string variable and
    // double as the price variable
   std::map<std::string,double> rates;
    // create a vector of type string     
   std::vector<std::string> products;
    // set the vector of type string equal to the products of the orderbook 
    // by using the getKnownProducts function from the orderBook Class. 
   products = orderBook.getKnownProducts();
    
    // create a for loop that loops through all the product of the OrderBook
    for(std::string const&  p : products){

    // create a vector of type OrderBookEntry
    std::vector<OrderBookEntry> asks;

    // set the vector of type OrderBookEntry equal to the function getOrders
    // which can be found in the OrderBook class 
    // The hetOrders function takes a OrderBookType , product and std::string timestamp 
    // as parameters
    asks =orderBook.getOrders(OrderBookType::ask,p,timestamp);

    // setting a variable of type double to 0
    double sum=0;

    // setting a variable of type int to 0
    int count = 0;

    // loop throough the vector of type OrderBookEntry asks
    for(OrderBookEntry& ask :asks){
        // adds all the prices of all asks with the same product name and timestamp
        sum += ask.price;
        // increment integer every time all asks of a certain time and product has been processed
        count++;
    }
    // set a variable of type double equal to the sum divided by the count (This is the STANDARD MOVING AVERAGE)
    double SMA = sum/count;
    // set the double value of the map variable rates equal to the SMA
    rates[p]=SMA;

    }
    // return map of all SMA's with their corresponding products 
    return rates;
}

void MerkelBot::CalculateEMA(std::map<std::string,double> rates){

    // set variable of type double equal to 0
    double counter=0;

    // set variable of type double equal to 2 divided by the counter plus one 
    // this variable will be used to help calculate the exponential moving average
    double smoothingAverage = (2/(counter+1));

    // create a variable of type map that takes type string and double
    std::map<std::string , double> EMAtoday;

    // looping through all the rates that are returned by the CalculateRates function
    for(auto const& rate: rates){

        // setting a double variable equal to the price values returned by Rates through the CalculateRates function
        double EMAt = rate.second;
        
        // logical statement that checks if The EMA vector  (which is a vector of type map that takes string and double as types)
        // is not equal to 0 and continues if this is true
        if(EMAs.size()!=0){

            // logical statement that checks if the current EMA has a double value larger than 0
            if(EMAs[EMAs.size()-1][rate.first]>0){

                // set a variable of type doouble equal to the value of the previous EMA's value 
                double lastEMA = EMAs[EMAs.size()-1][rate.first];

                // sets a variable of type double equal to the Exponential Moving Average equation
                EMAt = (EMAt*smoothingAverage)+(lastEMA*(1-smoothingAverage));
            }
        }

        // sets the double value of the MAP variable egual to the new EMA
        EMAtoday[rate.first]=EMAt;
    
    }

    // adds the calculated EMA to the EMAs vector 
    EMAs.push_back(EMAtoday);
       
    // logical statement that is used to see if the EMAs vector has more than 10 elements
    // it then erases the element at the neginning as only a few EMAs are applicable after a certain timespace
    if(EMAs.size() > 10){

        EMAs.erase(EMAs.begin());

        // increment the counter every time a variable is removed from he vector 
        // this counter variable is used for the smoothingAverage
        counter+=0.2;
    }

    // logical statement to see if the EMAs vector has more then 3 elements
    if(EMAs.size()>5){
       
        // set variable of type map that takes type string and double
        // this variable is used to look at the EMA 2 timestamps back 
        std::map<std::string ,double> prev_1EMA = EMAs[EMAs.size()-3];

        // set variable of type map that takes type string and double
        // this variable is used to look at the EMA 1 timestamps back
        std::map<std::string ,double> previousEMA = EMAs[EMAs.size()-2];

        // set variable of type map that takes type string and double
        // this variable is used to look at the current timestamp
        std::map<std::string ,double> currentEMA = EMAs[EMAs.size()-1];

        // a for loop that loops through each products previous EMA
        for(auto const& eachProd : previousEMA){

            // calls the withdrawOrders function on all products 2 timestamps back
            WithdrawOrders(eachProd.first, TimeBeforepreviousTime);

            // creates a log of all bids and asks one timestamp back
            BidsAndOffersLog("BidsAndAsks.csv",eachProd.first,previousTime);
            
            // logical statement that checks if the EMA value of 2 timestamps back is larger then the previous timestamp's EMA 
            // AND if the previous timestamps EMA is less then the current EMA
            // AND if the EMA of 2 timestamps back is less then the Current EMA
            // This means that the price went down but went back up more than it went down previously therefore being a good time to sell 
            if(prev_1EMA[eachProd.first] < currentEMA[eachProd.first] && (prev_1EMA[eachProd.first] > eachProd.second) && (eachProd.second  < currentEMA[eachProd.first])){
               
                GenerateAsks(currentEMA[eachProd.first],eachProd.first,currentTime);
            }
               
            // logical statement that checks if the EMA value of 2 timestamps back is smaller then the previous timestamp's EMA 
            // AND if the previous timestamps EMA is more then the current EMA
            // AND if the EMA of 2 timestamps back is less then the Current EMA
            // This means the price went up and then decreased again but it did not decrease more than what it was 2 timestamps back therefore being a good time to buy
            if(( prev_1EMA[eachProd.first] <  eachProd.second) && (prev_1EMA[eachProd.first] < currentEMA[eachProd.first])    &&  (eachProd.second  > currentEMA[eachProd.first]) ){
            
                GenerateBid(eachProd.second, eachProd.first,currentTime);
            }
        }
    }
}
    
void MerkelBot::GenerateBid(double price,std::string product,std::string timestamp){

// create an OrderBookEntry that takes 3 parameters from the function call
// the amount is set to 1000 because the value will decrease until the required amount is met
// the price is multipled by two because then the bot will be able to get higher prioity when buying
    OrderBookEntry obe{price*2 , 200 , timestamp , product  ,OrderBookType::bid ,"simuser"};

        while (true)
        {
            // logical statement that uses a function called canFulfillOrder which can be found in the Wallet class 
            // this function takes the created OrderbookEntry as a parameter
            if (wallet.canFulfillOrder(obe)){

                // inserts the created OrderbookEntry into the OrderBook Vector
                orderBook.insertOrder(obe);
                SuccessfullOrderLog("SuccessOrders.csv", product , price  , timestamp, OrderBookType::bid);
                break;
            }
            
            else{
                // the following logical statement changes the amount of the created bid to match the ask in the orderBook
                if(obe.amount > 0 ){
                    obe.amount = obe.amount/2;
                }
                // this logic will be used when the wallet has insufficient funds
                else {
                    obe.amount = 0;
                    break;
                }    
            } 
        }
}

void MerkelBot::GenerateAsks(double price,std::string product,std::string timestamp){

    // create an OrderBookEntry that takes 3 parameters from the function call
    // the amount is set to 0.3 because it is the most optimal calculated amount by me
    // the price is one percent less then the full price so that offers are taken more frequently
    OrderBookEntry obe{price*0.99, 0.3, timestamp , product  ,OrderBookType::ask ,"simuser"};
        while (true)
        {   
            // logical statement that uses a function called canFulfillOrder which can be found in the Wallet class 
            // this function takes the created OrderbookEntry as a parameter
            if (wallet.canFulfillOrder(obe)){ 

                // inserts the created OrderbookEntry into the OrderBook Vector        
                orderBook.insertOrder(obe);
                SuccessfullOrderLog("SuccessOrders.csv", product , price*0.99 ,  timestamp, OrderBookType::ask);
                
                break;
            }
            // this logic will be used when the wallet has insufficient funds 
            else{  
                   
                obe.amount = 0;
                break;
                    
            } 
        }
}

void MerkelBot::WithdrawOrders(std::string product , std::string timestamp){
    
    // create variable of type orderbookEntry
    std::vector<OrderBookEntry> PrevBids;
    // create variable of type orderbookEntry
    std::vector<OrderBookEntry> PrevAsks;

    // loops through the orders of both asks and bids
    PrevAsks =orderBook.getOrders(OrderBookType::ask, product ,timestamp);
    PrevBids =orderBook.getOrders(OrderBookType::bid, product ,timestamp);

    // loops through all orders of type ask
    for (OrderBookEntry& asks : PrevAsks){

        // loops through all orders of type bid
        for(OrderBookEntry& bids : PrevBids){

            // checks if the simuser created the bids
            if(bids.username=="simuser"){

                // makes use of tokenise function to return the first product eg. ETH/BTC will return ETH
                std::vector<std::string> currsBids = CSVReader::tokenise(bids.product, '/');

                // logical statement checking if bid prices were lower than ask prices
                if( bids.price < asks.price){

                    std::cout<<"WITHDRAWING BID"<<std::endl;
                    std::string currency = currsBids[1];
                    // update the wallet
                    wallet.insertCurrency(currency, bids.amount);
                    // remove the bid prom the orderbook
                    PrevBids.pop_back();
                    break;
                }
            }
            // checks if the simuser created the asks
            if(asks.username=="simuser"){

                // makes use of tokenise function to return the first product eg. ETH/BTC will return ETH
                std::vector<std::string> currsAsks = CSVReader::tokenise(asks.product, '/');

                // logical statement checking if ask prices were greater than the Highest bid price or if 
                // the ask price is lower then the lowest ask price
                if( asks.price > orderBook.getHighPrice(PrevBids) || asks.price < orderBook.getLowPrice(PrevBids) ){
                    
                    std::string currency = currsAsks[0];
                    // update the wallet
                    wallet.insertCurrency(currency, asks.amount);
                    // remove the ask from the orderbook
                    PrevAsks.pop_back();
                    break;

                }
            }
        } 
    }
} 
        
void MerkelBot::BidsAndOffersLog(std::string csvFilename ,std::string product , std::string timestamp){

    // create variable of type orderbookEntry
    std::vector<OrderBookEntry> PrevBids;
    // create variable of type orderbookEntry
    std::vector<OrderBookEntry> PrevAsks;
    // loops through the orders of both asks and bids
    PrevAsks =orderBook.getOrders(OrderBookType::ask, product ,timestamp);
    PrevBids =orderBook.getOrders(OrderBookType::bid, product ,timestamp);

        // loops through all bids of previous bids
        for(OrderBookEntry& bids : PrevBids){

            // checks if the simuser created the bids
            if(bids.username=="simuser"){

                // create an output stream to the designated file
                std::ofstream csvFile{csvFilename , std::ios::app };

                // checks if the file is open
                if (csvFile.is_open())
                {
                    csvFile <<" Bid "<< " product : "<< product <<" time : " << timestamp <<" price : "<< bids.price << std::endl;     
                }
             }         
        }
        // loops through all asks of previous asks
        for (OrderBookEntry& asks : PrevAsks){

            // checks if the simuser created the asks
            if(asks.username=="simuser"){
                
                // create an output stream to the designated file
                std::ofstream csvFile{csvFilename  , std::ios::app };
                
                // checks if the file is open
                if (csvFile.is_open())
                    {
                    csvFile <<" Ask "<< " product : "<< product <<" time : " << timestamp <<" price : "<< asks.price << std::endl;
                    }
                }
        }
 } 

void MerkelBot::AssetLog(std::string csvFilename ){

    // create an output stream to the designated file
    std::ofstream csvFile{csvFilename  };
        // checks if the file is open
        if (csvFile.is_open())
         {
            // prints the wallet to the assetLog.csv and updates with every timeframe
            csvFile << wallet.toString() << std::endl;
         }
}

void MerkelBot::SuccessfullOrderLog(std::string csvFilename , std::string product , double price,  std::string timestamp , OrderBookType type){

        // see if orderbooktype is a ask
        if(type ==OrderBookType::ask){
            std::ofstream csvAskFile{csvFilename , std::ios::app };
            // checks if the file is open
            if (csvAskFile.is_open())
            {
                // prints the wallet to the assetLog.csv and updates with every timeframe
                csvAskFile << "Successful Ask : - " << "Product : - " << product << "Time : - "<< timestamp << "Price : - " <<price  << std::endl;
            }
        }
        // see if orderbooktype is a bid
        if(type==OrderBookType::bid){

            std::ofstream csvBidFile{csvFilename , std::ios::app };
            // checks if the file is open
            if (csvBidFile.is_open())
            {
                // prints the wallet to the assetLog.csv and updates with every timeframe
                csvBidFile << "Successful Bid : - " << "Product : - " << product << "Time : - "<< timestamp << "Price : - " <<price  << std::endl;
            }
        }
}