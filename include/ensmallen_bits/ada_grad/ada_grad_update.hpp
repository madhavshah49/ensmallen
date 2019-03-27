/**
 * @file ada_grad_update.hpp
 * @author Abhinav Moudgil
 *
 * AdaGrad update for Stochastic Gradient Descent.
 *
 * ensmallen is free software; you may redistribute it and/or modify it under
 * the terms of the 3-clause BSD license.  You should have received a copy of
 * the 3-clause BSD license along with ensmallen.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef ENSMALLEN_ADA_GRAD_ADA_GRAD_UPDATE_HPP
#define ENSMALLEN_ADA_GRAD_ADA_GRAD_UPDATE_HPP

namespace ens {

/**
 * Implementation of the AdaGrad update policy. AdaGrad update policy chooses
 * learning rate dynamically by adapting to the data. Hence AdaGrad eliminates
 * the need to manually tune the learning rate.
 *
 * For more information, see the following.
 *
 * @code
 * @article{duchi2011adaptive,
 *   author  = {Duchi, John and Hazan, Elad and Singer, Yoram},
 *   title   = {Adaptive subgradient methods for online learning and
 *              stochastic optimization},
 *   journal = {Journal of Machine Learning Research},
 *   volume  = {12},
 *   number  = {Jul},
 *   pages   = {2121--2159},
 *   year    = {2011}
 * }
 * @endcode
 *
 */
class AdaGradUpdate
{
 public:
  /**
   * Construct the AdaGrad update policy with given epsilon parameter.
   *
   * @param epsilon The epsilon value used to initialise the squared gradient
   *        parameter.
   */
  AdaGradUpdate(const double epsilon = 1e-8) : epsilon(epsilon)
  {
    // Nothing to do.
  }

  //! Get the value used to initialise the squared gradient parameter.
  double Epsilon() const { return epsilon; }
  //! Modify the value used to initialise the squared gradient parameter.
  double& Epsilon() { return epsilon; }

  /**
   * The UpdatePolicyType policy classes must contain an internal 'Policy'
   * template class with two template arguments: MatType and GradType.  This is
   * instantiated at the start of the optimization, and holds parameters
   * specific to an individual optimization.
   */
  template<typename MatType, typename GradType>
  class Policy
  {
   public:
    /**
     * This constructor is called by the SGD optimizer before the start of the
     * iteration update process. In AdaGrad update policy, squared gradient
     * matrix is initialized to the zeros matrix with the same size as gradient
     * matrix (see ens::SGD<>).
     *
     * @param parent Instantiated parent class.
     * @param rows Number of rows in the gradient matrix.
     * @param cols Number of columns in the gradient matrix.
     */
    Policy(AdaGradUpdate& parent, const size_t rows, const size_t cols) :
        parent(parent),
        squaredGradient(rows, cols)
    {
      // Initialize an empty matrix for sum of squares of parameter gradient.
      squaredGradient.zeros();
    }

    /**
     * Update step for SGD. The AdaGrad update adapts the learning rate by
     * performing larger updates for more sparse parameters and smaller updates
     * for less sparse parameters.
     *
     * @param iterate Parameters that minimize the function.
     * @param stepSize Step size to be used for the given iteration.
     * @param gradient The gradient matrix.
     */
    void Update(MatType& iterate,
                const double stepSize,
                const GradType& gradient)
    {
      squaredGradient += (gradient % gradient);
      iterate -= (stepSize * gradient) / (arma::sqrt(squaredGradient) +
          parent.Epsilon());
    }

   private:
    // Instantiated parent class.
    AdaGradUpdate& parent;
    // The squared gradient matrix.
    GradType squaredGradient;
  };

 private:
  // The epsilon value used to initialise the squared gradient parameter.
  double epsilon;
};

} // namespace ens

#endif
