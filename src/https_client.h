// Copyright 2014 Toggl Desktop developers.

#ifndef SRC_HTTPS_CLIENT_H_
#define SRC_HTTPS_CLIENT_H_

#include <string>
#include <vector>

#include "./proxy.h"
#include "./types.h"

#include "Poco/Activity.h"

namespace Poco {
class Logger;
namespace Util {
}
}

namespace toggl {

class ServerStatus {
 public:
    ServerStatus()
        : gone_(false)
    , checker_(this, &ServerStatus::runActivity)
    , fast_retry_(true)
    , test_code_(0) {}

    virtual ~ServerStatus() {
        stopStatusCheck();
    }

    error Status();
    error UpdateStatus(const int status_code);
    void SetTestCode(const Poco::Int64 code) {
        test_code_ = code;
    }

 protected:
    void runActivity();

 private:
    bool gone_;
    Poco::Mutex gone_m_;

    Poco::Activity<ServerStatus> checker_;
    Poco::Mutex checker_m_;
    bool fast_retry_;

    Poco::Int64 test_code_;

    void setGone(const bool value);
    bool gone();

    void startStatusCheck(const bool fast_retry);
    void stopStatusCheck();
    bool checkingStatus();

    Poco::Logger &logger() const;
};

class HTTPSClientConfig {
 public:
    HTTPSClientConfig()
        : AppName("")
    , AppVersion("")
    , UseProxy(false)
    , ProxySettings(Proxy())
    , IgnoreCert(false)
    , CACertPath("") {}
    ~HTTPSClientConfig() {}

    std::string AppName;
    std::string AppVersion;
    bool UseProxy;
    toggl::Proxy ProxySettings;
    bool IgnoreCert;
    std::string CACertPath;

    std::string UserAgent() {
        return AppName + "/" + AppVersion;
    }
};

class HTTPSClient {
 public:
    HTTPSClient() {}
    virtual ~HTTPSClient() {}

    virtual error PostJSON(
        const std::string host,
        const std::string relative_url,
        const std::string json,
        const std::string basic_auth_username,
        const std::string basic_auth_password,
        std::string *response_body);

    virtual error GetJSON(
        const std::string host,
        const std::string relative_url,
        const std::string basic_auth_username,
        const std::string basic_auth_password,
        std::string *response_body);

    static HTTPSClientConfig Config;

 protected:
    virtual error request(
        const std::string method,
        const std::string host,
        const std::string relative_url,
        const std::string payload,
        const std::string basic_auth_username,
        const std::string basic_auth_password,
        std::string *response_body,
        int *response_status);
};

class TogglClient : public HTTPSClient {
 public:
    static void SetTestCode(const Poco::Int64 status_code) {
        toggl_status_.SetTestCode(status_code);
    }

 protected:
    virtual error request(
        const std::string method,
        const std::string host,
        const std::string relative_url,
        const std::string json,
        const std::string basic_auth_username,
        const std::string basic_auth_password,
        std::string *response_body,
        int *response_status);

 private:
    static ServerStatus toggl_status_;
};

}  // namespace toggl

#endif  // SRC_HTTPS_CLIENT_H_
