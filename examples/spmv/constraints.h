#pragma once

struct dia_cutoff_type : nitro::constraint_type {
  bool operator()(const std::vector<double>& fv)
  {
    if(fv[3] > 9.0)
      return false;
    return true;
  }
};

struct ell_cutoff_type : nitro::constraint_type {
  bool operator()(const std::vector<double>& fv)
  {
    if(fv[4] > 9.0)
      return false;
    return true;
  }
};
