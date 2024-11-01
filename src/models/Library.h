#pragma once
#include "Section.h"
#include <drogon/orm/DbClient.h>
#include <vector>
#include <json/json.h>

class Library
{
public:
    explicit Library(const std::vector<std::shared_ptr<Section>> &sections)
        : sections(sections) {}

    static std::shared_ptr<Library> fetch(const drogon::orm::DbClientPtr &dbClient);

    const std::vector<std::shared_ptr<Section>> &getSections() const { return sections; }

    Json::Value toJson() const
    {
        Json::Value json;
        json["sections"] = Json::Value(Json::arrayValue);
        for (const auto &section : sections)
        {
            json["sections"].append(section->toJson());
        }
        return json;
    }

private:
    std::vector<std::shared_ptr<Section>> sections;
};