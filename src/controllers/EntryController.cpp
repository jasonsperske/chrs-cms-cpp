#include "EntryController.h"
#include "../models/Entry.h"
#include <json/json.h>

void EntryController::get(const HttpRequestPtr &req,
                          std::function<void(const HttpResponsePtr &)> &&callback,
                          const std::string &id)
{
    auto dbClient = app().getDbClient();
    Entry entry(std::stoi(id), dbClient);

    Json::Value response;
    response["success"] = true;
    response["response"] = entry.toJson();

    auto resp = HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}