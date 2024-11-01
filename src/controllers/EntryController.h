#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

class EntryController
{
public:
    void get(const HttpRequestPtr &req,
             std::function<void(const HttpResponsePtr &)> &&callback,
             const std::string &id);
};