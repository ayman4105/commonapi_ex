/*
 * HelloworldStubImpl.hpp
 *
 * This is YOUR implementation file. You inherit from the generated
 * HelloworldStubDefault class and override the methods you want to implement.
 */

#ifndef HELLOWORLD_STUB_IMPL_HPP
#define HELLOWORLD_STUB_IMPL_HPP

#include <v1/ayman/examples/HelloStubDefault.hpp>
#include <iostream>

class HelloworldStubImpl : public v1::ayman::examples::HelloStubDefault
{
public:
    HelloworldStubImpl()
    {
        std::cout << "[Server] HelloworldStubImpl created" << std::endl;
    }

    virtual ~HelloworldStubImpl()
    {
        std::cout << "[Server] HelloworldStubImpl destroyed" << std::endl;
    }

    /*
     * This method is called when a client invokes SayHello remotely.
     *
     * Parameters:
     *   _client - Identifies which client made this request (for access control, logging)
     *   _name   - The input parameter from the client (what they sent us)
     *   _reply  - A callback function. Call this to send the response back!
     *
     * IMPORTANT: You MUST call _reply() exactly once. If you don't call it,
     * the client will timeout waiting for a response.
     */
    void sayHello(const std::shared_ptr<CommonAPI::ClientId> _client, std::string _name, sayHelloReply_t _reply)
        override
    {

        // Log the request
        std::cout << "[Server] Received SayHello request from client" << std::endl;
        std::cout << "[Server] Input name: " << _name << std::endl;

        // Build the response message
        std::string responseMessage = "Hello " + _name + "! Welcome to CommonAPI SOME/IP!";

        std::cout << "[Server] Sending reply: " << responseMessage << std::endl;

        // Send the reply back to the client via the callback
        // This is what actually transmits the response over SOME/IP
        _reply(responseMessage);
    }
};

#endif // HELLOWORLD_STUB_IMPL_HPP