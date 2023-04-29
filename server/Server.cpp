#include "Server.h"

Status Server::set(ServerContext* context, const Key* request, Value* reply)  {
        return Status::OK;
}

Status Server::get(ServerContext* context, const Key* request, Value* reply) {
        return Status::OK;
}

Status Server::remove(ServerContext* context, const Key* request,
                      Value* reply) {
        return Status::OK;
}

Status Server::size(ServerContext* context, const Empty* request,
                    DictionarySizeInfo* reply) {
        return Status::OK;
}
