#ifndef CHOICE_REACTION_REQUEST_HANDLER_H
#define CHOICE_REACTION_REQUEST_HANDLER_H

#include "server/default_request_handler.h"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

class ChoiceReactionRequestHandler : public DefaultRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
    bool isValidInputData(const QJsonObject& inputData) override;
};

#endif // CHOICE_REACTION_REQUEST_HANDLER_H
