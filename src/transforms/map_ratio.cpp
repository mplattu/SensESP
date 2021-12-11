#include "map_ratio.h"

// Keys and descriptions of constant parameters

const ParamInfo MapRatio::param_info_[] = {
  {"input_min", "Minimum input value"},
  {"input_max", "Maximum input value"}
};

// Function implementing the linear transform

float (*MapRatio::function_)(float, float, float) =
    [](float input, float input_min, float input_max) { return (float) map(input, input_min, input_max, 0, 1); };

// Constructor definition

MapRatio::MapRatio(float input_min, float input_max, String config_path)
    : LambdaTransform<float, float, float, float>(function_, input_min, input_max, param_info_, config_path) {}
