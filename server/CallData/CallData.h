#pragma once

#include "CallDataInterface.h"
#include "../StorageService.h"

// Abstract class encompasing the state and logic needed to serve a request.
template <class T, class K>
class CallData : public CallDataInterface {
   public:
    CallData(dictionary::StorageService* storage,
             grpc::ServerCompletionQueue* cq)
        : storage_(storage), cq_(cq), responder_(&ctx_) {
    }

    void proceed() {
        if (status_ == CREATE) {
            startTask();
        } else if (status_ == PROCESS) {
            processReply();
        } else {
            finish();
        }
    }

    void finish() {
        GPR_ASSERT(status_ == FINISH);
        delete this;
    }
    ~CallData() {};

   protected:
    dictionary::StorageService* storage_;
    grpc::ServerCompletionQueue* cq_;
    grpc::ServerContext ctx_;

    // What we get from the client.
    T request_;
    // What we send back to the client.
    K reply_;

    grpc::ServerAsyncResponseWriter<K> responder_;
};
