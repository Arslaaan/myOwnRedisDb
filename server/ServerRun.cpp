#include <grpc++/grpc++.h>

#include "GetCommand.h"
#include "CallData/SetCommand.h"
#include "CallData/RemoveCommand.h"
#include "CallData/SizeCommand.h"

class ServerImpl final {
   public:
    ~ServerImpl() {
        server_->Shutdown();
        cq_->Shutdown();
    }

    void Run() {
        std::string server_address = "0.0.0.0:50051";

        ServerBuilder builder;
        builder.AddListeningPort(server_address,
                                 grpc::InsecureServerCredentials());
        builder.RegisterService(&storage_);
        cq_ = builder.AddCompletionQueue();
        server_ = builder.BuildAndStart();
        std::cout << "Server listening on " << server_address << std::endl;

        // Main Loop
        HandleRpcs();
    }

   private:
    void HandleRpcs() {
        // Spawn a new CallData instance to serve new clients.
        initServerCompletionQueue();
        void* tag;  // uniquely identifies a request.
        bool ok;
        while (true) {
            GPR_ASSERT(cq_->Next(&tag, &ok));
            GPR_ASSERT(ok);
            static_cast<CallDataInterface*>(tag)->proceed();
        }
    }

    void initServerCompletionQueue() {
        new GetCommand(&storage_, cq_.get());
        new SetCommand(&storage_, cq_.get());
        new RemoveCommand(&storage_, cq_.get());
        new SizeCommand(&storage_, cq_.get());
    }

    std::unique_ptr<grpc::ServerCompletionQueue> cq_;
    dictionary::StorageService storage_;
    std::unique_ptr<Server> server_;
};

int main(int argc, char** argv) {
    ServerImpl server;
    server.Run();
    return 0;
}