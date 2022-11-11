#include <QString>
#include <QDebug>

#include <string>
#include <iostream>
#include <fstream>

#include "GripStrengthRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPResponse.h"



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

    std::ostream& out = response.send();
    std::ifstream input("C:/work/clsa/cypress/rest/server/pages/index.html");
    std::string str;

    if (input.is_open())
    {
        while (getline(input, str))
        {
            out << str;
        }

        input.close();
    }


    //out << "<html><head><title>Cypress</title></head>";
    //out << "<body><div>Initializing grip strength...</div></body>";
    //out << "</html>";

    out.flush();

    //QString program = "/usr/bin/firefox";
    //QStringList arguments;

    //QProcess cypress;
    //cypress.start(program, arguments);
    //cypress.waitForFinished();
}
