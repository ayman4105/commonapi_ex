#include <iostream>
#include <thread>
#include <chrono>

#include <CommonAPI/CommonAPI.hpp>
#include <HelloworldStubImpl.hpp>

int main()
{

    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();
    
    if (!runtime) {
        std::cerr << "[Server] ERROR: Failed to get CommonAPI runtime!" << std::endl;
        return 1;
    }
    std::cout << "[Server] Got CommonAPI runtime" << std::endl;

    std::shared_ptr<HelloworldStubImpl> myService = 
        std::make_shared<HelloworldStubImpl>();
    
    std::cout << "[Server] Created service implementation" << std::endl;

    std::string domain = "local";
    std::string instance = "ayman.examples.justSendHi";  // Must match fdepl!
    
    bool registered = runtime->registerService(domain, instance, myService);

    if (!registered) {
        std::cerr << "[Server] ERROR: Failed to register service!" << std::endl;
        std::cerr << "[Server] Check that the instance ID matches your .fdepl file" << std::endl;
        return 1;
    }

     std::cout << "[Server] Service registered successfully!" << std::endl;
    std::cout << "[Server] Domain: " << domain << std::endl;
    std::cout << "[Server] Instance: " << instance << std::endl;
    std::cout << std::endl;
    std::cout << "[Server] Waiting for client requests..." << std::endl;

     while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        std::cout << "[Server] Still running..." << std::endl;
    }


    return 0;
}
