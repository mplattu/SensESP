#ifndef _map_ratio_H_
#define _map_ratio_H_

#include "lambda_transform.h"

/**
 * @brief Performs a map transform on the input value:
 * output = map(input_min, input_max, 1, 0)
 *
 * @param input_min Minimum value of the range.
 *
 * @param input_max Maximum value of the range.
 *
 * @param config_path The path to configure this transform in the Config UI
 **/
class MapRatio : public LambdaTransform<float, float, float, float> {
 public:
  MapRatio(float input_min, float input_max, String config_path = "");

 private:
  static float (*function_)(float, float, float);
  static const ParamInfo param_info_[];
};

#endif
