#pragma once

#include "benchmark.h"

template <typename HostMatrix>
struct coo_type : nitro::variant_type<HostMatrix *> {
  std::string operator()(HostMatrix* host_matrix)
  {
      typedef typename HostMatrix::index_type IndexType;
      typedef typename HostMatrix::value_type ValueType;

      // convert HostMatrix to TestMatrix on host
      cusp::coo_matrix<IndexType, ValueType, cusp::host_memory> test_matrix_on_host(*host_matrix);

      // transfer TestMatrix to device
      typedef typename cusp::coo_matrix<IndexType, ValueType, cusp::device_memory> DeviceMatrix;
      DeviceMatrix test_matrix_on_device(test_matrix_on_host);

      return test_spmv("coo_flat",     *host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_coo_flat    <DeviceMatrix,ValueType>);
      //test_spmv("coo_flat_tex", host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_coo_flat_tex<DeviceMatrix,ValueType>);

  //    test_spmv("coo_flat_k",     host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_coo_flat_k    <DeviceMatrix,ValueType>);
  //    test_spmv("coo_flat_k_tex", host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_coo_flat_k_tex<DeviceMatrix,ValueType>);
  }
};

template <typename HostMatrix>
struct csr_type : nitro::variant_type<HostMatrix *> {
  std::string operator()(HostMatrix* host_matrix)
  {
      typedef typename HostMatrix::index_type IndexType;
      typedef typename HostMatrix::value_type ValueType;

      // convert HostMatrix to TestMatrix on host
      cusp::csr_matrix<IndexType, ValueType, cusp::host_memory> test_matrix_on_host(*host_matrix);

      // transfer csr_matrix to device
      typedef typename cusp::csr_matrix<IndexType, ValueType, cusp::device_memory> DeviceMatrix;
      DeviceMatrix test_matrix_on_device(test_matrix_on_host);

      return test_spmv("csr_scalar",     *host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_scalar    <DeviceMatrix,ValueType>);
      //test_spmv("csr_scalar_tex", host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_scalar_tex<DeviceMatrix,ValueType>);
      //test_spmv("csr_vector",     host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_vector    <DeviceMatrix,ValueType>);
      //test_spmv("csr_vector_tex", host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_vector_tex<DeviceMatrix,ValueType>);
  }
};

template <typename HostMatrix>
struct csr_tex_type : nitro::variant_type<HostMatrix *> {
  std::string operator()(HostMatrix* host_matrix)
  {
      typedef typename HostMatrix::index_type IndexType;
      typedef typename HostMatrix::value_type ValueType;

      // convert HostMatrix to TestMatrix on host
      cusp::csr_matrix<IndexType, ValueType, cusp::host_memory> test_matrix_on_host(*host_matrix);

      // transfer csr_matrix to device
      typedef typename cusp::csr_matrix<IndexType, ValueType, cusp::device_memory> DeviceMatrix;
      DeviceMatrix test_matrix_on_device(test_matrix_on_host);

      return test_spmv("csr_scalar_tex",     *host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_scalar_tex    <DeviceMatrix,ValueType>);
      //test_spmv("csr_scalar_tex", host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_scalar_tex<DeviceMatrix,ValueType>);
      //test_spmv("csr_vector",     host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_vector    <DeviceMatrix,ValueType>);
      //test_spmv("csr_vector_tex", host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_vector_tex<DeviceMatrix,ValueType>);
  }
};

template <typename HostMatrix>
struct csr_vector_type : nitro::variant_type<HostMatrix *> {
  std::string operator()(HostMatrix *host_matrix)
  {
      typedef typename HostMatrix::index_type IndexType;
      typedef typename HostMatrix::value_type ValueType;

      // convert HostMatrix to TestMatrix on host
      cusp::csr_matrix<IndexType, ValueType, cusp::host_memory> test_matrix_on_host(*host_matrix);

      // transfer csr_matrix to device
      typedef typename cusp::csr_matrix<IndexType, ValueType, cusp::device_memory> DeviceMatrix;
      DeviceMatrix test_matrix_on_device(test_matrix_on_host);

      //test_spmv("csr_scalar",     host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_scalar    <DeviceMatrix,ValueType>);
      //test_spmv("csr_scalar_tex", host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_scalar_tex<DeviceMatrix,ValueType>);

      return test_spmv("csr_vector",     *host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_vector    <DeviceMatrix,ValueType>);
      //test_spmv("csr_vector_tex", host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_vector_tex<DeviceMatrix,ValueType>);
  }
};

