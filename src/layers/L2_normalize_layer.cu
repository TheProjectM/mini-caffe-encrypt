#include <vector>

#include "./L2_normalize_layer.hpp"
#include "../util/math_functions.hpp"

namespace caffe {

    void L2NormalizeLayer::Forward_gpu(const vector<Blob *> &bottom,
                                       const vector<Blob *> &top) {
        const real_t *bottom_data = bottom[0]->gpu_data();
        real_t *top_data = top[0]->mutable_gpu_data();
        real_t *squared_data = squared_.mutable_gpu_data();
        real_t normsqr;
        int n = bottom[0]->num();
        int d = bottom[0]->count() / n;
        caffe_gpu_powx(n * d, bottom_data, real_t(2), squared_data);
        for (int i = 0; i < n; ++i) {
            caffe_gpu_asum(d, squared_data + i * d, &normsqr);
            caffe_gpu_scale(d, pow(normsqr, -0.5), bottom_data + i * d, top_data + i * d);
        }
    }

}