#include <grpc++/grpc++.h>

#include "CallData.h"

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
        gpr_log(GPR_INFO, "Server listening on %s", server_address.c_str());

        // Main loop
        HandleRpcs();
    }

   private:
    void HandleRpcs() {
        // Spawn a new CallData instances to serve new clients.
        initServerCompletionQueue();
        void* tag;  // uniquely identifies a request.
        bool ok;
        while (true) {
            storage_.removeOutdatedEntries();
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
        new KeysCommand(&storage_, cq_.get());
        new PurgeCommand(&storage_, cq_.get());
    }

    std::unique_ptr<grpc::ServerCompletionQueue> cq_;
    dictionary::StorageService storage_;
    std::unique_ptr<Server> server_;
};

int main(int argc, char** argv) {
    // grpc_tracer_set_enabled();
    gpr_set_log_verbosity(GPR_LOG_SEVERITY_DEBUG);

    ServerImpl server;
    server.Run();
    return 0;
}