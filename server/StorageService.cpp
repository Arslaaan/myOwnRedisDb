#include "StorageService.h"

Status dictionary::StorageService::set(ServerContext* context, const Entry* request, Empty* reply)  {
        return Status::OK;
}

Status dictionary::StorageService::get(ServerContext* context, const Key* request, Value* reply) {
        return Status::OK;
}

Status dictionary::StorageService::remove(ServerContext* context, const Key* request,
                      Empty* reply) {
        return Status::OK;
}

Status dictionary::StorageService::size(ServerContext* context, const Empty* request,
                    StorageSizeInfo* reply) {
        return Status::OK;
}
