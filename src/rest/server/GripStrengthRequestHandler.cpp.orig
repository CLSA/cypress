#include "GripStrengthRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPResponse.h"

#include <QProcess>
#include <QDebug>
#include <QString>

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
    //std::ifstream input("/home/ant/work/cypress/http_server/server/pages/index.html");

    std::ostream& out = response.send();

    out << "<html><head><title>Cypress</title></head>";
    out << "<body><div>Initializing grip strength...</div></body>";
    out << "</html>";

    out.flush();

    QString program = "/usr/bin/firefox";
    QStringList arguments;

    QProcess cypress;
    cypress.start(program, arguments);
    cypress.waitForFinished();
}
