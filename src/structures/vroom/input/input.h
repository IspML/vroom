#ifndef INPUT_H
#define INPUT_H

/*

This file is part of VROOM.

Copyright (c) 2015-2018, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include <array>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/optional.hpp>

#include "../../../problems/cvrp/cvrp.h"
#include "../../../problems/tsp/tsp.h"
#include "../../../routing/routed_wrapper.h"
#include "../../../routing/routing_io.h"
#include "../../../utils/exceptions.h"
#include "../../../utils/helpers.h"
#include "../../abstract/matrix.h"
#include "../../typedefs.h"
#include "../job.h"
#include "../solution/solution.h"
#include "../vehicle.h"
#if LIBOSRM
#include "../../../routing/libosrm_wrapper.h"
#endif

class vrp;
class cvrp_local_search; // To remove

struct type_with_id {
  TYPE type;
  ID_t id;
};

class input {
private:
  std::chrono::high_resolution_clock::time_point _start_loading;
  std::chrono::high_resolution_clock::time_point _end_loading;
  std::chrono::high_resolution_clock::time_point _end_solving;
  std::chrono::high_resolution_clock::time_point _end_routing;
  std::unique_ptr<routing_io<cost_t>> _routing_wrapper;
  bool _has_skills;
  const bool _geometry;
  matrix<cost_t> _matrix;
  std::vector<location_t> _locations;
  unsigned _amount_size;
  amount_t _amount_lower_bound;
  std::vector<std::vector<bool>> _vehicle_to_job_compatibility;
  void check_amount_size(unsigned size);
  std::unique_ptr<vrp> get_problem() const;
  void check_cost_bound() const;
  void set_vehicle_to_job_compatibility();
  std::unordered_set<index_t> _matrix_used_index;
  bool _all_locations_have_coords;

  solution format_solution(const raw_solution& raw_routes) const;
  void store_amount_lower_bound(const amount_t& amount);

public:
  std::vector<job_t> _jobs;
  std::vector<vehicle_t> _vehicles;

  input(std::unique_ptr<routing_io<cost_t>> routing_wrapper, bool geometry);

  void add_job(const job_t& job);

  void add_vehicle(const vehicle_t& vehicle);

  void set_matrix(matrix<cost_t>&& m);

  unsigned amount_size() const;

  amount_t get_amount_lower_bound() const;

  bool vehicle_ok_with_job(index_t v_index, index_t j_index) const;

  const matrix<cost_t>& get_matrix() const;

  matrix<cost_t> get_sub_matrix(const std::vector<index_t>& indices) const;

  solution solve(unsigned nb_thread);

  friend cvrp_local_search; // To remove
};

#endif
