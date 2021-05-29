#include <cstdio>
#include <cstdlib>
#include <random>

#include <faiss/index_factory.h>
#include <faiss/IndexFlat.h>
#include <faiss/IndexIVFFlat.h>
#include <faiss/IndexIVFPQ.h>
// 64-bit int
using idx_t = faiss::Index::idx_t;
void generate_array(std::vector<long>& array, int n) {
    static  std::random_device rd;
    static std::mt19937 rng(rd());
    static std::uniform_int_distribution<> distrib(1000000UL, INT_MAX);
    array.resize(n);
    for (int i = 0; i < n; ++i) {
        array[i] = distrib(rng);
    }
}
void generate_array(std::vector<float>& array, int d, int nb) {
    static std::mt19937 rng;
    static std::uniform_real_distribution<> distrib;
    array.resize(d*nb);
    for (int i = 0; i < nb; ++i) {
        for (int j = 0; j < d; ++j) {
            array[d * i + j] = distrib(rng);
        }
        array[d * i] = i / 1000;
    }
}
template<typename T>
void print_result(const char* header, const T* array, const char* format, int k, int  begin, int end) {
    printf("%s", header);
    for (int i = begin; i < end; i++) {
        for (int j = 0; j < k; j++)
            printf(format, array[i * k + j]);
        printf("\n");
    }
}
void print_result(const char* header, const idx_t* array, int k, int  begin, int end) {
    print_result(header, array, "%5zd ", k, begin, end);
}
void print_result(const char* header, const float* array, int k, int  begin, int end) {
    print_result(header, array, "%7g ", k, begin, end);
}
void GenerateIndexFlatL2Test(int d, int nb, int nq) {
    printf("########### %s####\n", __FUNCTION__ );
    std::vector<float> xb;
    std::vector<float> xq;
    generate_array(xb, d, nb);
    generate_array(xq, d, nq);
    faiss::IndexFlatL2 index(d); // call constructor
    printf("is_trained = %s\n", index.is_trained ? "true" : "false");
    index.add(nb, xb.data()); // add vectors to the index
    printf("ntotal = %zd\n", index.ntotal);
    int k = 4;
    {
        // sanity check: search 5 first vectors of xb
        idx_t* I = new idx_t[k * 5];
        float* D = new float[k * 5];
        index.search(5, xb.data(), k, D, I);
        // print results
        print_result("I=\n", I, k, 0, 5);
        print_result("D=\n", D, k, 0, 5);
        delete[] I;
        delete[] D;
    }
    { // search xq
        idx_t* I = new idx_t[k * nq];
        float* D = new float[k * nq];
        index.search(nq, xq.data(), k, D, I);
        // print results
        print_result("I (5 first results)=\n", I, k, 0, 5);
        print_result("I (5 last results)=\n", I, k, nq - 5, nq);
        delete[] I;
        delete[] D;
    }
}
void GenerateIndexIVFPQ(int d, int nb, int nq) {
    printf("########### %s####\n", __FUNCTION__ );
    std::vector<float> xb;
    std::vector<float> xq;
    generate_array(xb, d, nb);
    generate_array(xq, d, nq);
    int nlist = 100;
    int k = 4;
    int m = 8;                       // bytes per vector
    faiss::IndexFlatL2 quantizer(d); // the other index
    faiss::IndexIVFPQ index(&quantizer, d, nlist, m, 8);
    index.train(nb, xb.data());
    index.add(nb, xb.data());
    { // sanity check
        idx_t* I = new idx_t[k * 5];
        float* D = new float[k * 5];
        index.search(5, xb.data(), k, D, I);
        print_result("I=\n", I, k, 0, 5);
        print_result("D=\n", D, k, 0, 5);
        delete[] I;
        delete[] D;
    }

    { // search xq
        idx_t* I = new idx_t[k * nq];
        float* D = new float[k * nq];

        index.nprobe = 10;
        index.search(nq, xq.data(), k, D, I);
        print_result("I=\n", I, k, nq - 5, nq);
        delete[] I;
        delete[] D;
    }
    /*
     faiss::write_index(&index, "file.idx");
     auto index = faiss::read_index("file.idx");
     */

}
void GenerateIndexVFlat(int d, int nb, int nq) {
    printf("########### %s####\n", __FUNCTION__ );
    std::vector<float> xb;
    std::vector<float> xq;
    std::vector<long> ids;
    generate_array(ids, nb);
    generate_array(xb, d, nb);
    generate_array(xq, d, nq);

    int nlist = 100;
    int k = 4;
    faiss::IndexFlatL2 quantizer(d); // the other index
    faiss::IndexIVFFlat index(&quantizer, d, nlist);
    assert(!index.is_trained);
    index.train(nb, xb.data());
    assert(index.is_trained);
#if 0
    index.add(nb, xb.data());
#else
    index.add_with_ids (nb, xb.data(), ids.data());
#endif
    { // search xq
        idx_t* I = new idx_t[k * nq];
        float* D = new float[k * nq];

        index.search(nq, xq.data(), k, D, I);
        print_result("I=\n", I, k, nq - 5, nq);
        print_result("D=\n", D, k, nq-5, nq);

        index.nprobe = 10;
        index.search(nq, xq.data(), k, D, I);
        print_result("I=\n", I, k, nq -5, nq);
        delete[] I;
        delete[] D;
    }
}
int main() {
    int d = 64;      // dimension
    int nb = 100000; // database size
    int nq = 10000;  // nb of queries
    //GenerateIndexFlatL2Test(d, nb, nq);
    GenerateIndexVFlat(d, nb, nq);
    //GenerateIndexIVFPQ(d, nb, nq);
    return 0;
}