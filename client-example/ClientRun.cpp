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

        Status status = stub_->set(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
            return "OK";
        } else {
            return "RPC failed";
        }
    }

   private:
    std::unique_ptr<dictionary::Storage::Stub> stub_;
};

int main(int argc, char** argv) {
    Client client(grpc::CreateChannel("localhost:50051",
                                      grpc::InsecureChannelCredentials()));
    std::string reply = client.set("world", "weird");
    std::cout << reply << std::endl;

    return 0;
}