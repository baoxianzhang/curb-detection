/******************************************************************************
 * Copyright (C) 2011 by Jerome Maye                                          *
 * jerome.maye@gmail.com                                                      *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the Lesser GNU General Public License as published by*
 * the Free Software Foundation; either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * Lesser GNU General Public License for more details.                        *
 *                                                                            *
 * You should have received a copy of the Lesser GNU General Public License   *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.       *
 ******************************************************************************/

/** \file EstimatorMLMixtureLinearRegression.h
    \brief This file implements an ML estimator for mixture of linear
           regression models
  */

#include "statistics/LinearRegression.h"
#include "statistics/MixtureDistribution.h"

#include <vector>

/** The class EstimatorML is implemented for mixtures of linear regressions.
    \brief Mixture of linear regression ML estimator
  */

template <size_t M, size_t N>
class EstimatorML<MixtureDistribution<LinearRegression<M>, N>, M, N> :
  public virtual Serializable {
public:
  /** \name Types definitions
    @{
    */
  /// Point type
  typedef Eigen::Matrix<double, M, 1> Point;
  /// Points container
  typedef std::vector<Point > Container;
  /// Constant point iterator
  typedef typename Container::const_iterator ConstPointIterator;
  /** @}
    */

  /** \name Constructors/destructor
    @{
    */
  /// Constructs estimator with initial guess of the parameters
  EstimatorML(const Eigen::Matrix<double, N, M>& coefficients, const
    Eigen::Matrix<double, N, 1>& variances, const Eigen::Matrix<double, N, 1>&
    weights, size_t maxNumIter = 100, double tol = 1e-5);
  /// Copy constructor
  EstimatorML(const
    EstimatorML<MixtureDistribution<LinearRegression<M>, N>, M, N>& other);
  /// Assignment operator
  EstimatorML<MixtureDistribution<LinearRegression<M>, N>, M, N>& operator =
    (const EstimatorML<MixtureDistribution<LinearRegression<M>, N>, M, N>&
    other);
  /// Destructor
  virtual ~EstimatorML();
  /** @}
    */

  /** \name Accessors
    @{
    */
  /// Returns the number of points
  size_t getNumPoints() const;
  /// Returns the validity state of the estimator
  bool getValid() const;
  /// Returns the estimated coefficients
  const Eigen::Matrix<double, N, M>& getCoefficients() const;
  /// Returns the estimated variances
  const Eigen::Matrix<double, N, 1>& getVariances() const;
  /// Returns the estimated responsibilities
  const Eigen::Matrix<double, Eigen::Dynamic, N>& getResponsibilities() const;
  /// Returns the estimated component weights
  const Eigen::Matrix<double, N, 1>& getWeights() const;
  /// Returns the tolerance of the estimator
  double getTolerance() const;
  /// Sets the tolerance of the estimator
  void setTolerance(double tol);
  /// Returns the maximum number of iterations for EM
  size_t getMaxNumIter() const;
  /// Sets the maximum number of iterations for EM
  void setMaxNumIter(size_t maxNumIter);
  /// Add points to the estimator / Returns number of EM iterationss
  size_t addPoints(const ConstPointIterator& itStart, const ConstPointIterator&
    itEnd);
  /// Reset the estimator
  void reset();
  /** @}
    */

protected:
  /** \name Stream methods
    @{
    */
  /// Reads from standard input
  virtual void read(std::istream& stream);
  /// Writes to standard output
  virtual void write(std::ostream& stream) const;
  /// Reads from a file
  virtual void read(std::ifstream& stream);
  /// Writes to a file
  virtual void write(std::ofstream& stream) const;
  /** @}
    */

  /** \name Protected members
    @{
    */
  /// Estimated regression coefficients
  Eigen::Matrix<double, N, M> mCoefficients;
  /// Estimated variances
  Eigen::Matrix<double, N, 1> mVariances;
  /// Estimated responsibilities
  Eigen::Matrix<double, Eigen::Dynamic, N> mResponsibilities;
  /// Estimated component weights
  Eigen::Matrix<double, N, 1> mWeights;
  /// Maximum number of iterations for EM
  size_t mMaxNumIter;
  /// Tolerance for the determinant
  double mTol;
  /// Number of points in the estimator
  size_t mNumPoints;
  /// Valid flag
  bool mValid;
  /** @}
    */

};

#include "statistics/EstimatorMLMixtureLinearRegression.tpp"
