#include <grpc++/grpc++.h>
#include <proto/dictionary.grpc.pb.h>

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

   private:
    std::unique_ptr<dictionary::Storage::Stub> stub_;
};

int main(int argc, char** argv) {
    Client client(grpc::CreateChannel("localhost:50051",
                                      grpc::InsecureChannelCredentials()));
    std::cout << "Starting client" << std::endl;

    std::cout << client.set("world", "weird") << std::endl;
    std::cout << client.set("world", "weir") << std::endl;
    std::cout << client.set("hello", "123") << std::endl;
    std::cout << client.set("hello", "12") << std::endl;
    std::cout << client.remove("hello") << std::endl;
    std::cout << client.set("hello", "12") << std::endl;
    std::cout << client.get("world") << std::endl;
    std::cout << client.get("hello") << std::endl;
    std::cout << client.size() << std::endl;

    return 0;
}