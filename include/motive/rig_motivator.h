// Copyright 2014 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef MOTIVE_RIG_MOTIVATOR_H_
#define MOTIVE_RIG_MOTIVATOR_H_

#include "motive/motivator.h"
#include "motive/rig_processor.h"

namespace motive {

class RigAnim;

class RigMotivator : public Motivator {
 public:
  RigMotivator() {}
  RigMotivator(const MotivatorInit& init, MotiveEngine* engine)
      : Motivator(init, engine, 1) {}

  /// Initialize to the type specified by `init`. The only type defined
  /// within Motive for `init` is RigInit, but you can register your
  /// own RigProcessor classes if you like.
  void Initialize(const MotivatorInit& init, MotiveEngine* engine) {
    InitializeWithDimension(init, engine, 1);
  }

  /// Blend from the current state to the animation specified in `anim`.
  /// Blend time is specified in `anim` itself.
  /// If the current state is unspecified because no animation
  /// has yet been played, snap to `anim`.
  void BlendToAnim(const RigAnim& anim, const SplinePlayback& playback) {
    Processor().BlendToAnim(index_, anim, playback);
  }

  void SetPlaybackRate(float playback_rate) {
    Processor().SetPlaybackRate(index_, playback_rate);
  }

  /// Returns array of matricies: one for each bone position. The matrices are
  /// all in the space of the root bones. That is, the bone hierarchy has been
  /// flattened.
  const mathfu::AffineTransform* GlobalTransforms() const {
    return Processor().GlobalTransforms(index_);
  }

  /// Returns the shell aniamtion that defines this rig. It contains all the
  /// bones and operations-on-those-bones that can be animated.
  ///
  /// Distinction,
  /// Rig: defines the bone heirarchy.
  /// Defining animation: defines the bone heirarchy + operations on each bone.
  /// Operations-on-bone: one of MatrixOperationType, for example, a rotation
  ///     about the x-axis, or a translation along the y-axis. Animations are
  ///     composed of several such operations on each bone. Not every animation
  ///     has all the operations, however. The defining animation is the union
  ///     of all possible operations on each bone.
  const RigAnim* DefiningAnim() const {
    return Processor().DefiningAnim(index_);
  }

  const RigAnim* CurrentAnim() const { return Processor().CurrentAnim(index_); }

  /// Return the time remaining in the current spline animation.
  /// Time units are defined by the user.
  MotiveTime TimeRemaining() const { return Processor().TimeRemaining(index_); }

  std::string CsvHeaderForDebugging() const {
    return Processor().CsvHeaderForDebugging(index_);
  }
  std::string CsvValuesForDebugging() const {
    return Processor().CsvValuesForDebugging(index_);
  }
  std::string LocalTransformsForDebugging(BoneIndex bone) const {
    return Processor().LocalTransformsForDebugging(index_, bone);
  }

 private:
  RigProcessor& Processor() { return *static_cast<RigProcessor*>(processor_); }
  const RigProcessor& Processor() const {
    return *static_cast<const RigProcessor*>(processor_);
  }
};

}  // namespace motive

#endif  // MOTIVE_RIG_MOTIVATOR_H_
