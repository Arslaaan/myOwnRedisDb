#include "StorageService.h"

Status dictionary::StorageService::set(ServerContext* context,
                                       const Entry* request, Empty* reply) {
    if (gmap.count(request->key().data())) {
        return Status(StatusCode::ALREADY_EXISTS, "Key already exists");
    }
    gmap.emplace(request->key().data(), request->value().data());
    return Status::OK;
}

Status dictionary::StorageService::get(ServerContext* context,
                                       const Key* request, Value* reply) {
    if (!gmap.count(request->data())) {
        return Status(StatusCode::NOT_FOUND, "No entry found");
    }
    reply->set_data(gmap[request->data()]);
    return Status::OK;
}

Status dictionary::StorageService::remove(ServerContext* context,
                                          const Key* request, Empty* reply) {
    if (!gmap.count(request->data())) {
        return Status(StatusCode::NOT_FOUND, "No entry found");
    }
    gmap.erase(request->data());
    return Status::OK;
}

Status dictionary::StorageService::size(ServerContext* context,
                                        const Empty* request,
                                        StorageSizeInfo* reply) {
    reply->set_size(gmap.size()); // todo may be incorrect because of types
    return Status::OK;
}
