#pragma once

template <typename HostMatrix>
struct nnz_type : nitro::input_feature_type<HostMatrix*> {

  __forceinline__
  double operator()(HostMatrix *host_matrix)
  {
    return (double)(*host_matrix).num_entries;
  }
};

template <typename HostMatrix>
struct num_rows_type : nitro::input_feature_type<HostMatrix*> {

  __forceinline__
  double operator()(HostMatrix *host_matrix)
  {
    return (double)(*host_matrix).num_rows;
  }
};

template <typename HostMatrix>
struct dia_fillin_type : nitro::input_feature_type<HostMatrix*> {

  __forceinline__
  double operator()(HostMatrix *host_matrix)
  {
    const size_t occupied_diagonals = cusp::detail::host::count_diagonals(*host_matrix);
    const double size = double(occupied_diagonals) * double(host_matrix->num_rows);
    const double fill_ratio = size / std::max(1.0, double(host_matrix->num_entries));

    return (fill_ratio < 10.0? fill_ratio: 10.0);
  }
};

template <typename HostMatrix>
struct ell_fillin_type : nitro::input_feature_type<HostMatrix*> {

  __forceinline__
  double operator()(HostMatrix *host_matrix)
  {
      const size_t max_entries_per_row = cusp::detail::host::compute_max_entries_per_row(*host_matrix);
      const float size       = float(max_entries_per_row) * float(host_matrix->num_rows);
      const float fill_ratio = size / std::max(1.0, double(host_matrix->num_entries));

      return (fill_ratio < 10.0? fill_ratio: 10.0);
  }
};


template <typename HostMatrix>
struct avg_nz_per_row_type : nitro::input_feature_type<HostMatrix*> {

  __forceinline__
  double operator()(HostMatrix *host_matrix)
  {
    return (double)((double)host_matrix->num_entries / (double)host_matrix->num_rows);
  }
};

template <typename HostMatrix>
struct max_deviation_type : nitro::input_feature_type<HostMatrix*> {

  double operator()(HostMatrix *host_matrix)
  {
    double mean = (double)((double)host_matrix->num_entries / (double)host_matrix->num_rows);
    int max = 0;

    for(size_t i = 0; i < host_matrix->num_rows; ++i) {
      int rowLength = (host_matrix->row_offsets[i+1] - host_matrix->row_offsets[i]);
      if(rowLength > max) max = rowLength;
    }

    return (double)(((double)max - (double)mean) / (double)(max));
  }
};

template <typename HostMatrix>
struct exceptional_rows_type : nitro::input_feature_type<HostMatrix*> {

  double operator()(HostMatrix *host_matrix)
  {
    double mean = (double)((double)host_matrix->num_entries / (double)host_matrix->num_rows);
    int max = 0;

    for(size_t i = 0; i < host_matrix->num_rows; ++i) {
      int rowLength = (host_matrix->row_offsets[i+1] - host_matrix->row_offsets[i]);
      if(rowLength > max) max = rowLength;
    }

    double maxDeviation = (double)mean + (double)(((double)max - (double)mean) / 2.0);
    int exRows = 0;
    for(size_t i = 0; i < host_matrix->num_rows; ++i) {
      if((host_matrix->row_offsets[i+1] - host_matrix->row_offsets[i]) > (int)maxDeviation) {
        ++exRows;
      }
    }

    return (double)((double)exRows / (double)host_matrix->num_rows);
  }
};

template <typename HostMatrix>
struct row_length_sd_type : nitro::input_feature_type<HostMatrix*> {

  double operator()(HostMatrix *host_matrix)
  {
    double mean = (double)((double)host_matrix->num_entries / (double)host_matrix->num_rows);

    double sd = 0.0f;
    for(size_t i = 0; i < host_matrix->num_rows; ++i) {
      int rowLength = (host_matrix->row_offsets[i+1] - host_matrix->row_offsets[i]);
      double elt = (double)rowLength - mean;
      sd += (elt * elt);
    }

    sd /= (double)host_matrix->num_rows;

    return (double)sqrt(sd);
  }
};

