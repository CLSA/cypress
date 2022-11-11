#include <QDebug>

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

#include "RetinalCameraRequestHandler.h"

using namespace Poco::Net;

RetinalCameraRequestHandler::RetinalCameraRequestHandler()
{

}

RetinalCameraRequestHandler::~RetinalCameraRequestHandler()
{
}

void RetinalCameraRequestHandler::handleRequest(HTTPServerRequest &request, HTTPServerResponse &response)
{
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("text/html");

    std::ostream& out = response.send();

    out << "<html><head><title>Cypress</title></head>";
    out << "<body><div>Retinal Camera</div></body>";
    out << "</html>";

    out.flush();
}
