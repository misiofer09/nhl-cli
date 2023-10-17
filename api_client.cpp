#include "api_client.hpp"
#include <iostream>

NHLApiClient::NHLApiClient()
    : io_context(), resolver(io_context), tcp_stream(io_context) 
{
    const auto results = resolver.resolve(api_hostname, "http");
    tcp_stream.connect(results); 
}

auto NHLApiClient::requestDataFromApi(std::string request_target) -> json::value {
    auto request = http::request<http::string_body>(http::verb::get, api_target + request_target, 11);
    
    request.set(http::field::host, api_hostname);
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        
    http::write(tcp_stream, request);
        
    auto buffer = beast::flat_buffer();
    auto response = http::response<http::string_body>();
    http::read(tcp_stream, buffer, response);
    
    const auto response_body = boost::lexical_cast<std::string>(response.body());
    const auto json_response = json::parse(response_body);
    
    std::cout << std::format("{}\n\n", json::value_to<std::string>(json_response.at("copyright")));
    return json_response;
}

auto NHLApiClient::parseSchedule(json::value response) -> std::vector<NHL::Game> {
    auto schedule = std::vector<NHL::Game>();
    auto dates = response.at("dates").as_array();
    

    for(const auto& date : dates) {
        auto games = date.at("games").as_array();
        for(const auto& game_json : games) {
            auto game_status = json::value_to<std::string>(game_json.at("status").at("codedGameState"));
            auto home_team_json = game_json.at("teams").at("home");
            auto away_team_json = game_json.at("teams").at("away");
            auto game_date_raw = json::value_to<std::string>(game_json.at("gameDate"));
            
            auto tmp_posix_date = std::tm{};
            std::stringstream(game_date_raw) >> std::get_time(&tmp_posix_date, "%Y-%m-%dT%H:%M:%SZ");
            auto date_as_time_t = std::mktime(&tmp_posix_date);

            schedule.emplace_back(
                json::value_to<int>(game_json.at("gamePk")),
                date_as_time_t,
                json::value_to<std::string>(home_team_json.at("team").at("name")),
                json::value_to<int>(home_team_json.at("team").at("id")),
                json::value_to<std::string>(away_team_json.at("team").at("name")),
                json::value_to<int>(away_team_json.at("team").at("id")),
                json::value_to<int>(home_team_json.at("score")),
                json::value_to<int>(away_team_json.at("score")),
                static_cast<NHL::Game::Status>(std::stoi(game_status)),
                json::value_to<std::string>(game_json.at("venue").at("name"))
            );
        }
    };

    return schedule;
}