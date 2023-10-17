#include <iostream>
#include <format>

#include "api_client.hpp"

auto main(int argc, char** argv) -> int {
    const auto args = std::vector<std::string>(argv + 1, argv + argc);
    for(const auto& argument : args) {
        if(argument == "-s" || argument == "--schedule") {
            //return 
        }
    }
    
    try {
        using namespace std::chrono_literals;
        auto api_client = NHLApiClient();

        const auto today = std::chrono::system_clock::now();
        const auto seven_days_later = today + (7 * 24h);

        const auto json_response = api_client.requestDataFromApi(std::format("schedule?startDate={:%Y-%m-%d}&endDate={:%Y-%m-%d}", today, seven_days_later));
        const auto games = api_client.parseSchedule(json_response);

        for(const auto& game : games) {
            const auto date = std::chrono::system_clock::from_time_t(game.date);

            std::cout << std::format("{:%d %b %Y} {} @ {} ", date, game.away_team, game.home_team);
            if(game.status == NHL::Game::Status::Final || game.status == NHL::Game::Status::Live 
            || game.status == NHL::Game::Status::GameOver || game.status == NHL::Game::Status::Final_)
                std::cout << std::format("({:d} : {:d}) {}", game.score_away, game.score_home, 
                (game.status == NHL::Game::Status::Live ? "LIVE " : ""));
            else if(game.status == NHL::Game::Status::Postponed)
                std::cout << "Postponed ";
            else if(game.status == NHL::Game::Status::TBD)
                std::cout << "TBD ";
            else
                std::cout << std::format("{:%R} ", date);
            
            std::cout << std::format("at {}\n", game.venue);
        }
    } 
    catch(std::exception const& e) {
        std::cerr << "[ERROR] " << e.what() << "\n";
        return EXIT_FAILURE; 
    }
    return EXIT_SUCCESS;
}
