#ifndef CHOICEREACTIONREQUESTHANDLER_H
#define CHOICEREACTIONREQUESTHANDLER_H

#include "server/DefaultRequestHandler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class ChoiceReactionRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // CHOICEREACTIONREQUESTHANDLER_H
