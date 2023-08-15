#include <grpc++/grpc++.h>
#include <proto/dictionary.grpc.pb.h>

#include <chrono>
#include <thread>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class Client {
   public:
    Client(std::shared_ptr<Channel> channel)
        : stub_(dictionary::Storage::NewStub(channel)) {}

    std::string set(const std::string& key, const std::string& value) {
        dictionary::Entry request;
        request.mutable_key()->set_data(key);
        request.mutable_value()->set_data(value);

        dictionary::Empty reply;

        ClientContext context;

        Status status = stub_->Set(&context, request, &reply);

        if (status.ok()) {
            return "OK";
        } else {
            return status.error_message();
        }
    }

    std::string setWithTTL(const std::string& key, const std::string& value,
                           int32_t ttl) {
        dictionary::Entry request;
        request.mutable_key()->set_data(key);
        request.mutable_value()->set_data(value);
        request.set_ttl(ttl);

        dictionary::Empty reply;

        ClientContext context;

        Status status = stub_->Set(&context, request, &reply);

        if (status.ok()) {
            return "OK";
        } else {
            return status.error_message();
        }
    }

    std::string get(const std::string& key) {
        dictionary::Key request;
        request.set_data(key);

        dictionary::Value reply;
        ClientContext context;

        Status status = stub_->Get(&context, request, &reply);

        if (status.ok()) {
            return reply.data();
        } else {
            return status.error_message();
        }
    }

    std::string remove(const std::string& key) {
        dictionary::Key request;
        request.set_data(key);

        dictionary::Empty reply;
        ClientContext context;

        Status status = stub_->Remove(&context, request, &reply);

        if (status.ok()) {
            return "OK";
        } else {
            return status.error_message();
        }
    }

    std::string size() {
        dictionary::Empty request;

        dictionary::StorageSizeInfo reply;
        ClientContext context;

        Status status = stub_->Size(&context, request, &reply);

        if (status.ok()) {
            return std::to_string(reply.size());
        } else {
            return status.error_message();
        }
    }

    std::string purge() {
        dictionary::Empty request;

        dictionary::Empty reply;
        ClientContext context;

        Status status = stub_->Purge(&context, request, &reply);

        if (status.ok()) {
            return "PURGED";
        } else {
            return status.error_message();
        }
    }

    void keys() {
        dictionary::Empty request;

        dictionary::Key reply;
        ClientContext context;

        auto responder = stub_->Keys(&context, request);

        std::cout << "KEYS:" << std::endl;
        while(responder->Read(&reply)) {
            std::cout << reply.data() << std::endl;
        }
        std::cout << "KEYS END" << std::endl;
    }

   private:
    std::unique_ptr<dictionary::Storage::Stub> stub_;
};

void testCommon(Client& client) {
    std::cout << client.set("world", "weird") << std::endl;
    std::cout << client.set("hello", "123") << std::endl;
    std::cout << client.remove("hello") << std::endl;
    std::cout << client.get("world") << std::endl;
    std::cout << client.get("hello") << std::endl;
    std::cout << client.size() << std::endl;
}

void testTtl(Client& client) {
    std::cout << client.purge() << std::endl;
    client.setWithTTL("QWE", "something", 5);
    client.setWithTTL("ASD", "something", 5);
    client.setWithTTL("ZXC", "something", 15);
    if (client.size() == "3") {
        std::this_thread::sleep_for(std::chrono::seconds(6));
        if (client.size() == "1") {
            std::cout << "OK" << std::endl;
            return;
        }
    }
    std::cout << "FAIL" << std::endl;
}

void testKeys(Client& client) {
    std::cout << client.purge() << std::endl;
    client.set("QWE", "something");
    client.set("ASD", "something");
    client.set("ZXC", "something");
    client.keys();
}

int main(int argc, char** argv) {
    Client client(grpc::CreateChannel("localhost:50051",
                                      grpc::InsecureChannelCredentials()));
    std::cout << "Starting client" << std::endl;
    testKeys(client);
    testTtl(client);
    return 0;
}