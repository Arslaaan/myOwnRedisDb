#pragma once

#include <grpcpp/alarm.h>
#include <proto/dictionary.grpc.pb.h>
#include <utils/GprTime.h>

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <chrono>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::StatusCode;

namespace dictionary {

class TTLEntry {
    std::string key_;
    // Time to remove entry
    std::chrono::_V2::system_clock::time_point timestamp_;

   public:
    TTLEntry(const std::string& key,
             std::chrono::_V2::system_clock::time_point timestamp);
    std::chrono::_V2::system_clock::time_point getTimestamp() const;
    const std::string& getKey() const;
    bool operator<(const TTLEntry& other) const;
    bool operator>(const TTLEntry& other) const;
};

class StorageService final : public Storage::AsyncService {
   public:
    StorageService();
    Status Set(ServerContext* context, const Entry* request,
               Empty* reply) override;

    Status Get(ServerContext* context, const Key* request,
               Value* reply) override;

    Status Remove(ServerContext* context, const Key* request,
                  Empty* reply) override;

    Status Size(ServerContext* context, const Empty* request,
                StorageSizeInfo* reply) override;

    Status Purge(ServerContext* context, const Empty* request,
                 Empty* reply) override;

    // Get all keys from storage
    std::vector<Key> getKeys();
    // Make snapshot of storage state to rdb file
    void save();
    // Load snapshot of storage state
    void load();
    // Run snapshot timer if its not started or reset
    void resetTimer();

    bool snapshotEnabled() const;
    // Delete entries with outdated lifetime
    void removeOutdatedEntries();

   private:
    std::unordered_map<std::string, std::string> simpleMap;
    // Every operation with storage updates alarm.
    // If there no events for a "storageIdle" seconds then save a snapshot
    std::unique_ptr<grpc::Alarm> alarm = std::make_unique<grpc::Alarm>();
    // idle time (in seconds) to save a snapshot
    const int storageIdle = 30;
    // Option to enable snapshot
    bool snapshotEnabled_ = true;
    std::priority_queue<TTLEntry, std::vector<TTLEntry>, std::greater<TTLEntry>>
        ttlHeap;
    // Max amount of entries to remove in method removeOutdatedEntries.
    // This is necessary so as not to slow down the process for too long
    int maxEntriesToRemoveForIteration = 200;
};

}  // namespace dictionary
