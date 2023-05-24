#include "StorageService.h"

Status dictionary::StorageService::Set(ServerContext* context,
                                       const Entry* request, Empty* reply) {
    stringToStringMap.emplace(request->key().data(), request->value().data());
    return Status::OK;
}

Status dictionary::StorageService::Get(ServerContext* context,
                                       const Key* request, Value* reply) {
    if (!stringToStringMap.count(request->data())) {
        return Status(StatusCode::NOT_FOUND, "No entry found");
    }
    reply->set_data(stringToStringMap[request->data()]);
    return Status::OK;
}

Status dictionary::StorageService::Remove(ServerContext* context,
                                          const Key* request, Empty* reply) {
    if (!stringToStringMap.count(request->data())) {
        return Status(StatusCode::NOT_FOUND, "No entry found");
    }
    stringToStringMap.erase(request->data());
    return Status::OK;
}

Status dictionary::StorageService::Size(ServerContext* context,
                                        const Empty* request,
                                        StorageSizeInfo* reply) {
    reply->set_size(
        stringToStringMap.size());  // todo may be incorrect because of types
    return Status::OK;
}

void dictionary::StorageService::save() {
    pid_t pid = fork();
    if (pid == -1) {
        gpr_log(GPR_ERROR, "Can`t fork process to save snapshot");
        abort();
    } else if (pid <= 0) {
        std::ofstream snapshot("last.rdb");
        {
            cereal::BinaryOutputArchive oarchive(snapshot);
            oarchive(stringToStringMap);
        }
        exit(0);
    }
}

void dictionary::StorageService::load() {}
