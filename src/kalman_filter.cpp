#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

// Please note that the Eigen library does not initialize
// VectorXd or MatrixXd objects with zeros upon creation.

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  /**
  TODO:
    * predict the state
  */
  x_ = F_ * x_;
  MatrixXd Ft = F_.transpose();
  P_ = F_ * P_ * Ft + Q_;
}

void KalmanFilter::CalcError(const VectorXd &z, VectorXd &y) {

  VectorXd z_pred = H_ * x_;
  y = z - z_pred;

}

void KalmanFilter::CalcErrorEKF(const VectorXd &z, VectorXd &y) {

  float px = x_(0);
  float py = x_(1);
  float vx = x_(2);
  float vy = x_(3);

  if(fabs(px) < 0.0001){
    px = 0.0001;
  }
  if(fabs(py) < 0.0001){
    py = 0.0001;
  }

  float rho = sqrt(px*px + py*py);
  if(rho < 0.0001){
    rho = 0.0001;
  }

  float phi = atan2(py, px);

  float rho_dot = (px*vx + py*vy) / rho;

  VectorXd z_pred(3);
  z_pred << rho, phi, rho_dot;

  y = z - z_pred;

  float pi = 3.14159265359;
  while (y(1) < -pi){
    y(1) = y(1) + 2 * pi;
  }
  while(y(1) > pi){
    y(1) = y(1) - 2 * pi;
  }

}

void KalmanFilter::CalcKalmanGain(MatrixXd &K) {

  MatrixXd Ht = H_.transpose();
  MatrixXd S = H_ * P_ * Ht + R_;
  MatrixXd Si = S.inverse();
  MatrixXd PHt = P_ * Ht;
  K = PHt * Si;

}

void KalmanFilter::CalcNewEstimate(const MatrixXd &K, const VectorXd &y) {

  //new estimate
  x_ = x_ + (K * y);
  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K * H_) * P_;

}

void KalmanFilter::Update(const VectorXd &z) {

  VectorXd y(2);
  MatrixXd K(4, 2);

  CalcError(z, y);
  CalcKalmanGain(K);

  //new estimate
  CalcNewEstimate(K, y);
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {

  VectorXd y(3);
  MatrixXd K(4, 3);

  CalcErrorEKF(z, y);
  CalcKalmanGain(K);

  //new estimate
  CalcNewEstimate(K, y);
}


// void KalmanFilter::Update(const VectorXd &z) {
  // [>*
  // TODO:
    // * update the state by using Kalman Filter equations
  // */
  // VectorXd z_pred = H_ * x_;
  // VectorXd y = z - z_pred;
  // MatrixXd Ht = H_.transpose();
  // MatrixXd S = H_ * P_ * Ht + R_;
  // MatrixXd Si = S.inverse();
  // MatrixXd PHt = P_ * Ht;
  // MatrixXd K = PHt * Si;

  // //new estimate
  // x_ = x_ + (K * y);
  // long x_size = x_.size();
  // MatrixXd I = MatrixXd::Identity(x_size, x_size);
  // P_ = (I - K * H_) * P_;
// }

// void KalmanFilter::UpdateEKF(const VectorXd &z) {
  // [>*
  // TODO:
    // * update the state by using Extended Kalman Filter equations
  // */
  // float px = x_(0);
  // float py = x_(1);
  // float vx = x_(2);
  // float vy = x_(3);

  // if(fabs(px) < 0.0001){
    // px = 0.0001;
  // }
  // if(fabs(py) < 0.0001){
    // py = 0.0001;
  // }

  // float rho = sqrt(px*px + py*py);
  // if(rho < 0.0001){
    // rho = 0.0001;
  // }

  // float phi = atan2(py, px);

  // float rho_dot = (px*vx + py*vy) / rho;

  // VectorXd z_pred(3);
  // z_pred << rho, phi, rho_dot;

  // VectorXd y = z - z_pred;

  // float pi = 3.14159265359;
  // while (y(1) < -pi){
    // y(1) = y(1) + 2 * pi;
  // }
  // while(y(1) > pi){
    // y(1) = y(1) - 2 * pi;
  // }

  // MatrixXd Ht = H_.transpose();
  // MatrixXd S = H_ * P_ * Ht + R_;
  // MatrixXd Si = S.inverse();
  // MatrixXd PHt = P_ * Ht;
  // MatrixXd K = PHt * Si;

  // //new estimate
  // x_ = x_ + (K * y);
  // long x_size = x_.size();
  // MatrixXd I = MatrixXd::Identity(x_size, x_size);
  // P_ = (I - K * H_) * P_;
// }
