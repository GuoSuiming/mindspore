/**
 * Copyright 2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "nnacl/fp32/arithmetic_fp32.h"
#include <math.h>

#define ACCURACY_DATA 0.00000001

int ElementOptMul(const float *in0, const float *in1, float *out, int size, const ArithmeticParameter *param) {
#ifdef ENABLE_NEON
  float32x4_t vin0_opt = vdupq_n_f32(in0[0]);
  float32x4_t vin1_opt = vdupq_n_f32(in1[0]);
#endif
  int index = 0;
  if (param->in_elements_num0_ == 1) {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin1 = vld1q_f32(in1 + index);
      float32x4_t vout = vmulq_f32(vin0_opt, vin1);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = in0[0] * in1[index];
    }
  } else {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin0 = vld1q_f32(in0 + index);
      float32x4_t vout = vmulq_f32(vin0, vin1_opt);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = in0[index] * in1[0];
    }
  }
  return NNACL_OK;
}

int ElementOptMulRelu(const float *in0, const float *in1, float *out, int size, const ArithmeticParameter *param) {
#ifdef ENABLE_NEON
  float32x4_t vin0_opt = vdupq_n_f32(in0[0]);
  float32x4_t vin1_opt = vdupq_n_f32(in1[0]);
  float32x4_t zeros = vdupq_n_f32(0.0f);
#endif
  int index = 0;
  if (param->in_elements_num0_ == 1) {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin1 = vld1q_f32(in1 + index);
      float32x4_t vout = vmaxq_f32(vmulq_f32(vin0_opt, vin1), zeros);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMAX(in0[0] * in1[index], 0);
    }
  } else {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin0 = vld1q_f32(in0 + index);
      float32x4_t vout = vmaxq_f32(vmulq_f32(vin0, vin1_opt), zeros);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMAX(in0[index] * in1[0], 0);
    }
  }
  return NNACL_OK;
}

int ElementOptMulRelu6(const float *in0, const float *in1, float *out, int size, const ArithmeticParameter *param) {
#ifdef ENABLE_NEON
  float32x4_t vin0_opt = vdupq_n_f32(in0[0]);
  float32x4_t vin1_opt = vdupq_n_f32(in1[0]);
  float32x4_t zeros = vdupq_n_f32(0.0f);
  float32x4_t bounds = vdupq_n_f32(6.0f);
#endif
  int index = 0;
  if (param->in_elements_num0_ == 1) {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin1 = vld1q_f32(in1 + index);
      float32x4_t vout = vminq_f32(vmaxq_f32(vmulq_f32(vin0_opt, vin1), zeros), bounds);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMIN(MSMAX(in0[0] * in1[index], 0), 6);
    }
  } else {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin0 = vld1q_f32(in0 + index);
      float32x4_t vout = vminq_f32(vmaxq_f32(vmulq_f32(vin0, vin1_opt), zeros), bounds);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMIN(MSMAX(in0[index] * in1[0], 0), 6);
    }
  }
  return NNACL_OK;
}

int ElementOptMulInt(const int *in0, const int *in1, int *out, int size, const ArithmeticParameter *param) {
#ifdef ENABLE_NEON
  int32x4_t vin0_opt = vdupq_n_s32(in0[0]);
  int32x4_t vin1_opt = vdupq_n_s32(in1[0]);
#endif
  int index = 0;
  if (param->in_elements_num0_ == 1) {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      int32x4_t vin1 = vld1q_s32(in1 + index);
      int32x4_t vout = vmulq_s32(vin0_opt, vin1);
      vst1q_s32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = in0[0] * in1[index];
    }
  } else {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      int32x4_t vin0 = vld1q_s32(in0 + index);
      int32x4_t vout = vmulq_s32(vin0, vin1_opt);
      vst1q_s32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = in0[index] * in1[0];
    }
  }
  return NNACL_OK;
}

int ElementOptMulReluInt(const int *in0, const int *in1, int *out, int size, const ArithmeticParameter *param) {
#ifdef ENABLE_NEON
  int32x4_t vin0_opt = vdupq_n_s32(in0[0]);
  int32x4_t vin1_opt = vdupq_n_s32(in1[0]);
  int32x4_t zeros = vdupq_n_s32(0);
#endif
  int index = 0;
  if (param->in_elements_num0_ == 1) {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      int32x4_t vin1 = vld1q_s32(in1 + index);
      int32x4_t vout = vmaxq_s32(vmulq_s32(vin0_opt, vin1), zeros);
      vst1q_s32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMAX(in0[0] * in1[index], 0);
    }
  } else {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      int32x4_t vin0 = vld1q_s32(in0 + index);
      int32x4_t vout = vmaxq_s32(vmulq_s32(vin0, vin1_opt), zeros);
      vst1q_s32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMAX(in0[index] * in1[0], 0);
    }
  }
  return NNACL_OK;
}

int ElementOptMulRelu6Int(const int *in0, const int *in1, int *out, int size, const ArithmeticParameter *param) {
#ifdef ENABLE_NEON
  int32x4_t vin0_opt = vdupq_n_s32(in0[0]);
  int32x4_t vin1_opt = vdupq_n_s32(in1[0]);
  int32x4_t zeros = vdupq_n_s32(0);
  int32x4_t bounds = vdupq_n_s32(6);
#endif
  int index = 0;
  if (param->in_elements_num0_ == 1) {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      int32x4_t vin1 = vld1q_s32(in1 + index);
      int32x4_t vout = vminq_s32(vmaxq_s32(vmulq_s32(vin0_opt, vin1), zeros), bounds);
      vst1q_s32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMIN(MSMAX(in0[0] * in1[index], 0), 6);
    }
  } else {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      int32x4_t vin0 = vld1q_s32(in0 + index);
      int32x4_t vout = vminq_s32(vmaxq_s32(vmulq_s32(vin0, vin1_opt), zeros), bounds);
      vst1q_s32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMIN(MSMAX(in0[index] * in1[0], 0), 6);
    }
  }
  return NNACL_OK;
}

int ElementOptSub(const float *in0, const float *in1, float *out, int size, const ArithmeticParameter *param) {
#ifdef ENABLE_NEON
  float32x4_t vin0_opt = vdupq_n_f32(in0[0]);
  float32x4_t vin1_opt = vdupq_n_f32(in1[0]);
#endif
  int index = 0;
  if (param->in_elements_num0_ == 1) {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin1 = vld1q_f32(in1 + index);
      float32x4_t vout = vsubq_f32(vin0_opt, vin1);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = in0[0] - in1[index];
    }
  } else {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin0 = vld1q_f32(in0 + index);
      float32x4_t vout = vsubq_f32(vin0, vin1_opt);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = in0[index] - in1[0];
    }
  }
  return NNACL_OK;
}

int ElementOptSubInt(const int *in0, const int *in1, int *out, int size, const ArithmeticParameter *param) {
#ifdef ENABLE_NEON
  int32x4_t vin0_opt = vdupq_n_s32(in0[0]);
  int32x4_t vin1_opt = vdupq_n_s32(in1[0]);
#endif
  int index = 0;
  if (param->in_elements_num0_ == 1) {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      int32x4_t vin1 = vld1q_s32(in1 + index);
      int32x4_t vout = vsubq_s32(vin0_opt, vin1);
      vst1q_s32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = in0[0] - in1[index];
    }
  } else {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      int32x4_t vin0 = vld1q_s32(in0 + index);
      int32x4_t vout = vsubq_s32(vin0, vin1_opt);
      vst1q_s32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = in0[index] - in1[0];
    }
  }
  return NNACL_OK;
}

int ElementOptSubRelu(const float *in0, const float *in1, float *out, int size, const ArithmeticParameter *param) {
#ifdef ENABLE_NEON
  float32x4_t vin0_opt = vdupq_n_f32(in0[0]);
  float32x4_t vin1_opt = vdupq_n_f32(in1[0]);
  float32x4_t zeros = vdupq_n_f32(0.0f);
#endif
  int index = 0;
  if (param->in_elements_num0_ == 1) {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin1 = vld1q_f32(in1 + index);
      float32x4_t vout = vmaxq_f32(vsubq_f32(vin0_opt, vin1), zeros);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMAX(in0[0] - in1[index], 0);
    }
  } else {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin0 = vld1q_f32(in0 + index);
      float32x4_t vout = vmaxq_f32(vsubq_f32(vin0, vin1_opt), zeros);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMAX(in0[index] - in1[0], 0);
    }
  }
  return NNACL_OK;
}

int ElementOptSubRelu6(const float *in0, const float *in1, float *out, int size, const ArithmeticParameter *param) {
#ifdef ENABLE_NEON
  float32x4_t vin0_opt = vdupq_n_f32(in0[0]);
  float32x4_t vin1_opt = vdupq_n_f32(in1[0]);
  float32x4_t zeros = vdupq_n_f32(0.0f);
  float32x4_t bounds = vdupq_n_f32(6.0f);
#endif
  int index = 0;
  if (param->in_elements_num0_ == 1) {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin1 = vld1q_f32(in1 + index);
      float32x4_t vout = vminq_f32(vmaxq_f32(vsubq_f32(vin0_opt, vin1), zeros), bounds);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMIN(MSMAX(in0[0] - in1[index], 0), 6);
    }
  } else {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin0 = vld1q_f32(in0 + index);
      float32x4_t vout = vminq_f32(vmaxq_f32(vsubq_f32(vin0, vin1_opt), zeros), bounds);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMIN(MSMAX(in0[index] - in1[0], 0), 6);
    }
  }
  return NNACL_OK;
}

int ElementOptAdd(const float *in0, const float *in1, float *out, int size, const ArithmeticParameter *param) {
#ifdef ENABLE_NEON
  float32x4_t vin0_opt = vdupq_n_f32(in0[0]);
  float32x4_t vin1_opt = vdupq_n_f32(in1[0]);
#endif
  int index = 0;
  if (param->in_elements_num0_ == 1) {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin1 = vld1q_f32(in1 + index);
      float32x4_t vout = vaddq_f32(vin0_opt, vin1);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = in0[0] + in1[index];
    }
  } else {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin0 = vld1q_f32(in0 + index);
      float32x4_t vout = vaddq_f32(vin0, vin1_opt);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = in0[index] + in1[0];
    }
  }
  return NNACL_OK;
}

int ElementOptAddInt(const int *in0, const int *in1, int *out, int size, const ArithmeticParameter *param) {
#ifdef ENABLE_NEON
  int32x4_t vin0_opt = vdupq_n_s32(in0[0]);
  int32x4_t vin1_opt = vdupq_n_s32(in1[0]);
#endif
  int index = 0;
  if (param->in_elements_num0_ == 1) {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      int32x4_t vin1 = vld1q_s32(in1 + index);
      int32x4_t vout = vaddq_s32(vin0_opt, vin1);
      vst1q_s32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = in0[0] + in1[index];
    }
  } else {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      int32x4_t vin0 = vld1q_s32(in0 + index);
      int32x4_t vout = vaddq_s32(vin0, vin1_opt);
      vst1q_s32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = in0[index] + in1[0];
    }
  }
  return NNACL_OK;
}

int ElementOptAddRelu(const float *in0, const float *in1, float *out, int size, const ArithmeticParameter *param) {
#ifdef ENABLE_NEON
  float32x4_t vin0_opt = vdupq_n_f32(in0[0]);
  float32x4_t vin1_opt = vdupq_n_f32(in1[0]);
  float32x4_t zeros = vdupq_n_f32(0.0f);
#endif
  int index = 0;
  if (param->in_elements_num0_ == 1) {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin1 = vld1q_f32(in1 + index);
      float32x4_t vout = vmaxq_f32(vaddq_f32(vin0_opt, vin1), zeros);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMAX(in0[0] + in1[index], 0);
    }
  } else {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin0 = vld1q_f32(in0 + index);
      float32x4_t vout = vmaxq_f32(vaddq_f32(vin0, vin1_opt), zeros);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMAX(in0[index] + in1[0], 0);
    }
  }
  return NNACL_OK;
}

int ElementOptAddRelu6(const float *in0, const float *in1, float *out, int size, const ArithmeticParameter *param) {
#ifdef ENABLE_NEON
  float32x4_t vin0_opt = vdupq_n_f32(in0[0]);
  float32x4_t vin1_opt = vdupq_n_f32(in1[0]);
  float32x4_t zeros = vdupq_n_f32(0.0f);
  float32x4_t bounds = vdupq_n_f32(6.0f);
#endif
  int index = 0;
  if (param->in_elements_num0_ == 1) {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin1 = vld1q_f32(in1 + index);
      float32x4_t vout = vminq_f32(vmaxq_f32(vaddq_f32(vin0_opt, vin1), zeros), bounds);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMIN(MSMAX(in0[0] + in1[index], 0), 6);
    }
  } else {
#ifdef ENABLE_NEON
    for (; index <= size - 4; index += C4NUM) {
      float32x4_t vin0 = vld1q_f32(in0 + index);
      float32x4_t vout = vminq_f32(vmaxq_f32(vaddq_f32(vin0, vin1_opt), zeros), bounds);
      vst1q_f32(out + index, vout);
    }
#endif
    for (; index < size; index++) {
      out[index] = MSMIN(MSMAX(in0[index] + in1[0], 0), 6);
    }
  }

  return NNACL_OK;
}

int ElementOptDiv(const float *in0, const float *in1, float *out, int size, const ArithmeticParameter *param) {
  if (param->in_elements_num0_ == 1) {
    for (int index = 0; index < size; index++) {
      out[index] = in0[0] / in1[index];
    }
  } else {
    if (in1[0] == 0) {
      return NNACL_ERRCODE_DIVISOR_ZERO;
    }
    for (int index = 0; index < size; index++) {
      out[index] = in0[index] / in1[0];
    }
  }
  return NNACL_OK;
}

int ElementOptDivRelu(const float *in0, const float *in1, float *out, int size, const ArithmeticParameter *param) {
  if (param->in_elements_num0_ == 1) {
    for (int index = 0; index < size; index++) {
      out[index] = in0[0] / in1[index];
      out[index] = out[index] > 0 ? out[index] : 0;
    }
  } else {
    for (int index = 0; index < size; index++) {
      out[index] = in0[index] / in1[0];
      out[index] = out[index] > 0 ? out[index] : 0;
    }
  }
  return NNACL_OK;
}

int ElementOptDivRelu6(const float *in0, const float *in1, float *out, int size, const ArithmeticParameter *param) {
  if (param->in_elements_num0_ == 1) {
    for (int index = 0; index < size; index++) {
      out[index] = MSMIN(MSMAX(in0[0] / in1[index], 0), 6);
    }
  } else {
    for (int index = 0; index < size; index++) {
      out[index] = MSMIN(MSMAX(in0[index] / in1[0], 0), 6);
    }
  }
  return NNACL_OK;
}

int ElementOptDivInt(const int *in0, const int *in1, int *out, int size, const ArithmeticParameter *param) {
  if (param->in_elements_num0_ == 1) {
    for (int index = 0; index < size; index++) {
      out[index] = in0[0] / in1[index];
    }
  } else {
    if (in1[0] == 0) {
      return NNACL_ERRCODE_DIVISOR_ZERO;
    }
    for (int index = 0; index < size; index++) {
      out[index] = in0[index] / in1[0];
    }
  }
  return NNACL_OK;
}

int BroadcastAdd(const float *in0, const float *in1, float *tile_in0, float *tile_in1, float *out, int size,
                 ArithmeticParameter *param) {
  TileDimensionsFp32(in0, in1, tile_in0, tile_in1, param);
  return ElementAdd(tile_in0, tile_in1, out, size);
}

int BroadcastMul(const float *in0, const float *in1, float *tile_in0, float *tile_in1, float *out, int size,
                 ArithmeticParameter *param) {
  TileDimensionsFp32(in0, in1, tile_in0, tile_in1, param);
  return ElementMul(tile_in0, tile_in1, out, size);
}

int ElementMul(const float *in0, const float *in1, float *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  for (; index <= size - 4; index += C4NUM) {
    float32x4_t vin0 = vld1q_f32(in0 + index);
    float32x4_t vin1 = vld1q_f32(in1 + index);
    float32x4_t vout = vmulq_f32(vin0, vin1);
    vst1q_f32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    out[index] = in0[index] * in1[index];
  }
  return NNACL_OK;
}

int ElementMulRelu(const float *in0, const float *in1, float *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  float32x4_t zeros = vdupq_n_f32(0.0f);
  for (; index <= size - 4; index += C4NUM) {
    float32x4_t vin0 = vld1q_f32(in0 + index);
    float32x4_t vin1 = vld1q_f32(in1 + index);
    float32x4_t vout = vmulq_f32(vin0, vin1);
    vout = vbslq_f32(vcgtq_f32(vout, zeros), vout, zeros);
    vst1q_f32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    float res = in0[index] * in1[index];
    out[index] = res > 0 ? res : 0;
  }
  return NNACL_OK;
}

int ElementMulRelu6(const float *in0, const float *in1, float *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  float32x4_t zeros = vdupq_n_f32(0.0f);
  float32x4_t bounds = vdupq_n_f32(6.0f);
  for (; index <= size - 4; index += C4NUM) {
    float32x4_t vin0 = vld1q_f32(in0 + index);
    float32x4_t vin1 = vld1q_f32(in1 + index);
    float32x4_t vout = vminq_f32(vmaxq_f32(vmulq_f32(vin0, vin1), zeros), bounds);
    vst1q_f32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    out[index] = MSMIN(MSMAX(in0[index] * in1[index], 0), 6);
  }
  return NNACL_OK;
}

int ElementMulInt(const int *in0, const int *in1, int *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  for (; index <= size - 4; index += C4NUM) {
    int32x4_t vin0 = vld1q_s32(in0 + index);
    int32x4_t vin1 = vld1q_s32(in1 + index);
    int32x4_t vout = vmulq_s32(vin0, vin1);
    vst1q_s32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    out[index] = in0[index] * in1[index];
  }
  return NNACL_OK;
}

int ElementMulReluInt(const int *in0, const int *in1, int *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  int32x4_t zeros = vdupq_n_s32(0);
  for (; index <= size - 4; index += C4NUM) {
    int32x4_t vin0 = vld1q_s32(in0 + index);
    int32x4_t vin1 = vld1q_s32(in1 + index);
    int32x4_t vout = vmulq_s32(vin0, vin1);
    vout = vbslq_s32(vcgtq_s32(vout, zeros), vout, zeros);
    vst1q_s32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    float res = in0[index] * in1[index];
    out[index] = res > 0 ? res : 0;
  }
  return NNACL_OK;
}

int ElementMulRelu6Int(const int *in0, const int *in1, int *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  int32x4_t zeros = vdupq_n_s32(0);
  int32x4_t bounds = vdupq_n_s32(6);
  for (; index <= size - 4; index += C4NUM) {
    int32x4_t vin0 = vld1q_s32(in0 + index);
    int32x4_t vin1 = vld1q_s32(in1 + index);
    int32x4_t vout = vminq_s32(vmaxq_s32(vmulq_s32(vin0, vin1), zeros), bounds);
    vst1q_s32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    out[index] = MSMIN(MSMAX(in0[index] * in1[index], 0), 6);
  }
  return NNACL_OK;
}

int ElementAdd(const float *in0, const float *in1, float *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  for (; index <= size - 4; index += C4NUM) {
    float32x4_t vin0 = vld1q_f32(in0 + index);
    float32x4_t vin1 = vld1q_f32(in1 + index);
    float32x4_t vout = vaddq_f32(vin0, vin1);
    vst1q_f32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    out[index] = in0[index] + in1[index];
  }
  return NNACL_OK;
}

int ElementAddRelu(const float *in0, const float *in1, float *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  float32x4_t zeros = vdupq_n_f32(0.0f);
  for (; index <= size - 4; index += C4NUM) {
    float32x4_t vin0 = vld1q_f32(in0 + index);
    float32x4_t vin1 = vld1q_f32(in1 + index);
    float32x4_t vout = vaddq_f32(vin0, vin1);
    vout = vbslq_f32(vcgtq_f32(vout, zeros), vout, zeros);
    vst1q_f32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    float res = in0[index] + in1[index];
    out[index] = res > 0 ? res : 0;
  }
  return NNACL_OK;
}

int ElementAddRelu6(const float *in0, const float *in1, float *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  float32x4_t zeros = vdupq_n_f32(0.0f);
  float32x4_t bounds = vdupq_n_f32(6.0f);
  for (; index <= size - 4; index += C4NUM) {
    float32x4_t vin0 = vld1q_f32(in0 + index);
    float32x4_t vin1 = vld1q_f32(in1 + index);
    float32x4_t vout = vminq_f32(vmaxq_f32(vaddq_f32(vin0, vin1), zeros), bounds);
    vst1q_f32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    out[index] = MSMIN(MSMAX(in0[index] + in1[index], 0), 6);
  }
  return NNACL_OK;
}

int ElementAddInt(const int *in0, const int *in1, int *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  for (; index <= size - 4; index += C4NUM) {
    int32x4_t vin0 = vld1q_s32(in0 + index);
    int32x4_t vin1 = vld1q_s32(in1 + index);
    int32x4_t vout = vaddq_s32(vin0, vin1);
    vst1q_s32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    out[index] = in0[index] + in1[index];
  }
  return NNACL_OK;
}

int ElementSub(const float *in0, const float *in1, float *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  for (; index <= size - 4; index += C4NUM) {
    float32x4_t vin0 = vld1q_f32(in0 + index);
    float32x4_t vin1 = vld1q_f32(in1 + index);
    float32x4_t vout = vsubq_f32(vin0, vin1);
    vst1q_f32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    out[index] = in0[index] - in1[index];
  }
  return NNACL_OK;
}

int ElementSubInt(const int *in0, const int *in1, int *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  for (; index <= size - 4; index += C4NUM) {
    int32x4_t vin0 = vld1q_s32(in0 + index);
    int32x4_t vin1 = vld1q_s32(in1 + index);
    int32x4_t vout = vsubq_s32(vin0, vin1);
    vst1q_s32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    out[index] = in0[index] - in1[index];
  }
  return NNACL_OK;
}

int ElementSubRelu(const float *in0, const float *in1, float *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  float32x4_t zeros = vdupq_n_f32(0.0f);
  for (; index <= size - 4; index += C4NUM) {
    float32x4_t vin0 = vld1q_f32(in0 + index);
    float32x4_t vin1 = vld1q_f32(in1 + index);
    float32x4_t vout = vsubq_f32(vin0, vin1);
    vout = vbslq_f32(vcgtq_f32(vout, zeros), vout, zeros);
    vst1q_f32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    float res = in0[index] - in1[index];
    out[index] = res > 0 ? res : 0;
  }
  return NNACL_OK;
}

int ElementSubRelu6(const float *in0, const float *in1, float *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  float32x4_t zeros = vdupq_n_f32(0.0f);
  float32x4_t bounds = vdupq_n_f32(6.0f);
  for (; index <= size - 4; index += C4NUM) {
    float32x4_t vin0 = vld1q_f32(in0 + index);
    float32x4_t vin1 = vld1q_f32(in1 + index);
    float32x4_t vout = vminq_f32(vmaxq_f32(vsubq_f32(vin0, vin1), zeros), bounds);
    vst1q_f32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    out[index] = MSMIN(MSMAX(in0[index] - in1[index], 0), 6);
  }

  return NNACL_OK;
}

int BroadcastDiv(const float *in0, const float *in1, float *tile_in0, float *tile_in1, float *out, int size,
                 ArithmeticParameter *param) {
  TileDimensionsFp32(in0, in1, tile_in0, tile_in1, param);
  return ElementDiv(tile_in0, tile_in1, out, size);
}

int ElementDiv(const float *in0, const float *in1, float *out, int size) {
  for (int i = 0; i < size; i++) {
    out[i] = in0[i] / in1[i];
  }
  return NNACL_OK;
}

int ElementDivRelu(const float *in0, const float *in1, float *out, int size) {
  for (int i = 0; i < size; i++) {
    float res = in0[i] / in1[i];
    out[i] = res > 0 ? res : 0;
  }
  return NNACL_OK;
}

int ElementDivRelu6(const float *in0, const float *in1, float *out, int size) {
  for (int i = 0; i < size; i++) {
    out[i] = MSMIN(MSMAX(in0[i] / in1[i], 0), 6);
  }
  return NNACL_OK;
}

int ElementFloorMod(const float *in0, const float *in1, float *out, int size) {
  for (int i = 0; i < size; i++) {
    out[i] = in0[i] - floorf(in0[i] / in1[i]) * in1[i];
  }
  return NNACL_OK;
}

int ElementFloorModInt(const int *in0, const int *in1, int *out, int size) {
  for (int i = 0; i < size; i++) {
    out[i] = in0[i] - (in0[i] / in1[i]) * in1[i];
  }
  return NNACL_OK;
}

int ElementMod(const float *in0, const float *in1, float *out, int size) {
  for (int i = 0; i < size; i++) {
    out[i] = fmod(in0[i], in1[i]);
  }
  return NNACL_OK;
}

int ElementModInt(const int *in0, const int *in1, int *out, int size) {
  for (int i = 0; i < size; i++) {
    out[i] = fmod(in0[i], in1[i]);
  }
  return NNACL_OK;
}

int ElementOptMod(const float *in0, const float *in1, float *out, int size, const ArithmeticParameter *param) {
  if (param->in_elements_num0_ == 1) {
    for (int index = 0; index < size; index++) {
      out[index] = fmod(in0[0], in1[index]);
    }
  } else {
    for (int index = 0; index < size; index++) {
      out[index] = fmod(in0[index], in1[0]);
    }
  }
  return NNACL_OK;
}

int ElementOptModInt(const int *in0, const int *in1, int *out, int size, const ArithmeticParameter *param) {
  if (param->in_elements_num0_ == 1) {
    for (int index = 0; index < size; index++) {
      out[index] = fmod(in0[0], in1[index]);
    }
  } else {
    for (int index = 0; index < size; index++) {
      out[index] = fmod(in0[index], in1[0]);
    }
  }
  return NNACL_OK;
}

int ElementFloorDiv(const float *in0, const float *in1, float *out, int size) {
  for (int i = 0; i < size; i++) {
    out[i] = floorf(in0[i] / in1[i]);
  }
  return NNACL_OK;
}

int ElementFloorDivInt(const int *in0, const int *in1, int *out, int size) {
  for (int i = 0; i < size; i++) {
    out[i] = in0[i] / in1[i];
  }
  return NNACL_OK;
}

int ElementLogicalAnd(const float *in0, const float *in1, float *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  float32x4_t vtrue = vdupq_n_f32(1);
  float32x4_t vfalse = vdupq_n_f32(0);
  uint32x4_t mask = vmovq_n_u32(((uint32_t)(1u << 31) - 1));
  uint32x4_t zeros = vdupq_n_u32(0);
  for (; index <= size - 4; index += C4NUM) {
    uint32x4_t vin0 = vandq_u32(vreinterpretq_s32_f32(vld1q_f32(in0 + index)), mask);
    uint32x4_t vin1 = vandq_u32(vreinterpretq_s32_f32(vld1q_f32(in1 + index)), mask);
    float32x4_t vout = vbslq_f32(vceqq_u32(vandq_u32(vin0, vin1), zeros), vfalse, vtrue);
    vst1q_f32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    out[index] = (float)((bool)(in0[index]) & (bool)(in1[index]));
  }
  return NNACL_OK;
}

int ElementLogicalAndInt(const int *in0, const int *in1, int *out, int size) {
  int index = 0;
  for (; index < size; index++) {
    out[index] = (int)((unsigned int)(in0[index]) & (unsigned int)(in1[index]));
  }
  return NNACL_OK;
}

int ElementLogicalAndBool(const bool *in0, const bool *in1, bool *out, int size) {
  int index = 0;
  for (; index < size; index++) {
    out[index] = (bool)((unsigned int)(in0[index]) & (unsigned int)(in1[index]));
  }
  return NNACL_OK;
}

int ElementSquaredDifference(const float *in0, const float *in1, float *out, int size) {
  ElementSub(in0, in1, out, size);
  return ElementMul(out, out, out, size);
}

int ElementLogicalOr(const float *in0, const float *in1, float *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  float32x4_t vtrue = vdupq_n_f32(1);
  float32x4_t vfalse = vdupq_n_f32(0);
  uint32x4_t mask = vmovq_n_u32(((uint32_t)(1u << 31) - 1));
  uint32x4_t zeros = vdupq_n_u32(0);
  for (; index <= size - 4; index += C4NUM) {
    uint32x4_t vin0 = vandq_u32(vreinterpretq_s32_f32(vld1q_f32(in0 + index)), mask);
    uint32x4_t vin1 = vandq_u32(vreinterpretq_s32_f32(vld1q_f32(in1 + index)), mask);
    float32x4_t vout = vbslq_f32(vceqq_u32(vorrq_u32(vin0, vin1), zeros), vfalse, vtrue);
    vst1q_f32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    out[index] = (float)((bool)(in0[index]) | (bool)(in1[index]));
  }
  return NNACL_OK;
}

int ElementMaximum(const float *in0, const float *in1, float *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  for (; index <= size - 4; index += C4NUM) {
    float32x4_t vin0 = vld1q_f32(in0 + index);
    float32x4_t vin1 = vld1q_f32(in1 + index);
    float32x4_t vout = vmaxq_f32(vin0, vin1);
    vst1q_f32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    out[index] = in0[index] > in1[index] ? in0[index] : in1[index];
  }
  return NNACL_OK;
}

int ElementMaximumInt(const int *in0, const int *in1, int *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  for (; index <= size - 4; index += C4NUM) {
    int32x4_t vin0 = vld1q_s32(in0 + index);
    int32x4_t vin1 = vld1q_s32(in1 + index);
    int32x4_t vout = vmaxq_s32(vin0, vin1);
    vst1q_s32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    out[index] = in0[index] > in1[index] ? in0[index] : in1[index];
  }
  return NNACL_OK;
}

int ElementMinimumInt(const int *input0, const int *input1, int *output, const int element_size) {
  int index = 0;
#ifdef ENABLE_NEON
  for (; index <= element_size - 4; index += C4NUM) {
    int32x4_t vin0 = vld1q_s32(input0 + index);
    int32x4_t vin1 = vld1q_s32(input1 + index);
    int32x4_t vout = vminq_s32(vin0, vin1);
    vst1q_s32(output + index, vout);
  }
#endif
  for (; index < element_size; index++) {
    output[index] = input0[index] > input1[index] ? input1[index] : input0[index];
  }
  return NNACL_OK;
}

int ElementMinimum(const float *in0, const float *in1, float *out, int size) {
  int index = 0;
#ifdef ENABLE_NEON
  for (; index <= size - 4; index += C4NUM) {
    float32x4_t vin0 = vld1q_f32(in0 + index);
    float32x4_t vin1 = vld1q_f32(in1 + index);
    float32x4_t vout = vminq_f32(vin0, vin1);
    vst1q_f32(out + index, vout);
  }
#endif
  for (; index < size; index++) {
    out[index] = in0[index] > in1[index] ? in1[index] : in0[index];
  }
  return NNACL_OK;
}

#undef ACCURACY_DATA

void TileOneDimensionFp32(const float *inData, float *outData, int dim, size_t ndim, const int *inShape,
                          const int *inStrides, const int *outStrides, const int *multiple) {
  int srcDimSize = inShape[dim];
  if (dim == ndim - 1) {
    for (int i = 0; i < multiple[dim]; i++) {
      memcpy(outData, inData, srcDimSize * sizeof(float));
      outData += srcDimSize;
    }
    return;
  }
  for (size_t i = 0; i < srcDimSize; i++) {
    for (size_t j = 0; j < multiple[dim]; j++) {
      TileOneDimensionFp32(inData + inStrides[dim] * i, outData + outStrides[dim] * (i + j * srcDimSize), dim + 1, ndim,
                           inShape, inStrides, outStrides, multiple);
    }
  }
}

void TileDimensionsFp32(const float *data0, const float *data1, float *tile_data0, float *tile_data1,
                        ArithmeticParameter *param) {
  CalcMultiplesAndStrides(param);
  TileOneDimensionFp32(data0, tile_data0, 0, param->ndim_, param->in_shape0_, param->in_strides0_, param->out_strides_,
                       param->multiples0_);
  TileOneDimensionFp32(data1, tile_data1, 0, param->ndim_, param->in_shape1_, param->in_strides1_, param->out_strides_,
                       param->multiples1_);
}
