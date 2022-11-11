#include "DefaultRequestHandler.h"

#include <QString>
#include <QStringList>
#include <QProcess>
#include <QDebug>

#include <string>
#include <iostream>
#include <fstream>

#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

DefaultRequestHandler::DefaultRequestHandler()
{

}

DefaultRequestHandler::~DefaultRequestHandler()
{

}

void DefaultRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    std::string uri = request.getURI();
    qDebug() << QString::fromStdString(uri);
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("text/html");

    std::ostream& out = response.send();

    out << "<html><head><title>Cypress</title></head>";
    out << "<body><div>404</div></body>";
    out << "</html>";

    out.flush();
}
