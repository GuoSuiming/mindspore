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
#ifndef MINDSPORE_LITE_SRC_OPS_LAYER_NORM_H_
#define MINDSPORE_LITE_SRC_OPS_LAYER_NORM_H_

#include <vector>
#include <set>
#include <cmath>
#include <memory>

#include "src/ops/primitive_c.h"

namespace mindspore {
namespace lite {
class LayerNorm : public PrimitiveC {
 public:
  LayerNorm() = default;
  ~LayerNorm() = default;
#ifdef PRIMITIVE_WRITEABLE
  MS_DECLARE_PARENT(LayerNorm, PrimitiveC);
  explicit LayerNorm(schema::PrimitiveT *primitive) : PrimitiveC(primitive) {}
  void SetEpsilon(float epsilon);
  void SetBeginNormAxis(int axis);
  void SetBeginParamsAxis(int axis);
  int UnPackAttr(const Primitive &prim, const std::vector<AnfNodePtr> &inputs) override;
#else
  int UnPackToFlatBuilder(const schema::Primitive *primitive, flatbuffers::FlatBufferBuilder *fbb) override;
#endif
  int InferShape(std::vector<lite::Tensor *> inputs_, std::vector<lite::Tensor *> outputs_) override;
  float GetEpsilon() const;
  int GetBeginNormAxis() const;
  int GetBeginParamsAxis() const;
  std::vector<int> GetNormlizedShape() const { return normlized_shape_; }

 protected:
  std::vector<int> normlized_shape_;
};
}  // namespace lite
}  // namespace mindspore

#endif  // MINDSPORE_LITE_SRC_OPS_LAYER_NORM_H_