template <typename HostMatrix>
struct csr_vector_tex_type : nitro::variant_type<HostMatrix *> {
  std::string operator()(HostMatrix *host_matrix)
  {
      typedef typename HostMatrix::index_type IndexType;
      typedef typename HostMatrix::value_type ValueType;

      // convert HostMatrix to TestMatrix on host
      cusp::csr_matrix<IndexType, ValueType, cusp::host_memory> test_matrix_on_host(*host_matrix);

      // transfer csr_matrix to device
      typedef typename cusp::csr_matrix<IndexType, ValueType, cusp::device_memory> DeviceMatrix;
      DeviceMatrix test_matrix_on_device(test_matrix_on_host);

      //test_spmv("csr_scalar",     host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_scalar    <DeviceMatrix,ValueType>);
      //test_spmv("csr_scalar_tex", host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_scalar_tex<DeviceMatrix,ValueType>);
      //return test_spmv("csr_vector",     host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_vector    <DeviceMatrix,ValueType>);
      return test_spmv("csr_vector_tex", *host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_csr_vector_tex<DeviceMatrix,ValueType>);
  }
};

template <typename HostMatrix>
struct dia_type : nitro::variant_type<HostMatrix *> {
  std::string operator()(HostMatrix *host_matrix)
  {
      typedef typename HostMatrix::index_type IndexType;
      typedef typename HostMatrix::value_type ValueType;

      // convert HostMatrix to TestMatrix on host
      cusp::dia_matrix<IndexType, ValueType, cusp::host_memory> test_matrix_on_host;

      try
      {
          test_matrix_on_host = *host_matrix;
      }
      catch (cusp::format_conversion_exception)
      {
          std::cout << "\tRefusing to convert to DIA format" << std::endl;
          std::stringstream ss;
          ss << std::numeric_limits<double>::max();
          return ss.str();
      }

      // transfer TestMatrix to device
      typedef typename cusp::dia_matrix<IndexType, ValueType, cusp::device_memory> DeviceMatrix;
      DeviceMatrix test_matrix_on_device(test_matrix_on_host);

      return test_spmv("dia",     *host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_dia    <DeviceMatrix,ValueType>);
      //test_spmv("dia_tex", host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_dia_tex<DeviceMatrix,ValueType>);
  }
};

template <typename HostMatrix>
struct dia_tex_type : nitro::variant_type<HostMatrix *> {
  std::string operator()(HostMatrix *host_matrix)
  {
      typedef typename HostMatrix::index_type IndexType;
      typedef typename HostMatrix::value_type ValueType;

      // convert HostMatrix to TestMatrix on host
      cusp::dia_matrix<IndexType, ValueType, cusp::host_memory> test_matrix_on_host;

      try
      {
          test_matrix_on_host = *host_matrix;
      }
      catch (cusp::format_conversion_exception)
      {
          std::cout << "\tRefusing to convert to DIA format" << std::endl;
          std::stringstream ss;
          ss << std::numeric_limits<double>::max();
          return ss.str();
      }

      // transfer TestMatrix to device
      typedef typename cusp::dia_matrix<IndexType, ValueType, cusp::device_memory> DeviceMatrix;
      DeviceMatrix test_matrix_on_device(test_matrix_on_host);

      //return test_spmv("dia",     host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_dia    <DeviceMatrix,ValueType>);
      return test_spmv("dia_tex", *host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_dia_tex<DeviceMatrix,ValueType>);
  }
};

