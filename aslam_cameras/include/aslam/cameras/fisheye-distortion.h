#ifndef TEST_FISHEYE_DISTORTION_H_
#define TEST_FISHEYE_DISTORTION_H_

#include <ceres/ceres.h>
#include <Eigen/Core>
#include <glog/logging.h>

#include <aslam/cameras/distortion.h>

namespace aslam {

class TestFisheyeDistortion : public aslam::Distortion {
 public:
  explicit TestFisheyeDistortion(double w) : w_(w) {}
  explicit TestFisheyeDistortion(const Eigen::VectorXd& params) {
    setParameters(params);
  }

  enum {
    kNumOfParams = 1
  };

  inline static constexpr size_t parameterSize() {
    return kNumOfParams;
  }

  inline int minimalDimensions() const {
    return kNumOfParams;
  }

  void setParameters(const Eigen::VectorXd& parameters) {
    w_ = parameters(0);
  }

  inline double* getParametersMutable() {
    return &w_;
  }

  void distort(const Eigen::Matrix<double, 2, 1>* point,
               Eigen::Matrix<double, 2, Eigen::Dynamic>* outJy) const;
  void undistort(Eigen::Matrix<double, 2, 1>* y,
                 Eigen::Matrix<double, 2, Eigen::Dynamic>* outJy) const;

  // templated versions, e.g. for ceres autodiff
  template <typename ScalarType>
  void distort(Eigen::Matrix<ScalarType, 2, 1>* keypoint) const;

  template <typename ScalarType>
  void undistort(Eigen::Matrix<ScalarType, 2, 1>* keypoint) const;

  void distortParameterJacobian(
      Eigen::Matrix<double, 2, 1>* imageY,
      Eigen::Matrix<double, 2, Eigen::Dynamic>* outJd) const;

  virtual bool operator==(const aslam::Distortion& other) const {
  }

  virtual void distort(const Eigen::Matrix<double, 2, 1>*) const {}
  virtual void distort(const Eigen::Matrix<double, 2, 1>&,
                       Eigen::Matrix<double, 2, 1>*) const {}
  virtual void undistort(Eigen::Matrix<double, 2, 1>*) const {}
  virtual void update(const double*) {}

  virtual void getParameters(Eigen::VectorXd* params) const {
    CHECK_NOTNULL(params);
    params->resize(1);
    (*params)(0) = w_;
  }

  virtual bool distortionParametersValid() const {
    // TODO(dymczykm) any constraints on w? positive?
    return true;
  }

 private:
  double w_;
};

} // namespace aslam

#include "fisheye-distortion-inl.h"

#endif /* TEST_FISHEYE_DISTORTION_H_ */
