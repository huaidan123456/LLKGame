//
//  ALNetworkHelperForIos.cpp
//  LLKGame-mobile
//
//  Created by Allen on 2018/1/10.
//


#include "ALNetworkHelperForIos.h"
#include "Reachability.h"

bool ALNetworkHelperForIos::checkCurrentNetworkState()
{
    Reachability *reach = [Reachability reachabilityForInternetConnection];
    NetworkStatus netStatus = [reach currentReachabilityStatus];
    if (netStatus == NotReachable) {
        return false;
    }else{
        return true;
    }
}
