#pragma once
struct SimpleStruct {
    bool                  bool_;
    int                   int_;
    double                double_;
    std::string           string_;
    bool optional_;
    //std::unique_ptr<bool> optional_;
};

struct NestedStruct {
    SimpleStruct              nested_;
    std::vector<SimpleStruct> vector_;
};
DEFINE_STRUCT_SCHEMA(SimpleStruct,
                     DEFINE_STRUCT_FIELD(bool_, "_bool"),
                     DEFINE_STRUCT_FIELD(int_, "_int"),
                     DEFINE_STRUCT_FIELD(double_, "_double"),
                     DEFINE_STRUCT_FIELD(string_, "_string"),
                     DEFINE_STRUCT_FIELD(optional_, "_optional"));

DEFINE_STRUCT_SCHEMA(NestedStruct,
                     DEFINE_STRUCT_FIELD(nested_, "_nested"),
                     DEFINE_STRUCT_FIELD(vector_, "_vector"));