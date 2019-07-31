#ifndef CAFFE_L2NORMLIZE_LAYERS_HPP_
#define CAFFE_L2NORMLIZE_LAYERS_HPP_

#include <string>
#include <utility>
#include <vector>

#include "caffe/blob.hpp"
#include "../common.hpp"
#include "../layer.hpp"
#include "../proto/caffe.pb.h"

namespace caffe {
	
	/**
	* @brief Normalizes input.
	*/
	class L2NormalizeLayer : public Layer{
	public:
		explicit L2NormalizeLayer(const LayerParameter& param)
			: Layer(param) {}
		virtual void Reshape(const vector<Blob*>& bottom,
			const vector<Blob*>& top);

		virtual inline const char* type() const { return "L2Normalize"; }
		virtual inline int ExactNumBottomBlobs() const { return 1; }
		virtual inline int ExactNumTopBlobs() const { return 1; }

	protected:
		virtual void Forward_cpu(const vector<Blob*>& bottom,
			const vector<Blob*>& top);
		virtual void Forward_gpu(const vector<Blob*>& bottom,
			const vector<Blob*>& top);

		Blob sum_multiplier_, norm_, squared_;
	};
	
}
#endif
