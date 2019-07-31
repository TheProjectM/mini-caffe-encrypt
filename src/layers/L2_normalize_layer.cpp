#include <algorithm>
#include <vector>
#include <cmath>

//#include "caffe/layer.hpp"
#include "../util/math_functions.hpp"
#include "L2_normalize_layer.hpp"

namespace caffe {

    void L2NormalizeLayer::Reshape(const vector<Blob *> &bottom,
                                   const vector<Blob *> &top) {
        top[0]->Reshape(bottom[0]->num(), bottom[0]->channels(),
                        bottom[0]->height(), bottom[0]->width());
        squared_.Reshape(bottom[0]->num(), bottom[0]->channels(),
                         bottom[0]->height(), bottom[0]->width());
    }


    void L2NormalizeLayer::Forward_cpu(const vector<Blob *> &bottom,
                                       const vector<Blob *> &top) {
        const real_t *bottom_data = bottom[0]->cpu_data();
        real_t *top_data = top[0]->mutable_cpu_data();
        real_t *squared_data = squared_.mutable_cpu_data();
        int n = bottom[0]->num();
        int d = bottom[0]->count() / n;
        caffe_sqr(n * d, bottom_data, squared_data);
        for (int i = 0; i < n; ++i) {
            real_t normsqr = caffe_cpu_asum(d, squared_data + i * d);
            caffe_cpu_scale(d, pow(normsqr, -0.5), bottom_data + i * d, top_data + i * d);
        }
    }


#ifdef CPU_ONLY
    STUB_GPU(L2NormalizeLayer);
#endif

//INSTANTIATE_CLASS(L2NormalizeLayer);
    REGISTER_LAYER_CLASS(L2Normalize);

}  // namespace caffe
