#ifndef WAVE_PLANE_HPP
#define WAVE_PLANE_HPP

#include <Eigen/Core>
#include <ceres/ceres.h>
#include "wave/odometry/geometry/assign_jacobians.hpp"
#include "wave/odometry/feature_track.hpp"

namespace wave {

template <int... states>
class PlaneResidual : public ceres::SizedCostFunction<1, 6, states...> {
 public:
    virtual ~PlaneResidual() {}

    PlaneResidual(const uint32_t &pt_id,
                  const uint32_t &feat_idx,
                  const float pt_time,
                  const FeatureTrack *track,
                  const Vec<VecE<MatXf>> *feat_points_T,
                  const Vec<VecE<MatX>> *jacs,
                  const Vec<float> *jac_stamps)
        : pt_id(pt_id),
          feat_idx(feat_idx),
          track(track),
          feat_points_T(feat_points_T),
          jacs(jacs),
          jac_stamps(jac_stamps) {
        auto iter = std::upper_bound(jac_stamps->begin(), jac_stamps->end(), pt_time);

        float T1, T2;
        if (iter == jac_stamps->end()) {
            T2 = *(--iter);
            T1 = *(--iter);
        } else {
            T2 = *iter;
            T1 = *(--iter);
        }

        this->w1 = (T2 - pt_time) / (T2 - T1);
        this->w2 = 1.f - this->w1;

        this->jac_index = static_cast<uint32_t>(iter - jac_stamps->begin());
    }

    virtual bool Evaluate(double const *const *parameters, double *residuals, double **jacobians) const;

 private:
    const uint32_t pt_id, feat_idx;
    const FeatureTrack *track;
    const Vec<VecE<MatXf>> *feat_points_T;

    /// pointer to state jacobians updated by evaluation callback
    /// The jacobians are an interpolation grid
    const Vec<VecE<MatX>> *jacs;
    const Vec<float> *jac_stamps;

    float w1, w2;
    uint32_t jac_index;
};
}

#include "wave/odometry/geometry/impl/plane_impl.hpp"

#endif  // WAVE_PLANE_HPP
