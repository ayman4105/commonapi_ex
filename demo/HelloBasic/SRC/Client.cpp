#include <iostream>
#include <thread>
#include <chrono>

// CommonAPI runtime - the core library
#include <CommonAPI/CommonAPI.hpp>

// The generated Proxy class - this is what we use to call the service
#include "v1/ayman/examples/HelloProxy.hpp"

int main()
{
    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();
    
    if (!runtime) {
        std::cerr << "[Client] ERROR: Failed to get CommonAPI runtime!" << std::endl;
        return 1;
    }

    std::cout << "[Client] Got CommonAPI runtime" << std::endl;

    std::string domain = "local";
    std::string instance = "ayman.examples.justSendHi";  // Must match server!

    std::shared_ptr<v1::ayman::examples::HelloProxy<>> proxy = runtime->buildProxy<v1::ayman::examples::HelloProxy>(domain,instance);

     if (!proxy) {
        std::cerr << "[Client] ERROR: Failed to build proxy!" << std::endl;
        return 1;
    }
    std::cout << "[Client] Proxy created" << std::endl;


    std::cout << "[Client] Waiting for service to become available..." << std::endl;

    int waitSeconds = 0;
    while (!proxy->isAvailable()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        waitSeconds++;
        std::cout << "[Client] Waiting... (" << waitSeconds << "s)" << std::endl;
        
        if (waitSeconds > 30) {
            std::cerr << "[Client] ERROR: Service not available after 30 seconds!" << std::endl;
            std::cerr << "[Client] Is the server running?" << std::endl;
            return 1;
        }
    }
    
    std::cout << "[Client] Service is available!" << std::endl;

    std::cout << "[Client] Calling SayHello(\"ayman\")..." << std::endl;
    
    CommonAPI::CallStatus callStatus;
    std::string returnMessage;

    proxy->sayHello("ayman" , callStatus , returnMessage);

    if(callStatus == CommonAPI::CallStatus::SUCCESS){
        std::cout << "[Client] SUCCESS!" << std::endl;
        std::cout << "[Client] Server replied: " << returnMessage << std::endl;
    }
    else{
        std::cerr << "[Client] Call failed with status: " << static_cast<int>(callStatus) << std::endl;
    }

    std::cout << "[Client] Calling SayHello ASYNC (\"Async User\")..." << std::endl;

    auto asyncCallback = [](const CommonAPI::CallStatus& status, const std::string& message) {
        if (status == CommonAPI::CallStatus::SUCCESS) {
            std::cout << "[Client] ASYNC callback received: " << message << std::endl;
        } else {
            std::cerr << "[Client] ASYNC call failed!" << std::endl;
        }
    };
    
    // This returns immediately - does NOT wait for response
    std::future<CommonAPI::CallStatus> future = 
        proxy->sayHelloAsync("Async User", asyncCallback);
    
    std::cout << "[Client] Async call sent, continuing execution..." << std::endl;
    std::cout << "[Client] Waiting for async response..." << std::endl;
    
    // Wait for the async call to complete (optional - you might do other work here)
    std::this_thread::sleep_for(std::chrono::seconds(2));
    


    return 0;
}
