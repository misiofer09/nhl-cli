#pragma once

#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>

#include "structs.hpp"

namespace beast = boost::beast;
namespace json = boost::json;
namespace asio = boost::asio;
namespace http = beast::http;

class NHLApiClient {
    public:
        NHLApiClient();

        auto requestDataFromApi(std::string) -> json::value;
        
        auto parseSchedule(json::value) -> std::vector<NHL::Game>;

    private:
        constexpr static auto api_hostname = "statsapi.web.nhl.com";
        constexpr static auto api_target = "/api/v1/";

        asio::io_context io_context;
        asio::ip::tcp::resolver resolver;
        beast::tcp_stream tcp_stream;
};
