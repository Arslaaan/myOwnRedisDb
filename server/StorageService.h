#pragma once

#include <proto/dictionary.grpc.pb.h>
#include <unordered_map>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::StatusCode;

namespace dictionary {
class StorageService final : public Storage::Service {
   public:
    Status set(ServerContext* context, const Entry* request,
               Empty* reply) override;

    Status get(ServerContext* context, const Key* request,
               Value* reply) override;

    Status remove(ServerContext* context, const Key* request,
                  Empty* reply) override;

    Status size(ServerContext* context, const Empty* request,
                StorageSizeInfo* reply) override;

   private:
   std::unordered_map<std::string, std::string> gmap;
};
}  // namespace dictionary
