#include "GripStrengthRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPResponse.h"
#include <iostream>
#include <string>
#include <fstream>

GripStrengthRequestHandler::GripStrengthRequestHandler()
{

}

GripStrengthRequestHandler::~GripStrengthRequestHandler()
{

}

void GripStrengthRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("text/html");
    std::ifstream input("C:/work/clsa/cypress/src/index.html");

    std::ostream& out = response.send();
    for (std::string line; getline(input, line);)
    {
        out << line;
    }

    out.flush();
    //src.close();
}
