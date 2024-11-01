#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

class VisionController
{
public:
    void post(const HttpRequestPtr &req,
              std::function<void(const HttpResponsePtr &)> &&callback);

private:
    std::string generatePrompt(const std::vector<std::string> &files);
    HttpRequestPtr createOpenAIRequest(const std::string &jsonBody);
};