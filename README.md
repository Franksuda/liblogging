# liblogging

This SDK provide synyi designed interface for logging, default spdlog, OATPP_LOG were also welcomed.

The document briefly introduce how to use our new designed API.

## Part I

create structured log with designed API

```c++
auto sendSturctMap = synyi::logging::ArgsHelper::format(
    spdlog::level::level_enum::info, "start Demo listening on: {address}",
    "http://localhost:5000");
client.sendLog(sendSturctMap);
```

create unstructured log with designed API

```c++
// send unstructured log  with synyiLog
auto sendNormalMsg = synyi::logging::ArgsHelper::createNormalLog(
    spdlog::level::level_enum::info, "unstruct log with synyilogging");
client.sendLog(sendNormalMsg);
```

## Part II

```c++
// send unstructured log with spdlog default method
spdlog::info("info spdlog");
spdlog::error("error spdlog");

// send unstructured log with OATPP_LOG
OATPP_LOGI("synyi", "Server running on port %s", "255");
OATPP_LOGE("synyi", "Server running on port %s", "0");
```

## Part III

Any request from customer will be logged by registered self designed **MyRequestInterceptor&MyResponseInterceptor** to resend structured log.

**MyRequestInterceptor** will log request information, and MyResponseInterceptor will log reponse information. A random RequestId will create in **MyRequestInterceptor** and send into request's header. You can log with it before reponse finished.
