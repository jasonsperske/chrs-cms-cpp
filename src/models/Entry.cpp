#include "Entry.h"
#include <sstream>

Entry::Entry(const std::string &title,
             const std::string &mediaType,
             int id,
             const std::string &author,
             const std::string &publishedBy,
             const std::string &publishedLocation,
             const std::string &publishedOn,
             const std::string &edition,
             const std::string &editionYear,
             const std::string &serialNumber,
             const std::string &catalogNumber,
             const std::string &section)
    : title(title), mediaType(mediaType), id(id), author(author), publishedBy(publishedBy), publishedLocation(publishedLocation), publishedOn(publishedOn), edition(edition), editionYear(editionYear), serialNumber(serialNumber), catalogNumber(catalogNumber), section(section) {}

std::shared_ptr<Entry> Entry::fetch(int id, const drogon::orm::DbClientPtr &dbClient)
{
    try
    {
        auto result = dbClient->execSqlSync(
            "SELECT * FROM library WHERE id = ?",
            id);

        if (result.size() > 0)
        {
            const auto &row = result[0];
            return std::make_shared<Entry>(
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
        }
        return nullptr;
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_ERROR << "Database error: " << e.base().what();
        return nullptr;
    }
}

std::string Entry::getPublisher() const
{
    std::ostringstream oss;
    oss << publishedBy << " " << publishedLocation << " " << publishedOn;
    return oss.str();
}

std::string Entry::getEdition() const
{
    if (editionYear.empty())
    {
        return edition;
    }
    return edition + " (" + editionYear + ")";
}

std::string Entry::getSerialNumbers() const
{
    std::vector<std::string> numbers;

    if (!serialNumber.empty())
    {
        numbers.push_back("isbn:" + serialNumber);
    }
    if (!catalogNumber.empty())
    {
        numbers.push_back("catalog:" + catalogNumber);
    }

    std::ostringstream oss;
    for (size_t i = 0; i < numbers.size(); ++i)
    {
        if (i > 0)
            oss << " ";
        oss << numbers[i];
    }
    return oss.str();
}

Json::Value Entry::toJson() const
{
    Json::Value json;
    json["id"] = id;
    json["title"] = title;
    json["mediaType"] = mediaType;
    json["author"] = author;
    json["publishedBy"] = publishedBy;
    json["publishedLocation"] = publishedLocation;
    json["publishedOn"] = publishedOn;
    json["edition"] = edition;
    json["editionYear"] = editionYear;
    json["serialNumber"] = serialNumber;
    json["catalogNumber"] = catalogNumber;
    json["section"] = section;
    json["publisher"] = getPublisher();
    json["formattedEdition"] = getEdition();
    json["serialNumbers"] = getSerialNumbers();
    return json;
}