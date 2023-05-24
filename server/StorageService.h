#pragma once

#include <proto/dictionary.grpc.pb.h>
#include <unordered_map>
#include <fstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/string.hpp>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::StatusCode;

namespace dictionary {
class StorageService final : public Storage::AsyncService {
   public:
    Status Set(ServerContext* context, const Entry* request,
               Empty* reply) override;

    Status Get(ServerContext* context, const Key* request,
               Value* reply) override;

    Status Remove(ServerContext* context, const Key* request,
                  Empty* reply) override;

    Status Size(ServerContext* context, const Empty* request,
                StorageSizeInfo* reply) override;

    void save();
    void load();

   private:
   std::unordered_map<std::string, std::string> stringToStringMap;
};
}  // namespace dictionary