template <typename HostMatrix>
struct ell_type : nitro::variant_type<HostMatrix *> {
  std::string operator()(HostMatrix *host_matrix)
  {
      typedef typename HostMatrix::index_type IndexType;
      typedef typename HostMatrix::value_type ValueType;

      // convert HostMatrix to TestMatrix on host
      cusp::ell_matrix<IndexType, ValueType, cusp::host_memory> test_matrix_on_host;

      try
      {
          test_matrix_on_host = *host_matrix;
      }
      catch (cusp::format_conversion_exception)
      {
          std::cout << "\tRefusing to convert to ELL format" << std::endl;
          std::stringstream ss;
          ss << std::numeric_limits<double>::max();
          return ss.str();
      }

      // transfer TestMatrix to device
      typedef typename cusp::ell_matrix<IndexType, ValueType, cusp::device_memory> DeviceMatrix;
      DeviceMatrix test_matrix_on_device(test_matrix_on_host);

      return test_spmv("ell",     *host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_ell    <DeviceMatrix,ValueType>);
      //test_spmv("ell_tex", host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_ell_tex<DeviceMatrix,ValueType>);
  }
};

template <typename HostMatrix>
struct ell_tex_type : nitro::variant_type<HostMatrix *> {
  std::string operator()(HostMatrix *host_matrix)
  {
      typedef typename HostMatrix::index_type IndexType;
      typedef typename HostMatrix::value_type ValueType;

      // convert HostMatrix to TestMatrix on host
      cusp::ell_matrix<IndexType, ValueType, cusp::host_memory> test_matrix_on_host;

      try
      {
          test_matrix_on_host = *host_matrix;
      }
      catch (cusp::format_conversion_exception)
      {
          std::cout << "\tRefusing to convert to ELL format" << std::endl;
          std::stringstream ss;
          ss << std::numeric_limits<double>::max();
          return ss.str();
      }

      // transfer TestMatrix to device
      typedef typename cusp::ell_matrix<IndexType, ValueType, cusp::device_memory> DeviceMatrix;
      DeviceMatrix test_matrix_on_device(test_matrix_on_host);

      //return test_spmv("ell",     host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_ell    <DeviceMatrix,ValueType>);
      return test_spmv("ell_tex", *host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_ell_tex<DeviceMatrix,ValueType>);
  }
};

template <typename HostMatrix>
struct hyb_type : nitro::variant_type<HostMatrix *> {
  std::string operator()(HostMatrix *host_matrix)
  {
      typedef typename HostMatrix::index_type IndexType;
      typedef typename HostMatrix::value_type ValueType;

      // convert HostMatrix to TestMatrix on host
      cusp::hyb_matrix<IndexType, ValueType, cusp::host_memory> test_matrix_on_host(*host_matrix);

      // transfer TestMatrix to device
      typedef typename cusp::hyb_matrix<IndexType, ValueType, cusp::device_memory> DeviceMatrix;
      DeviceMatrix test_matrix_on_device(test_matrix_on_host);

      return test_spmv("hyb",     *host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_hyb    <DeviceMatrix,ValueType>);
      //test_spmv("hyb_tex", host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_hyb_tex<DeviceMatrix,ValueType>);
  }
};

template <typename HostMatrix>
struct hyb_tex_type : nitro::variant_type<HostMatrix *> {
  std::string operator()(HostMatrix *host_matrix)
  {
      typedef typename HostMatrix::index_type IndexType;
      typedef typename HostMatrix::value_type ValueType;

      // convert HostMatrix to TestMatrix on host
      cusp::hyb_matrix<IndexType, ValueType, cusp::host_memory> test_matrix_on_host(*host_matrix);

      // transfer TestMatrix to device
      typedef typename cusp::hyb_matrix<IndexType, ValueType, cusp::device_memory> DeviceMatrix;
      DeviceMatrix test_matrix_on_device(test_matrix_on_host);

      //return test_spmv("hyb",     host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_hyb    <DeviceMatrix,ValueType>);
      return test_spmv("hyb_tex", *host_matrix, test_matrix_on_host, test_matrix_on_device, cusp::detail::device::spmv_hyb_tex<DeviceMatrix,ValueType>);
  }
};

