#include <drogon/drogon.h>
#include "controllers/IndexController.h"
#include "controllers/EntryController.h"
#include "controllers/VisionController.h"
#include "models/Library.h"
#include "models/Entry.h"
#include <json/json.h>
#include <opencv2/opencv.hpp>
#include <openai.hpp>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>

using namespace drogon;

void loadEnvFile()
{
    std::ifstream envFile(".env");
    std::string line;
    while (std::getline(envFile, line))
    {
        size_t pos = line.find('=');
        if (pos != std::string::npos)
        {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            setenv(key.c_str(), value.c_str(), 1);
        }
    }
}

int main()
{
    loadEnvFile();

    // Configure Drogon app
    app().setLogPath("./").setLogLevel(trantor::Logger::kWarn).addListener("0.0.0.0", 3000).setDocumentRoot("templates").createDbClient("sqlite3", "library.sqlite", "", "", "", 1);

    // Load routes
    app().registerHandler("/",
                          [](const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback)
                          {
                              auto resp = HttpResponse::newHttpViewResponse("index");
                              callback(resp);
                          });

    app().registerHandler("/entry/{id}.json",
                          [](const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback,
                             const std::string &id)
                          {
                              auto controller = EntryController();
                              controller.get(req, std::move(callback), id);
                          });

    app().registerHandler("/api/openai/vision",
                          [](const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback)
                          {
                              auto controller = VisionController();
                              controller.post(req, std::move(callback));
                          },
                          {Post});

    // Start the server
    app().run();
    return 0;
}