#pragma once

#include "CallData.h"

class SetCommand : public CallData<dictionary::Entry, dictionary::Empty> {
   public:
    SetCommand(dictionary::StorageService* storage,
                grpc::ServerCompletionQueue* cq)
        : CallData(storage, cq){
            proceed();
        };

    void startTask() override {
        status_ = PROCESS;
        storage_->RequestSet(&ctx_, &request_, &responder_, cq_, cq_, this);
    }

    void processReply() override {
        new SetCommand(storage_, cq_);

        grpc::Status result = storage_->Set(&ctx_, &request_, &reply_);

        status_ = FINISH;
        responder_.Finish(reply_, result, this);
    }

    ~SetCommand() {}
};