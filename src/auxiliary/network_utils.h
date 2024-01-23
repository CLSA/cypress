#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H

#include <string>
#include <QByteArray>

#include "Poco/Net/HTTPRequest.h"

namespace NetworkUtils {
    bool sendHTTPRequest(
        const std::string &method,
        const std::string &endpoint,
        const std::string &contentType,
        const QByteArray &data);

    bool sendHTTPSRequest(
        const std::string &method,
        const std::string &endpoint,
        const std::string &contentType,
        const QByteArray &data);
}


#endif // NETWORKUTILS_H
