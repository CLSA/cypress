#ifndef PARTICIPANTREPORTREQUESTHANDLER_H
#define PARTICIPANTREPORTREQUESTHANDLER_H

#include "server/default_request_handler.h"

class ParticipantReportRequestHandler : public DefaultRequestHandler
{
// HTTPRequestHandler interface
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response) override;
};

#endif // PARTICIPANTREPORTREQUESTHANDLER_H
