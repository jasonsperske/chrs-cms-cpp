#include "Library.h"
#include <algorithm>

std::shared_ptr<Library> Library::fetch(const drogon::orm::DbClientPtr &dbClient)
{
    try
    {
        auto result = dbClient->execSqlSync(
            "SELECT * FROM library ORDER BY section ASC, title ASC");

        std::vector<std::shared_ptr<Section>> sections;
        std::string currentSectionName;
        std::vector<std::shared_ptr<Entry>> currentSectionEntries;

        auto createSection = [&sections, &currentSectionName, &currentSectionEntries]()
        {
            if (!currentSectionName.empty())
            {
                sections.push_back(std::make_shared<Section>(
                    currentSectionName,
                    currentSectionEntries));
            }
        };

        for (const auto &row : result)
        {
            auto entry = std::make_shared<Entry>(
                row["title"].as<std::string>(),
                row["mediaType"].as<std::string>(),
                row["id"].as<int>(),
                row["author"].as<std::string>(),
                row["publishedBy"].as<std::string>(),
                row["publishedLocation"].as<std::string>(),
                row["publishedOn"].as<std::string>(),
                row["edition"].as<std::string>(),
                row["editionYear"].as<std::string>(),
                row["serialNumber"].as<std::string>(),
                row["catalogNumber"].as<std::string>(),
                row["section"].as<std::string>());

            std::string sectionName = row["section"].as<std::string>();

            if (currentSectionName.empty())
            {
                currentSectionName = sectionName;
                currentSectionEntries.push_back(entry);
            }
            else if (currentSectionName != sectionName)
            {
                createSection();
                currentSectionName = sectionName;
                currentSectionEntries.clear();
                currentSectionEntries.push_back(entry);
            }
            else
            {
                currentSectionEntries.push_back(entry);
            }
        }

        // Don't forget to create the last section
        createSection();

        return std::make_shared<Library>(sections);
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_ERROR << "Database error in Library::fetch: " << e.base().what();
        return nullptr;
    }
}