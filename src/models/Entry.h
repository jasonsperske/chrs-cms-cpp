#pragma once
#include <drogon/orm/Result.h>
#include <json/json.h>
#include <string>

class Entry
{
public:
    Entry(const std::string &title,
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
          const std::string &section);

    // Factory method
    static std::shared_ptr<Entry> fetch(int id, const drogon::orm::DbClientPtr &dbClient);

    // Basic property getters
    int getId() const { return id; }
    const std::string &getTitle() const { return title; }
    const std::string &getMediaType() const { return mediaType; }
    const std::string &getAuthor() const { return author; }
    const std::string &getPublishedBy() const { return publishedBy; }
    const std::string &getPublishedLocation() const { return publishedLocation; }
    const std::string &getPublishedOn() const { return publishedOn; }
    const std::string &getEditionYear() const { return editionYear; }
    const std::string &getSerialNumber() const { return serialNumber; }
    const std::string &getCatalogNumber() const { return catalogNumber; }
    const std::string &getSection() const { return section; }

    // Computed property getters
    std::string getPublisher() const;
    std::string getEdition() const;
    std::string getSerialNumbers() const;

    // JSON serialization
    Json::Value toJson() const;

private:
    int id;
    std::string title;
    std::string mediaType;
    std::string author;
    std::string publishedBy;
    std::string publishedLocation;
    std::string publishedOn;
    std::string edition;
    std::string editionYear;
    std::string serialNumber;
    std::string catalogNumber;
    std::string section;
};