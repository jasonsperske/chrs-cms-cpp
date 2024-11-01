#include "VisionController.h"
#include <opencv2/opencv.hpp>
#include <json/json.h>
#include <drogon/HttpClient.h>

void VisionController::post(const HttpRequestPtr &req,
                            std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto files = req->getParameters()["files"];
    std::vector<std::string> processedFiles;

    for (const auto &file : files)
    {
        try
        {
            // Process image using OpenCV
            std::vector<uchar> buffer(file.begin(), file.end());
            cv::Mat img = cv::imdecode(buffer, cv::IMREAD_COLOR);

            if (!img.empty())
            {
                cv::Mat resized;
                cv::resize(img, resized, cv::Size(1024, 1024), 0, 0, cv::INTER_LANCZOS4);

                std::vector<uchar> encoded;
                cv::imencode(".jpg", resized, encoded);

                std::string base64Image = drogon::utils::base64Encode(encoded.data(), encoded.size());
                processedFiles.push_back(base64Image);
            }
        }
        catch (const std::exception &e)
        {
            LOG_ERROR << "Cannot process image: " << e.what();
        }
    }

    // Create messages array for OpenAI API
    Json::Value messages(Json::arrayValue);

    // System message
    Json::Value systemMsg;
    systemMsg["role"] = "system";
    systemMsg["content"] = "I have pictures of a book, a manual or possibly a magazine. Respond in the" +
                           "form of a JSON. I would like you to tell me what type of media it " +
                           "is in a field called `mediaType` with \"book\" for a book and \"magazine\" " +
                           "for magazine. Tell me the title (if available) in field called `title`, " +
                           "the author (if available) in a field called `author`. If the media " +
                           "is a book tell me the year it was initially published if available. " +
                           "If the media is a magazine tell me the year it was issued in a field " +
                           "called `yearPublished` and the month in a field called `monthPublished` " +
                           "if it can be determined. If its possible tell me the name of the " +
                           "publisher in a field called `publishedBy`. If it is possible, tell me " +
                           "the place where the book was published in a field called `publishedLocation`. " +
                           "If it is possible tell me the edition of the book or magazine in a field " +
                           "called `edition`, if the edition is published in a later year tell me in " +
                           "a fieled called `editionYear`. If it is possible identify the ISBN number " +
                           "(also sometimes labeled \"International Standard Book No.\") in a field " +
                           "called `serialNumber`. If it is possible identify the \"Library of Congress " +
                           "Catalog Card Number\" number (also sometimes labeled \"Library of Congress " +
                           "Catalog Number\") in a field called `catalogNumber`. There may be more than " +
                           "one interpretation, give me these multiple interpretations with a confidence " +
                           "score between 0 and 1 for each.";
    messages.append(systemMsg);

    // User message with images
    Json::Value userMsg;
    userMsg["role"] = "user";
    Json::Value content(Json::arrayValue);

    for (const auto &file : processedFiles)
    {
        Json::Value imgMsg;
        imgMsg["type"] = "image_url";
        imgMsg["image_url"]["url"] = "data:image/jpeg;base64," + file;
        imgMsg["image_url"]["detail"] = "auto";
        content.append(imgMsg);
    }

    userMsg["content"] = content;
    messages.append(userMsg);

    // Create the complete request body
    Json::Value requestBody;
    requestBody["model"] = "gpt-4-vision-preview";
    requestBody["messages"] = messages;
    requestBody["max_tokens"] = 800;

    Json::StreamWriterBuilder writer;
    std::string jsonBody = Json::writeString(writer, requestBody);

    // Create HTTP client and request
    auto httpClient = HttpClient::newHttpClient("https://api.openai.com");
    auto openaiReq = createOpenAIRequest(jsonBody);

    httpClient->sendRequest(openaiReq, [callback](ReqResult result, const HttpResponsePtr &openaiResp)
                            {
        if (result != ReqResult::Ok || !openaiResp) {
            Json::Value errorResponse;
            errorResponse["success"] = false;
            errorResponse["error"] = "Failed to connect to OpenAI API";
            auto resp = HttpResponse::newHttpJsonResponse(errorResponse);
            callback(resp);
            return;
        }
        
        auto statusCode = openaiResp->getStatusCode();
        if (statusCode != k200OK) {
            Json::Value errorResponse;
            errorResponse["success"] = false;
            errorResponse["error"] = "OpenAI API error: " + std::to_string(statusCode);
            errorResponse["message"] = openaiResp->getBody();
            auto resp = HttpResponse::newHttpJsonResponse(errorResponse);
            callback(resp);
            return;
        }
        
        try {
            Json::Value responseJson;
            Json::CharReaderBuilder reader;
            std::string errors;
            std::stringstream ss(openaiResp->getBody());
            
            if (!Json::parseFromStream(reader, ss, &responseJson, &errors)) {
                throw std::runtime_error("Failed to parse OpenAI response: " + errors);
            }
            
            Json::Value response;
            response["success"] = true;
            response["response"] = responseJson["choices"][0]["message"]["content"];
            
            auto resp = HttpResponse::newHttpJsonResponse(response);
            callback(resp);
            
        } catch (const std::exception& e) {
            Json::Value errorResponse;
            errorResponse["success"] = false;
            errorResponse["error"] = "Failed to parse OpenAI response";
            errorResponse["message"] = e.what();
            auto resp = HttpResponse::newHttpJsonResponse(errorResponse);
            callback(resp);
        } });
}

HttpRequestPtr VisionController::createOpenAIRequest(const std::string &jsonBody)
{
    auto req = HttpRequest::newHttpRequest();
    req->setMethod(Post);
    req->setPath("/v1/chat/completions");
    req->setBody(jsonBody);

    // Set headers
    req->addHeader("Content-Type", "application/json");
    req->addHeader("Authorization", "Bearer " + std::string(std::getenv("OPENAI_API_KEY")));

    return req;
}