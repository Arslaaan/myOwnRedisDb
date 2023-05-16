#pragma once

#include "CallData.h"

class RemoveCommand : public CallData<dictionary::Key, dictionary::Empty> {
   public:
    RemoveCommand(dictionary::StorageService* storage,
                grpc::ServerCompletionQueue* cq)
        : CallData(storage, cq){
            proceed();
        };

    void startTask() override {
        status_ = PROCESS;
        storage_->RequestRemove(&ctx_, &request_, &responder_, cq_, cq_, this);
    }

    void processReply() override {
        new RemoveCommand(storage_, cq_);

        grpc::Status result = storage_->Remove(&ctx_, &request_, &reply_);

        status_ = FINISH;
        responder_.Finish(reply_, result, this);
    }

    ~RemoveCommand() {}
};