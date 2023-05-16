#pragma once

class CallDataInterface {
   public:
    virtual void proceed() = 0;

   protected:
    virtual void startTask() = 0;
    virtual void processReply() = 0;
    virtual void finish() = 0;
    virtual ~CallDataInterface(){};

    enum CallStatus { CREATE, PROCESS, FINISH };
    CallStatus status_ = CREATE;
};