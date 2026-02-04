# CommonAPI SOME/IP – Step-by-Step Guide (Client & Server)

This README explains **how to build any CommonAPI + SOME/IP project** from zero to working client/server, with **clear steps, examples, multiple methods**, and **ASCII diagrams**.

---

## 1. Big Picture

CommonAPI SOME/IP projects always follow the same pipeline:

```
[FIDL]  -> What the service does
[FDEPL] -> How it maps to SOME/IP
[GEN]   -> Generated glue code
[IMPL]  -> Client & Server logic
[JSON]  -> Runtime / Network config
```

If you respect this order, everything works.

---

## 2. Project Structure

Recommended layout:

```
project/
├── fidl/
│   ├── hello.fidl
│   ├── hello.fdepl
│   └── src-gen/            # auto-generated (DO NOT EDIT)
│
├── SRC/
│   ├── Server.cpp
│   ├── Client.cpp
│   └── HelloStubImpl.hpp
│
├── Config/
│   └── vsomeip.json
│
├── CMakeLists.txt
└── build/
```

---

## 3. Define the Interface (FIDL)

**Purpose:**

* Defines WHAT the service offers
* No network, no IP, no ports

### Example: Multiple Methods

`fidl/hello.fidl`

```fidl
package ayman.examples

interface Hello {
    version { major 1 minor 0 }

    method sayHello {
        in {
            String name
        }
        out {
            String message
        }
    }

    method addNumbers {
        in {
            Int32 a
            Int32 b
        }
        out {
            Int32 sum
        }
    }
}
```

**Key rules:**

* Add as many methods as you want
* Only types and signatures

---

## 4. Map Interface to SOME/IP (FDEPL)

**Purpose:**

* Bind FIDL methods to SOME/IP IDs

`fidl/hello.fdepl`

```fdepl
import "platform:/plugin/org.genivi.commonapi.someip/deployment/CommonAPI-4-SOMEIP_deployment_spec.fdepl"
import "hello.fidl"

define org.genivi.commonapi.someip.deployment for interface ayman.examples.Hello {
    SomeIpServiceID = 0x1234

    method sayHello {
        SomeIpMethodID = 0x0001
        SomeIpReliable = false
    }

    method addNumbers {
        SomeIpMethodID = 0x0002
        SomeIpReliable = true
    }
}

define org.genivi.commonapi.someip.deployment for provider as Service {
    instance ayman.examples.Hello {
        InstanceId = "ayman.examples.justSendHi"
        SomeIpInstanceID = 0x5678
    }
}
```

---

## 5. Generate Code

Run:

```bash
/home/ayman/ITI/fady/VSomeIP/commonapi/commonapi_core_generator/commonapi-core-generator-linux-x86_64 -sk hello.fidl
/home/ayman/ITI/fady/VSomeIP/commonapi/commonapi_someip_generator/commonapi-someip-generator-linux-x86_64 hello.fdepl
```

Generated result:

```
fidl/src-gen/v1/ayman/examples/
├── HelloProxy.hpp
├── HelloStub.hpp
├── HelloSomeIPProxy.cpp
├── HelloStubAdapter.cpp
├── HelloSomeIPDeployment.cpp
```

**Important:**

* Never modify generated files
* Re-generate after any FIDL/FDEPL change

---

## 6. Server Implementation

### 6.1 Stub Implementation

`SRC/HelloStubImpl.hpp`

```cpp
#include <v1/ayman/examples/HelloStub.hpp>

class HelloStubImpl : public v1::ayman::examples::HelloStub {
public:
    void sayHello(
        const std::shared_ptr<CommonAPI::ClientId>,
        std::string name,
        sayHelloReply_t reply
    ) override {
        reply("Hello " + name);
    }

    void addNumbers(
        const std::shared_ptr<CommonAPI::ClientId>,
        int32_t a,
        int32_t b,
        addNumbersReply_t reply
    ) override {
        reply(a + b);
    }
};
```

---

### 6.2 Server Main

`SRC/Server.cpp`

```cpp
#include <CommonAPI/CommonAPI.hpp>
#include "HelloStubImpl.hpp"

int main() {
    auto runtime = CommonAPI::Runtime::get();
    auto service = std::make_shared<HelloStubImpl>();

    runtime->registerService(
        "local",
        "ayman.examples.justSendHi",
        service
    );

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
```

---

## 7. Client Implementation

`SRC/Client.cpp`

```cpp
#include <CommonAPI/CommonAPI.hpp>
#include <v1/ayman/examples/HelloProxy.hpp>

int main() {
    auto runtime = CommonAPI::Runtime::get();

    auto proxy = runtime->buildProxy<
        v1::ayman::examples::HelloProxy
    >("local", "ayman.examples.justSendHi");

    while (!proxy->isAvailable()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    CommonAPI::CallStatus status;
    std::string msg;
    int32_t sum;

    proxy->sayHello("Ayman", status, msg);
    proxy->addNumbers(3, 4, status, sum);
}
```

---

## 8. Runtime Configuration (vsomeip.json)

`Config/vsomeip.json`

```json
{
  "unicast": "127.0.0.1",
  "applications": [
    { "name": "Server", "id": "0x1001" },
    { "name": "Client", "id": "0x1002" }
  ],
  "routing": "Server"
}
```

---

## 9. Build (CMake)

Key points:

* Include src-gen
* Link CommonAPI + SOME/IP + vsomeip

```cmake
include_directories(fidl/src-gen)

target_link_libraries(Server CommonAPI CommonAPI-SomeIP vsomeip3)
```

---

## 10. Communication Flow (ASCII Diagram)

```
CLIENT                          SERVER
  |                                |
  | buildProxy()                   |
  |-------------------------------->|
  |                                |
  | isAvailable()                  |
  |<--------------------------------|
  |                                |
  | sayHello("Ayman")              |
  |-------------------------------->|
  |                                |
  |        "Hello Ayman"           |
  |<--------------------------------|
```

---

## 11. Mental Model (Very Important)

```
FIDL  -> API Contract
FDEPL -> SOME/IP IDs
Stub  -> Server Interface
Proxy -> Client Interface
JSON  -> Network & Runtime
```

---

## 12. Golden Rules

* Never edit src-gen
* Always regenerate after FIDL/FDEPL change
* Domain + InstanceId must match everywhere
* Server must start before client

---

## 13. Next Steps

* Add events
* Add attributes
* Split services
* Move client/server to different machines

You now have a reusable blueprint for **any** CommonAPI SOME/IP project.
