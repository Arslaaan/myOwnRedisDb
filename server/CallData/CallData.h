#pragma once

#include "CallDataInterface.h"
#include "server/StorageService.h"

// Abstract class encompasing the state and logic needed to serve a request.
// Implementations are written below
template <class T, class K, class R>
class CallData : public CallDataInterface {
   public:
    CallData(dictionary::StorageService* storage,
             grpc::ServerCompletionQueue* cq)
        : storage_(storage), cq_(cq), responder_(&ctx_) {}

    void proceed() {
        if (status_ == CREATE) {
            startTask();
        } else if (status_ == PROCESS) {
            if (storage_->snapshotEnabled()) {
                // activity detected so reset timer
                storage_->resetTimer();
            }
            storage_->removeOutdatedEntries();
            processReply();
        } else {
            finish();
        }
    }

    void finish() {
        GPR_ASSERT(status_ == FINISH);
        delete this;
    }
    ~CallData(){};

   protected:
    dictionary::StorageService* storage_;
    grpc::ServerCompletionQueue* cq_;
    grpc::ServerContext ctx_;

    // What we get from the client.
    T request_;
    // What we send back to the client.
    K reply_;
    // Means of responding to the client
    R responder_;
};

class GetCommand : public CallData<dictionary::Key, dictionary::Value, grpc::ServerAsyncResponseWriter<dictionary::Value>> {
   public:
    GetCommand(dictionary::StorageService* storage,
               grpc::ServerCompletionQueue* cq)
        : CallData(storage, cq) {
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

class SetCommand : public CallData<dictionary::Entry, dictionary::Empty, grpc::ServerAsyncResponseWriter<dictionary::Empty>> {
   public:
    SetCommand(dictionary::StorageService* storage,
               grpc::ServerCompletionQueue* cq)
        : CallData(storage, cq) {
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

class SizeCommand : public CallData<dictionary::Empty, dictionary::StorageSizeInfo, grpc::ServerAsyncResponseWriter<dictionary::StorageSizeInfo>> {
   public:
    SizeCommand(dictionary::StorageService* storage,
                grpc::ServerCompletionQueue* cq)
        : CallData(storage, cq) {
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

class RemoveCommand : public CallData<dictionary::Key, dictionary::Empty, grpc::ServerAsyncResponseWriter<dictionary::Empty>> {
   public:
    RemoveCommand(dictionary::StorageService* storage,
                  grpc::ServerCompletionQueue* cq)
        : CallData(storage, cq) {
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

class PurgeCommand : public CallData<dictionary::Empty, dictionary::Empty, grpc::ServerAsyncResponseWriter<dictionary::Empty>> {
   public:
    PurgeCommand(dictionary::StorageService* storage,
                  grpc::ServerCompletionQueue* cq)
        : CallData(storage, cq) {
        proceed();
    };

    void startTask() override {
        status_ = PROCESS;
        storage_->RequestPurge(&ctx_, &request_, &responder_, cq_, cq_, this);
    }

    void processReply() override {
        new PurgeCommand(storage_, cq_);

        grpc::Status result = storage_->Purge(&ctx_, &request_, &reply_);

        status_ = FINISH;
        responder_.Finish(reply_, result, this);
    }

    ~PurgeCommand() {}
};

class KeysCommand : public CallData<dictionary::Empty, dictionary::Key, grpc::ServerAsyncWriter<dictionary::Key>> {
    size_t counter;
    bool responderCreated;
    std::vector<dictionary::Key> keys;
   public:
    KeysCommand(dictionary::StorageService* storage,
                  grpc::ServerCompletionQueue* cq)
        : CallData(storage, cq), counter(0), responderCreated(false) {
        proceed();
    };

    void startTask() override {
        status_ = PROCESS;
        storage_->RequestKeys(&ctx_, &request_, &responder_, cq_, cq_, this);
    }

    void processReply() override {
        if (!responderCreated) {
            new KeysCommand(storage_, cq_);
            responderCreated = true;
        }

        if (keys.empty()) {
            keys = storage_->getKeys();
        }

        if (counter >= keys.size()) {
            status_ = FINISH;
            responder_.Finish(Status::OK, this);
        } else {
            responder_.Write(keys.at(counter), this);
            counter++;
        }
    }

    ~KeysCommand() {}
};
