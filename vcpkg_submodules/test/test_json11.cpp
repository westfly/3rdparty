#include <fmt/format.h>
#include <json11.hpp>

int main(int argc, char const* argv[]) {
    const std::string jvalue = R"(
        {
            "predictor":"LogitRegressionPredictor",
            "pullms":{
                "nn_dssm_user":"dssm_2_tower_user",
                "nn_dssm_ad":"dssm_2_tower_ad"
            },
            "schema":{
                "nn_ivr_dssm_combine_schema_ad":"joint/dssm_2_tower_combine_schema_ad",
                "nn_ivr_dssm_combine_schema_user":"joint/dssm_2_tower_combine_schema_user"
            },
            "model":"joint/dssm_2_tower_model"
        }
    )";
    std::string       err_str;
    try {
        auto json = std::move(json11::Json::parse(jvalue, err_str));
        auto json_map = json["schema"].object_items();
        for (auto& [key, value] : json_map) {
            fmt::print("schema: {}\n", key);
        }
        for (auto& [key, value] : json["pullms"].object_items()) {
            fmt::print("pullms: {}\n", key);
        }
    }
    catch (std::exception& ex) {
        printf("Json Parse error: %s:%s\n", ex.what(), err_str.c_str());
    }
    return 0;
}
