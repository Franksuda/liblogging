#pragma once

#ifndef BB08BBA7_3647_432E_A17F_3AB145B67E57
#define BB08BBA7_3647_432E_A17F_3AB145B67E57

#include <spdlog/common.h>

#include <synyi/logging/client.h>
#include <synyi/logging/exports.h>

#if __cplusplus
extern "C" {
#endif

#ifndef SYNYI_LOGGING_CLIENT_HANDLE
typedef void *SYNYI_LOGGING_CLIENT_HANDLE;
#endif

int SYNYI_LOGGING_CLIENT_LIB_EXPORT getLoggingSdkVersion(char **version);

SYNYI_LOGGING_CLIENT_HANDLE SYNYI_LOGGING_CLIENT_LIB_EXPORT
initialize_logging_client(const char *loggingUrl, const char *streamName,
                          const char *environment, const char *application,
                          const char *machineName, const char *logger,
                          const char *appVersion, int64_t intervallogcount,
                          int64_t watchdogintervaltime,
                          int64_t sendintervaltime);

int SYNYI_LOGGING_CLIENT_LIB_EXPORT
dispose_logging_client(const SYNYI_LOGGING_CLIENT_HANDLE hClient);

bool SYNYI_LOGGING_CLIENT_LIB_EXPORT createLoggingGroupName(
    const SYNYI_LOGGING_CLIENT_HANDLE hCliente, const char *groupName);

void SYNYI_LOGGING_CLIENT_LIB_EXPORT
initialization(const SYNYI_LOGGING_CLIENT_HANDLE hClient,
               std::function<int(int, const char *)> errorHandler);

void SYNYI_LOGGING_CLIENT_LIB_EXPORT
sendLog(const SYNYI_LOGGING_CLIENT_HANDLE hClient,
        const std::map<std::string, std::string> &sendMap);

#if __cplusplus
}
#endif

#endif /* BB08BBA7_3647_432E_A17F_3AB145B67E57 */
