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

#include "statistics/Randomizer.h"
#include "functions/LogGammaFunction.h"
#include "functions/IncompleteGammaQFunction.h"
#include "exceptions/BadArgumentException.h"
#include "exceptions/InvalidOperationException.h"

/******************************************************************************/
/* Constructors and Destructor                                                */
/******************************************************************************/

template <typename T>
InvGammaDistribution<T>::InvGammaDistribution(const T& shape, double scale) {
  setShape(shape);
  setScale(scale);
}

template <typename T>
InvGammaDistribution<T>::InvGammaDistribution(const InvGammaDistribution&
    other) :
    mShape(other.mShape),
    mScale(other.mScale),
    mNormalizer(other.mNormalizer) {
}

template <typename T>
InvGammaDistribution<T>& InvGammaDistribution<T>::operator =
    (const InvGammaDistribution& other) {
  if (this != &other) {
    mShape = other.mShape;
    mScale = other.mScale;
    mNormalizer = other.mNormalizer;
  }
  return *this;
}

template <typename T>
InvGammaDistribution<T>::~InvGammaDistribution() {
}

/******************************************************************************/
/* Stream operations                                                          */
/******************************************************************************/

template <typename T>
void InvGammaDistribution<T>::read(std::istream& stream) {
}

template <typename T>
void InvGammaDistribution<T>::write(std::ostream& stream) const {
  stream << "shape: " << mShape << std::endl
    << "scale: " << mScale;
}

template <typename T>
void InvGammaDistribution<T>::read(std::ifstream& stream) {
}

template <typename T>
void InvGammaDistribution<T>::write(std::ofstream& stream) const {
}

/******************************************************************************/
/* Accessors                                                                  */
/******************************************************************************/

template <typename T>
void InvGammaDistribution<T>::setShape(const T& shape) {
  if (shape <= 0)
    throw BadArgumentException<T>(shape,
      "InvGammaDistribution::setShape(): shape must be strictly positive",
      __FILE__, __LINE__);
  mShape = shape;
  computeNormalizer();
}

template <typename T>
const T& InvGammaDistribution<T>::getShape() const {
  return mShape;
}

template <typename T>
void InvGammaDistribution<T>::setScale(double scale) {
  if (scale <= 0)
    throw BadArgumentException<double>(scale,
      "InvGammaDistribution::setScale(): scale must be strictly positive",
      __FILE__, __LINE__);
  mScale = scale;
  computeNormalizer();
}

template <typename T>
double InvGammaDistribution<T>::getScale() const {
  return mScale;
}

template <typename T>
void InvGammaDistribution<T>::computeNormalizer() {
  const LogGammaFunction<T> logGammaFunction;
  mNormalizer = logGammaFunction(mShape) - mShape * log(mScale);
}

template <typename T>
double InvGammaDistribution<T>::getNormalizer() const {
  return mNormalizer;
}

template <typename T>
double InvGammaDistribution<T>::pdf(const RandomVariable& value) const {
  if (value <= 0)
    return 0.0;
  else 
    return exp(logpdf(value));
}

template <typename T>
double InvGammaDistribution<T>::logpdf(const RandomVariable& value) const {
  return -(mShape + 1) * log(value) - mScale / value - mNormalizer;
}

template <typename T>
double InvGammaDistribution<T>::cdf(const RandomVariable& value) const {
  const IncompleteGammaQFunction incGammaQFunction(mShape);
  if (value <= 0)
    return 0.0;
  else 
    return incGammaQFunction(mScale / value);
}

template <typename T>
typename InvGammaDistribution<T>::RandomVariable
    InvGammaDistribution<T>::getSample() const {
  const static Randomizer<double> randomizer;
  return 1.0 / randomizer.sampleGamma(mShape, mScale);
}

template <typename T>
typename InvGammaDistribution<T>::Mean InvGammaDistribution<T>::getMean() const
    {
  if (mShape > 1)
    return mScale / (mShape - 1);
  else
    throw InvalidOperationException("InvGammaDistribution<T>::getMean(): "
      "shape must be bigger than 1");
}

template <typename T>
typename InvGammaDistribution<T>::Mode InvGammaDistribution<T>::getMode()
    const {
  return mScale / (mShape + 1);
}

template <typename T>
typename InvGammaDistribution<T>::Variance
    InvGammaDistribution<T>::getVariance() const {
  if (mShape > 2)
    return (mScale * mScale) / ((mShape - 1) * (mShape - 1) * (mShape - 2));
  else
    throw InvalidOperationException("InvGammaDistribution<T>::getVariance(): "
      "shape must be bigger than 2");
}
