#include <QString>
#include <QDebug>
#include <QObject>
#include <string>
#include <iostream>
#include <fstream>
#include <QApplication>

#include "GripStrengthRequestHandler.h"
#include "CypressApplication.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPResponse.h"
#include "../server/server.h"


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
    std::ifstream input("C:/work/clsa/cypress/src/rest/server/pages/index.html");
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

    server -> requestTestStart();

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

    out.flush();
}
