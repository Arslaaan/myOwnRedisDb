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
    reply->set_size(stringToStringMap.size()); // todo may be incorrect because of types
    return Status::OK;
}
