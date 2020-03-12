#ifndef KALMAN_EXAMPLES1_ROBOT_SYSTEMMODEL_HPP_
#define KALMAN_EXAMPLES1_ROBOT_SYSTEMMODEL_HPP_

#include <kalman/LinearizedSystemModel.hpp>
#include <random>
#include <chrono>
#include <iostream>
#include <math.h>


namespace KalmanExamples
{
namespace Robot1
{

/**
 * @brief System state vector-type for a 3DOF planar robot
 *
 * This is a system state for a very simple planar robot that
 * is characterized by its (x,y)-Position and angular orientation.
 *
 * @param T Numeric scalar type
 */
template<typename T>
class State : public Kalman::Vector<T, 3>
{
public:
    KALMAN_VECTOR(State, T, 3)
    
    //! X-position
    static constexpr size_t X = 0;
    //! Y-Position
    static constexpr size_t Y = 1;
    //! Orientation
    static constexpr size_t THETA = 2;
    // //! X-velocity
    // static constexpr size_t dX = 3;
    // //! Y-velocity
    // static constexpr size_t dY = 4;
    // //! Orientation velocity
    // static constexpr size_t dTHETA = 5;
    
    T x()        const { return (*this)[ X ]; }
    T y()        const { return (*this)[ Y ]; }
    T theta()    const { return (*this)[ THETA ]; }
    // T dx()       const { return (*this)[ dX ]; }
    // T dy()       const { return (*this)[ dY ]; }
    // T dtheta()   const { return (*this)[ dTHETA ]; }
    
    T& x()      { return (*this)[ X ]; }
    T& y()      { return (*this)[ Y ]; }
    T& theta()  { return (*this)[ THETA ]; }
    // T& dx()     { return (*this)[ dX ]; }
    // T& dy()     { return (*this)[ dY ]; }
    // T& dtheta() { return (*this)[ dTHETA ]; }

    T xymod()   { return sqrt((*this)[ X ]*(*this)[ X ] + (*this)[ Y ]*(*this)[ Y ]);}
};

/**
 * @brief System control-input vector-type for a 3DOF planar robot
 *
 * This is the system control-input of a very simple planar robot that
 * can control the velocity in its current direction as well as the
 * change in direction.
 *
 * @param T Numeric scalar type
 */
template<typename T>
class Control : public Kalman::Vector<T, 2>
{
public:
    KALMAN_VECTOR(Control, T, 2)
    
    //! Velocity
    static constexpr size_t V = 0;
    //! Angular Rate (Orientation-change)
    static constexpr size_t DTHETA = 1;
    
    T v()       const { return (*this)[ V ]; }
    T dtheta()  const { return (*this)[ DTHETA ]; }
    
    T& v()      { return (*this)[ V ]; }
    T& dtheta() { return (*this)[ DTHETA ]; }
};

/**
 * @brief System model for a simple planar 3DOF robot
 *
 * This is the system model defining how our robot moves from one 
 * time-step to the next, i.e. how the system state evolves over time.
 *
 * @param T Numeric scalar type
 * @param CovarianceBase Class template to determine the covariance representation
 *                       (as covariance matrix (StandardBase) or as lower-triangular
 *                       coveriace square root (SquareRootBase))
 */
template<typename T, template<class> class CovarianceBase = Kalman::StandardBase>
class SystemModel : public Kalman::LinearizedSystemModel<State<T>, Control<T>, CovarianceBase>
{
public:
    //! State type shortcut definition
	typedef KalmanExamples::Robot1::State<T> S;
    //! Control type shortcut definition
    typedef KalmanExamples::Robot1::Control<T> C;

    /**
     * @brief Definition of (non-linear) state transition function
     *
     * This function defines how the system state is propagated through time,
     * i.e. it defines in which state \f$\hat{x}_{k+1}\f$ is system is expected to 
     * be in time-step \f$k+1\f$ given the current state \f$x_k\f$ in step \f$k\f$ and
     * the system control input \f$u\f$.
     *
     * @param [in] x The system state in current time-step
     * @param [in] u The control vector input
     * @returns The (predicted) system state in the next time-step
     */
    S f(const S& x, const C& u) const
    {
        static std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now(); 
        static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count()) ;
        //static std::uniform_real_distribution<float> u_noise(-1,1);
        static std::normal_distribution<float> g_noise(0,1);
        static std::normal_distribution<float> g_angle_noise(0, 0.4);

        //! Predicted state vector after transition
        S x_;
        
        // New orientation given by old orientation plus orientation change
        //auto newOrientation = x.theta() + u.dtheta();
        // Re-scale orientation to [-pi/2 to +pi/2]
        //x_.theta() = newOrientation;
       
        float ax = g_noise(generator);
        float ay = g_noise(generator);
        float atheta = g_angle_noise(generator);
        
        std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now(); //end of interval
        float t =std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;
        //ax = ay = atheta = 0.0;
        // x_.dx() = x_.dx() + ax * t;
        // x_.dy() = x_.dy() + ay * t;
        // x_.dtheta() = x_.dtheta() + atheta * t;
        x_.x() = x_.x() + ax*t;
        x_.y() = x_.y() + ay*t;
        x_.theta() = x_.theta() + atheta * t;
        
        // Return transitioned state vector
        start = std::chrono::high_resolution_clock::now(); 
        return x_;
    }
    
protected:
    /**
     * @brief Update jacobian matrices for the system state transition function using current state
     *
     * This will re-compute the (state-dependent) elements of the jacobian matrices
     * to linearize the non-linear state transition function \f$f(x,u)\f$ around the
     * current state \f$x\f$.
     *
     * @note This is only needed when implementing a LinearizedSystemModel,
     *       for usage with an ExtendedKalmanFilter or SquareRootExtendedKalmanFilter.
     *       When using a fully non-linear filter such as the UnscentedKalmanFilter
     *       or its square-root form then this is not needed.
     *
     * @param x The current system state around which to linearize
     * @param u The current system control input
     */
    void updateJacobians( const S& x, const C& u )
    {
        // F = df/dx (Jacobian of state transition w.r.t. the state)
        this->F.setIdentity();
        // W = df/dw (Jacobian of state transition w.r.t. the noise)
        this->W.setIdentity();
        // TODO: more sophisticated noise modelling
        //       i.e. The noise affects the the direction in which we move as 
        //       well as the velocity (i.e. the distance we move)
    }
private:
   
};

} // namespace Robot
} // namespace KalmanExamples

#endif