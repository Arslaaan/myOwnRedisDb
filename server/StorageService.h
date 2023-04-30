#pragma once

#include <proto/dictionary.grpc.pb.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

namespace dictionary {
class StorageService final : public Storage::Service {
    Status set(ServerContext* context, const Entry* request,
               Empty* reply) override;

    Status get(ServerContext* context, const Key* request,
               Value* reply) override;

    Status remove(ServerContext* context, const Key* request,
                  Empty* reply) override;

    Status size(ServerContext* context, const Empty* request,
                StorageSizeInfo* reply) override;
};
}  // namespace dictionary
