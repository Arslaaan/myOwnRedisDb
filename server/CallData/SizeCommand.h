#pragma once

#include "CallData.h"

class SizeCommand : public CallData<dictionary::Empty, dictionary::StorageSizeInfo> {
   public:
    SizeCommand(dictionary::StorageService* storage,
                grpc::ServerCompletionQueue* cq)
        : CallData(storage, cq){
            proceed();
        };

    void startTask() override {
        status_ = PROCESS;
        storage_->RequestSize(&ctx_, &request_, &responder_, cq_, cq_, this);
    }

    void processReply() override {
        new SizeCommand(storage_, cq_);

        grpc::Status result = storage_->Size(&ctx_, &request_, &reply_);

        status_ = FINISH;
        responder_.Finish(reply_, result, this);
    }

    ~SizeCommand() {}
};