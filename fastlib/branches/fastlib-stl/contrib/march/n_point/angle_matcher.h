/*
 *  angle_matcher.h
 *  
 *  Computes three point correlations for a matcher specified by a range of 
 *  values of r1, a factor c such that r2 = c*r1, and a range of angles between 
 *  the sides theta.
 *
 *  Created by William March on 7/26/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANGLE_MATCHER_H
#define ANGLE_MATCHER_H

#include "fastlib/fastlib.h"


// Assumptions (for now):
//
// bins might overlap (especially at large values of theta)
// Values of r1 are spaced far enough apart such that a tuple of points will 
// only satisfy one

namespace npt {

  class AngleMatcher {
    
  private:
    
    std::vector<double> short_sides_;
    // the long side is this times the short side
    // should I include more than one of these?
    double long_side_multiplier_;
    
    std::vector<double> long_sides_;

    // these are in radians
    std::vector<double> thetas_;
    
    // indexed by [value of r1][value of theta]
    std::vector<std::vector<double> > r3_sides_;
    
    // this is the value of theta where r2 = r3
    // computed by arccos(1/2k) where k is long_side_multiplier_
    double theta_cutoff_;
    // this is the index in the theta array where it occurs
    // i.e. thetas_[theta_cutoff_index_] is the first theta where r3 > r2
    int theta_cutoff_index_;
    
    
    // upper and lower bound arrays
    std::vector<double> r1_lower_sqr_;
    std::vector<double> r1_upper_sqr_;
    
    std::vector<double> r2_lower_sqr_;
    std::vector<double> r2_upper_sqr_;
    
    std::vector<std::vector<double> > r3_lower_sqr_;
    std::vector<std::vector<double> > r3_upper_sqr_;
    
    
    
    
    // This is 0.25 in the thesis (or maybe 0.1?)
    // Note that the bin thickness is this times the scale
    // Does this mean in each dimension?
    double bin_thickness_factor_;
    
    
    ////////////////////////////
    
    
    
  public:
    
    AngleMatcher(std::vector<double>& short_sides, double long_side,
                 std::vector<double>& thetas, double bin_size) :
    short_sides_(short_sides), long_side_multiplier_(long_size), 
    thetas_(thetas), bin_thickness_factor_(bin_size),
    r3_sides_(thetas.size()), long_sides_(short_sides.size()),
    r1_lower_sqr_(short_sides.size()), r1_upper_sqr_(short_sides.size()), 
    r2_lower_sqr_(short_sides.size()), r2_upper_sqr_(short_sides.size()),
    r3_lower_sqr_(short_sides.size()), r3_upper_sqr_(short_sides.size()) {
      
      theta_cutoff_ = math::acos(1.0/(2.0 * long_side_multiplier_));
      theta_cutoff_index_ = std::lower_bound(thetas_.begin(), thetas_.end(), 
                                             theta_cutoff) - thetas_.begin();
      
      for (int i = 0; i < short_sides_.size(); i++) {
          
        r3_sides_[i].resize(thetas_.size());
        long_sides_[i] = long_side_multiplier_ * short_sides_[i];
        
        r1_lower_sqr_[i] = ((1.0 - bin_thickness_factor_) * short_sides_[i])
                            * ((1.0 - bin_thickness_factor_) * short_sides_[i]);
        r1_upper_sqr_[i] = ((1.0 + bin_thickness_factor_) * short_sides_[i])
                            * ((1.0 + bin_thickness_factor_) * short_sides_[i]);
        
        r2_lower_sqr_[i] = ((1.0 - bin_thickness_factor_) * long_sides_[i])
        * ((1.0 - bin_thickness_factor_) * long_sides_[i]);
        r2_upper_sqr_[i] = ((1.0 + bin_thickness_factor_) * long_sides_[i])
        * ((1.0 + bin_thickness_factor_) * long_sides_[i]);
        
        for (int j = 0; i < thetas_.size(); j++) {
          
          r3_sides_[i][j] = ComputeR3_(short_sides_[i], 
                                     long_sides_[i], 
                                     thetas_[j]);
          
          r3_lower_sqr_[i][j] = ((1.0 - bin_thickness_factor_) * r3_sides_[i][j])
                              * ((1.0 - bin_thickness_factor_) * r3_sides_[i][j]);
          r3_upper_sqr_[i][j] = ((1.0 + bin_thickness_factor_) * r3_sides_[i][j])
                              * ((1.0 + bin_thickness_factor_) * r3_sides_[i][j]);
          
        } // for j
        
      } // for i
      
      // IMPORTANT: I'm not sure the upper and lower sqr arrays are still sorted
      // especially for r3
      
    } // constructor
    
    // returns the index of the value of r1 that is satisfied by the tuple
    // the list contains the indices of thetas_ that are satisfied by the tuple
    // assumes that valid_theta_indices is initialized and empty
    int TestPointTuple(arma::colvec& vec1, arma::colvec& vec2, 
                       arma::colvec& vec3,
                       std::vector<int>& valid_theta_indices);
    
    // returns true if the tuple of nodes might contain a tuple of points that
    // satisfy one of the matchers
    // If false, then pruning is ok
    bool TestNodeTuple(const DHrectBound<2>& box1, const DHrectBound<2>& box2,
                       const DHrectBound<2>& box3);
    
    
    
  }; // AngleMatcher

}


#endif