#py from parse import parse
#py import model
#py plugin = parse(pycpp.params['xml_file'])
// -------------------------------------------------------
// Add your custom functions at the bottom of the file
// and the server counterpart to lua/b0RemoteApiServer.lua
// -------------------------------------------------------

#pragma once

#include <string>
#include <vector>
#include <map>
#include <b0/node.h>
#include <b0/publisher.h>
#include <b0/subscriber.h>
#include <b0/service_client.h>
#include "msgpack.hpp"

#ifndef _WIN32
    #define __cdecl
#endif

typedef boost::function<void(std::vector<msgpack::object>*)> CB_FUNC;

struct SHandleAndCb
{
    b0::Subscriber* handle;
    bool dropMessages;
    CB_FUNC cb;
};


class b0RemoteApi
{
public:
    b0RemoteApi(const char* nodeName="b0RemoteApi_c++Client",const char* channelName="b0RemoteApi",int inactivityToleranceInSec=60,bool setupSubscribersAsynchronously=false);
    virtual ~b0RemoteApi();

    const char* simxServiceCall();
    const char* simxDefaultPublisher();
    const char* simxDefaultSubscriber(CB_FUNC cb,int publishInterval=1);
    const char* simxCreatePublisher(bool dropMessages=false);
    const char* simxCreateSubscriber(CB_FUNC cb,int publishInterval=1,bool dropMessages=false);

    long simxGetTimeInMs();
    void simxSleep(int durationInMs);
    void simxSpin();
    void simxSpinOnce();

    static void print(const std::vector<msgpack::object>* msg);
    static bool hasValue(const std::vector<msgpack::object>* msg);
    static const msgpack::object* readValue(std::vector<msgpack::object>* msg,int valuesToDiscard=0,bool* success=NULL);
    static bool readBool(std::vector<msgpack::object>* msg,int valuesToDiscard=0,bool* success=NULL);
    static int readInt(std::vector<msgpack::object>* msg,int valuesToDiscard=0,bool* success=NULL);
    static float readFloat(std::vector<msgpack::object>* msg,int valuesToDiscard=0,bool* success=NULL);
    static double readDouble(std::vector<msgpack::object>* msg,int valuesToDiscard=0,bool* success=NULL);
    static std::string readString(std::vector<msgpack::object>* msg,int valuesToDiscard=0,bool* success=NULL);
    static std::string readByteArray(std::vector<msgpack::object>* msg,int valuesToDiscard=0,bool* success=NULL);
    static bool readIntArray(std::vector<msgpack::object>* msg,std::vector<int>& array,int valuesToDiscard=0);
    static bool readFloatArray(std::vector<msgpack::object>* msg,std::vector<float>& array,int valuesToDiscard=0);
    static bool readDoubleArray(std::vector<msgpack::object>* msg,std::vector<double>& array,int valuesToDiscard=0);
    static bool readStringArray(std::vector<msgpack::object>* msg,std::vector<std::string>& array,int valuesToDiscard=0);

protected:
    std::vector<msgpack::object>* _handleFunction(const char* funcName,const std::string& packedArgs,const char* topic);
    void _handleReceivedMessage(const std::string packedData);
    void _pingCallback(std::vector<msgpack::object>* msg);

    std::string _serviceCallTopic;
    std::string _defaultPublisherTopic;
    std::string _defaultSubscriberTopic;
    std::vector<std::string> _allTopics;
    int _nextDefaultSubscriberHandle;
    int _nextDedicatedPublisherHandle;
    int _nextDedicatedSubscriberHandle;
    bool _pongReceived;
    bool _setupSubscribersAsynchronously;
    msgpack::unpacked _tmpUnpackedMsg;
    std::vector<msgpack::object> _tmpMsgPackObjects;
    std::string _channelName;
    b0::Node* _node;
    std::string _clientId;
    b0::ServiceClient* _serviceClient;
    b0::Publisher* _defaultPublisher;
    b0::Subscriber* _defaultSubscriber;
    std::map<std::string,SHandleAndCb> _allSubscribers;
    std::map<std::string,b0::Publisher*> _allDedicatedPublishers;

public:

    void simxSynchronous(bool enable);
    void simxSynchronousTrigger();
    void simxGetSimulationStepDone(const char* topic);
    void simxGetSimulationStepStarted(const char* topic);
    std::vector<msgpack::object>* simxCallScriptFunction(const char* funcAtObjName,int scriptType,const char* packedData,size_t packedDataSize,const char* topic);
    std::vector<msgpack::object>* simxCallScriptFunction(const char* funcAtObjName,const char* scriptType,const char* packedData,size_t packedDataSize,const char* topic);

#py for cmd in plugin.commands:
#py if cmd.generic and cmd.generateCode:
#py loopCnt=1
#py for p in cmd.params:
#py if p.ctype()=='int_eval':
#py loopCnt=2
#py endif
#py endfor
#py for k in range(loopCnt):
    std::vector<msgpack::object>* `cmd.name`(
#py theStringToWrite=''
#py itemCnt=len(cmd.params)
#py itemIndex=-1
#py for p in cmd.params:
#py itemIndex=itemIndex+1
#py if p.ctype()=='int_eval':
#py if k==0:
#py theStringToWrite+='        int '+p.name
#py else:
#py theStringToWrite+='        const char* '+p.name
#py endif
#py elif p.htype()=='byte[]':
#py theStringToWrite+='        const char* '+p.name+'_data,size_t '+p.name+'_size'
#py elif p.htype()=='int[]':
#py theStringToWrite+='        const int* '+p.name+'_data,size_t '+p.name+'_size'
#py elif 'int[' in p.htype():
#py theStringToWrite+='        const int* '+p.name
#py elif p.htype()=='float[]':
#py theStringToWrite+='        const float* '+p.name+'_data,size_t '+p.name+'_size'
#py elif 'float[' in p.htype():
#py theStringToWrite+='        const float* '+p.name
#py elif p.htype()=='double[]':
#py theStringToWrite+='        const double* '+p.name+'_data,size_t '+p.name+'_size'
#py elif 'double[' in p.htype():
#py theStringToWrite+='        const double* '+p.name
#py else:
#py theStringToWrite+='        '+p.htype()+' '+p.name
#py endif
#py if (itemCnt>1) and itemIndex<itemCnt-1:
#py theStringToWrite+=',\n'
#py endif
#py endfor
#py theStringToWrite+=');'
`theStringToWrite`
#py endfor
#py endif
#py endfor



    // -------------------------------
    // Add your custom functions here:
    // -------------------------------
    
};
