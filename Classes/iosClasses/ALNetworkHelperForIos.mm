//
//  ALNetworkHelperForIos.cpp
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/10.
//


#include "ALNetworkHelperForIos.h"
#include "Reachability.h"


static Reachability* s_reachability = nil;
bool ALNetworkHelperForIos::checkCurrentNetworkState()
{
    if (!s_reachability) {
        s_reachability = [Reachability reachabilityForInternetConnection];
    }
    
    NetworkStatus netStatus = [s_reachability currentReachabilityStatus];
    
    
    return (netStatus != NotReachable);
    
    
//    Reachability *reach = [Reachability reachabilityForInternetConnection];
//    NetworkStatus netStatus = [reach currentReachabilityStatus];
//    if (netStatus == NotReachable) {
//        return false;
//    }else{
//        return true;
//    }
}


