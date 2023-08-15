#include "StorageService.h"

dictionary::StorageService::StorageService() : Storage::AsyncService() {
    load();
};

Status dictionary::StorageService::Set(ServerContext* context,
                                       const Entry* request, Empty* reply) {
    simpleMap.emplace(request->key().data(), request->value().data());
    if (request->ttl() > 0) {
        ttlHeap.emplace(request->key().data(),
                        std::chrono::system_clock::now() +
                            std::chrono::seconds(request->ttl()));
    }
    return Status::OK;
}

Status dictionary::StorageService::Get(ServerContext* context,
                                       const Key* request, Value* reply) {
    if (!simpleMap.count(request->data())) {
        return Status(StatusCode::NOT_FOUND, "No entry found");
    }
    reply->set_data(simpleMap[request->data()]);
    return Status::OK;
}

Status dictionary::StorageService::Remove(ServerContext* context,
                                          const Key* request, Empty* reply) {
    if (!simpleMap.count(request->data())) {
        return Status(StatusCode::NOT_FOUND, "No entry found");
    }
    simpleMap.erase(request->data());
    return Status::OK;
}

Status dictionary::StorageService::Size(ServerContext* context,
                                        const Empty* request,
                                        StorageSizeInfo* reply) {
    reply->set_size(
        simpleMap.size());  // todo may be incorrect because of types
    return Status::OK;
}

Status dictionary::StorageService::Purge(ServerContext* context,
                                         const Empty* request, Empty* reply) {
    simpleMap.clear();
    ttlHeap = std::priority_queue<TTLEntry, std::vector<TTLEntry>,
                                  std::greater<TTLEntry>>();
    return Status::OK;
}

std::vector<dictionary::Key> dictionary::StorageService::getKeys() {
    std::vector<Key> result;
    result.reserve(simpleMap.size());

    for (const auto& entry : simpleMap) {
        Key key;
        key.set_data(entry.first);
        result.push_back(key);
    }
    return result;
}

void dictionary::StorageService::save() {
    std::ofstream snapshot("snapshot.rdb");  // todo make name include timestamp
    if (snapshot) {
        gpr_log(GPR_INFO, "Saving snapshot");
        cereal::BinaryOutputArchive oarchive(snapshot);
        oarchive(simpleMap);
    }
    snapshot.close();
}

void dictionary::StorageService::load() {
    std::ifstream snapshot("snapshot.rdb");
    if (snapshot) {
        gpr_log(GPR_INFO, "Loading snapshot");

        cereal::BinaryInputArchive oarchive(snapshot);
        oarchive(simpleMap);
    }
    snapshot.close();
}

void dictionary::StorageService::resetTimer() {
    alarm->Cancel();
    alarm->Set(GprTime::timeoutSecondsToDeadline(storageIdle),
               [this](bool isExpired) {
                   if (isExpired) {
                       this->save();
                   }
               });
}

bool dictionary::StorageService::snapshotEnabled() const {
    return snapshotEnabled_;
}

void dictionary::StorageService::removeOutdatedEntries() {
    int count = 0;
    while (!ttlHeap.empty() && count < maxEntriesToRemoveForIteration) {
        const auto& ttlEntry = ttlHeap.top();
        if (ttlEntry.getTimestamp() < std::chrono::system_clock::now()) {
            simpleMap.erase(ttlEntry.getKey());
            ttlHeap.pop();
            count++;
        } else {
            break;
        }
    }
}

dictionary::TTLEntry::TTLEntry(
    const std::string& key,
    std::chrono::_V2::system_clock::time_point timestamp)
    : key_(key), timestamp_(timestamp) {}

std::chrono::_V2::system_clock::time_point dictionary::TTLEntry::getTimestamp()
    const {
    return timestamp_;
}

const std::string& dictionary::TTLEntry::getKey() const { return key_; }

bool dictionary::TTLEntry::operator<(const TTLEntry& other) const {
    return timestamp_ < other.getTimestamp();
}

bool dictionary::TTLEntry::operator>(const TTLEntry& other) const {
    return timestamp_ > other.getTimestamp();
}
