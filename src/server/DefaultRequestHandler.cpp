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

void DefaultRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    std::string uri = request.getURI();

    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("text/html");
    std::ostream& out = response.send();

    out << "<html><head><title>Cypress</title></head>";
    out << "<body><div>404</div></body>";
    out << "</html>";

    out.flush();

    qDebug() << "404: " << QString::fromStdString(uri);
}

//std::ifstream input("C:/work/clsa/cypress/src/rest/server/pages/index.html");
    //std::string str;

    //if (input.is_open())
    //{
    //    while (getline(input, str))
    //    {
    //        out << str;
    //    }

    //    input.close();
    //}

    //out << "<html><head><title>Cypress</title></head>";
    //out << "<body><div>Initializing grip strength...</div></body>";
    //out << "</html>";


    //CypressApplication cypress;
    //QVariantMap args;


    //args["inputFileName"] = "";
    //args["outputFileName"] = "";
    //args["measureType"] = "";
    //args["runMode"] = "";
    //args["verbose"] = "";

    //try
    //{
    //    cypress.setArgs(args);
    //    cypress.initialize();
    //}
    //catch (std::exception& e)
    //{
    //    qDebug() << e.what();
    //}

