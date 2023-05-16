#pragma once

#include "CallData.h"

class GetCommand : public CallData<dictionary::Key, dictionary::Value> {
   public:
    GetCommand(dictionary::StorageService* storage,
                grpc::ServerCompletionQueue* cq)
        : CallData(storage, cq){
            proceed();
        };

    void startTask() override {
        status_ = PROCESS;
        storage_->RequestGet(&ctx_, &request_, &responder_, cq_, cq_, this);
    }

    void processReply() override {
        new GetCommand(storage_, cq_);

        grpc::Status result = storage_->Get(&ctx_, &request_, &reply_);

        status_ = FINISH;
        responder_.Finish(reply_, result, this);
    }

    ~GetCommand() {}
};