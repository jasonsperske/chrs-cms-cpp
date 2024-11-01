#pragma once
#include "Entry.h"
#include <string>
#include <vector>
#include <json/json.h>

class Section
{
public:
    Section(const std::string &name, const std::vector<std::shared_ptr<Entry>> &entries)
        : name(name), entries(entries) {}

    std::string getName() const { return name; }
    const std::vector<std::shared_ptr<Entry>> &getEntries() const { return entries; }

    Json::Value toJson() const
    {
        Json::Value json;
        json["name"] = name;
        json["entries"] = Json::Value(Json::arrayValue);
        for (const auto &entry : entries)
        {
            json["entries"].append(entry->toJson());
        }
        return json;
    }

private:
    std::string name;
    std::vector<std::shared_ptr<Entry>> entries;
};
