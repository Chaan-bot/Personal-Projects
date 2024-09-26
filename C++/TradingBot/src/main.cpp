#include "Wallet.h"
#include <iostream>
#include "MerkelMain.h"
#include "MerkelBot.h"

int main()
{   
    int option;
    std::cout << "Please select if auto or manual tradin will be used , 1 for manual , 2 for automated "<<std::endl;
    std::cin >> option;

    // select manual trading
    if(option == 1){
        MerkelMain app{};
        app.init();
    }
    // select automated(bot) trading
    else if(option == 2){
        MerkelBot app{};
         std::cout<<"----------------------------------------------"<<std::endl;
         std::cout<<"Calculating ... - this might take up to 20 minutes"<<std::endl;
         std::cout<<"----------------------------------------------"<<std::endl;

        //calling the botInit class which runs the bot 
        app.BotInit();
       
    }
    else{
        std::cout << " Please select a valid option: 1  or  2"<<std::endl;
    }
    
}
