#include <tsl/sparse-hash>

tsl::sparse_map<int, int> map = {{1, 1}, {2, 1}, {3, 1}};
for(auto it = map.begin(); it != map.end(); ++it) {
    //it->second = 2; // Illegal
    it.value() = 2; // Ok
}