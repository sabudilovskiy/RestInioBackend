#include "server/server.h"
#include <fstream>
#include <chrono>
using namespace restinio;
enum class FileType{
    FILE, FOLDER
};

int main()
{
    std::ifstream config_file("config.json", std::ios::in);
    config_file.imbue(std::locale("ru_RU.UTF-8"));
    std::ofstream logger_file("logs.txt", std::ios::out|std::ios::app);
    logger_file.imbue(std::locale("ru_RU.UTF-8"));
    try {
        if (!config_file.is_open()){
            throw std::runtime_error("can't open config.json. File isn't exist or server haven't permissions to access it");
        }
        nlohmann::json config_json = nlohmann::json::parse(config_file);
        if (config_json.is_discarded()){
            throw std::runtime_error("config.json isn't in JSON format");
        }
        auto it_json_connection_info = config_json.find("connection_info");
        if (config_json.end() == it_json_connection_info || !it_json_connection_info->is_string()){
            throw std::runtime_error("Not found connection_info in config.json");
        }
        std::string connection_info;
        it_json_connection_info->get_to(connection_info);
        std::shared_ptr<tao::pq::connection> conn;
        conn = tao::pq::connection::create(connection_info);
        auto router = make_router(conn);
        struct my_server_traits : public restinio::default_single_thread_traits_t {
            using request_handler_t = restinio::router::express_router_t<>;
        };
        restinio::run(restinio::on_this_thread<my_server_traits>()
                .port(80)
                .request_handler(std::move(router)));
    }
    catch (std::exception& errc){
        if (logger_file.is_open())
        {   auto time = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(time);
            logger_file << '[' << std::ctime(&time_t) << ']' << errc.what() << '\n';
            logger_file.flush();
        }

    }
    return 0;
}
