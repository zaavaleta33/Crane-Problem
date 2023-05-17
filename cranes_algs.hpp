///////////////////////////////////////////////////////////////////////////////
// cranes_algs.hpp
//
// Algorithms that solve the crane unloading problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on crane_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <math.h>

#include "cranes_types.hpp"

namespace cranes {

// Solve the crane unloading problem for the given grid, using an exhaustive
// optimization algorithm.
//
// This algorithm is expected to run in exponential time, so the grid's
// width+height must be small enough to fit in a 64-bit int; this is enforced
// with an assertion.
//
// The grid must be non-empty.
path crane_unloading_exhaustive(const grid& setting) { 

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  // Compute maximum path length, and check that it is legal.
  const size_t max_steps = setting.rows() + setting.columns() - 2;
  assert(max_steps < 64);

  // TODO: implement the exhaustive search algorithm, then delete this
  // comment.
   
/* here is the psudocode for exhastive optimization from notes
for candidate(ss)
  if verify(compare())
      compare(candidate)
          best = candidate
return candidate
*/
/*
Basic understanding of exhastive optimization: it finds all the possible solutions and then picks the best one. It is rather slow, it will find the 
best solution
*/
path best(setting);
for (size_t steps = 0; steps <= max_steps; steps++) {
    for (auto/*size_t*/ i = 0; i < (1 << steps); i++) {  
      path candidate(setting);
      for (size_t j = 0; j < steps; j++) {
        if ( (1 & (i >> j)) == 1) {
          //move east
          if (candidate.is_step_valid(STEP_DIRECTION_EAST)) {
            candidate.add_step(STEP_DIRECTION_EAST);
          } else {
            //this path is invalid, go to the next one
            break;
          } 
          } else {
            //move south
            if(candidate.is_step_valid(STEP_DIRECTION_SOUTH)){
              candidate.add_step(STEP_DIRECTION_SOUTH);
            } else {
              //this path is invalid, go to the next one
              break;
            }
          }
          
        } //closes j for loop
      
        //if the candidate path reaches the bottom right corner compare it to the best path found so far
         if (candidate.total_cranes() > best.total_cranes()){
              best = candidate;
         }
      } //close the i for loop
      
    } //closes the steps for loop
  
  return best;
}
// Solve the crane unloading problem for the given grid, using a dynamic
// programming algorithm.
//
// The grid must be non-empty.
//path crane_unloading_dyn_prog(const grid& setting) {
path crane_unloading_dyn_prog(const grid& setting) {
  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  path best(setting);
  using cell_type = std::optional<path>;

  std::vector<std::vector<cell_type> > A(setting.rows(),
                                        std::vector<cell_type>(setting.columns()));

  A[0][0] = path(setting);
  assert(A[0][0].has_value());


  
  for (coordinate r = 0; r < setting.rows(); ++r) {
    for (coordinate c = 0; c < setting.columns(); ++c) {

       if (r == 0 && c == 0) {
        continue;
      }
      
      if (setting.get(r, c) == CELL_BUILDING) {
        A[r][c].reset();
        continue;
      }

      
      cell_type from_above = std::nullopt;
      cell_type from_left = std::nullopt;
    

	    // TODO: implement the dynamic programming algorithm, then delete this
      // comment.
      
      
      if (c > 0 && setting.get(r,c - 1) != CELL_BUILDING) {
        from_left = A[r][c - 1];
        if(from_left.has_value()){
          from_left->add_step(STEP_DIRECTION_EAST);
        }
      } 
      if (r > 0 && setting.get(r - 1,c) != CELL_BUILDING ) {
        from_above = A[r - 1][c];
        if(from_above.has_value()){
          from_above->add_step(STEP_DIRECTION_SOUTH);
        }
      }

      
      
       // if(!from_above.has_value() && !from_left.has_value()) {
       //   A[r][c].reset();
       //   continue;
       // }

      if (from_above.has_value() && !from_left.has_value()) {
        A[r][c] = from_above;
      } else if (!from_above.has_value() && from_left.has_value()){
        A[r][c] = from_left;
      } else if (from_above.has_value() && from_left.has_value() && from_above->total_cranes() > from_left->total_cranes()){
           A[r][c] = from_above;
      } else {
        A[r][c] = from_left;
      } 
        // assert(best->has_value());
        //   std::cout << "total cranes" << (**best).total_cranes() << std::endl;
    }
 }

  for(coordinate r = 0; r < setting.rows(); ++r) {
    for (coordinate c =  0; c < setting.columns();  ++c) {
      if (A[r][c].has_value() && A[r][c].value().total_cranes() > best.total_cranes()) {
        best = A[r][c].value();
      }
    }
  }
   return best;
}
}
