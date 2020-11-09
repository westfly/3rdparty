#include<iostream>
#include "spdlog/spdlog.h"
#include "cpptoml.h"


int main() {
    auto config = cpptoml::parse_file("../conf/http.toml");
    auto port = config->get_qualified_as<int>("ENV.port");
    spdlog::info("server will start at port {}", *port);
    return 0;
}