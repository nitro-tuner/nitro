#define CUSP_USE_TEXTURE_MEMORY

#include <cusp/csr_matrix.h>
#include <cusp/io/matrix_market.h>
#include <cusp/gallery/poisson.h>

#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <limits>

#include <cusp/multiply.h>
#include <cusp/detail/device/spmv/csr_scalar.h>

#include "bytes_per_spmv.h"
#include "utility.h"
#include "benchmark.h"

#include "nitro_config.h"
#include <nitro.h>

#include "variants.h"
#include "features.h"
#include "constraints.h"

typedef std::map<std::string, std::string> ArgumentMap;
ArgumentMap args;

std::string process_args(int argc, char ** argv)
{
    std::string filename;

    for(int i = 1; i < argc; i++)
    {
        std::string arg(argv[i]);

        if (arg.substr(0,2) == "--")
        {   
            std::string::size_type n = arg.find('=',2);

            if (n == std::string::npos)
                args[arg.substr(2)] = std::string();              // (key)
            else
                args[arg.substr(2, n - 2)] = arg.substr(n + 1);   // (key,value)
        }
        else
        {
            filename = arg;
        }
    }

    return filename;
}

void usage(int argc, char** argv)
{
    std::cout << "Usage:\n";
    std::cout << "\t" << argv[0] << "\n";
    std::cout << "\t" << argv[0] << " my_matrix.mtx\n";
    std::cout << "\t" << argv[0] << " my_matrix.mtx --device=1\n";
    std::cout << "\t" << argv[0] << " my_matrix.mtx --value_type=double\n\n";
    std::cout << "Note: my_matrix.mtx must be real-valued sparse matrix in the MatrixMarket file format.\n"; 
    std::cout << "      If no matrix file is provided then a simple example is created.\n";
}

template <typename IndexType, typename ValueType>
void test_all_formats(std::string& filename)
{
    int device_id  = args.count("device") ? atoi(args["device"].c_str()) :  0;
    set_device(device_id);
    list_devices();

    std::cout << "Running on Device " << device_id << "\n\n";
    
    // load a matrix stored in MatrixMarket format
    typedef cusp::csr_matrix<IndexType, ValueType, cusp::host_memory> HostMatrix;
    HostMatrix host_matrix;

    if (filename == "")
    {
        cusp::gallery::poisson5pt(host_matrix, 512, 512);
        std::cout << "Generated matrix (poisson5pt) ";
    }
    else
    {
        cusp::io::read_matrix_market_file(host_matrix, filename);
        std::cout << "Read matrix (" << filename << ") ";
    }
        
    std::cout << "with shape ("  << host_matrix.num_rows << "," << host_matrix.num_cols << ") and "
              << host_matrix.num_entries << " entries" << "\n\n";
    
    FILE * fid = fopen(BENCHMARK_OUTPUT_FILE_NAME, "a");
    fprintf(fid, "file=%s rows=%d cols=%d nonzeros=%d\n", filename.c_str(), (int) host_matrix.num_rows, (int) host_matrix.num_cols, (int) host_matrix.num_entries);
    fclose(fid);
    
    using namespace nitro;
    using namespace thrust;

    typedef tuple<HostMatrix *> ArgTuple;

    context cx;
    code_variant<tuning_policies::spmv, ArgTuple, sm> spmv(cx);

    coo_type<HostMatrix> coo;
    csr_type<HostMatrix> csr;
    csr_tex_type<HostMatrix> csr_tex;
    csr_vector_type<HostMatrix> csr_vector;
    csr_vector_tex_type<HostMatrix> csr_vector_tex;
    dia_type<HostMatrix> dia;
    dia_tex_type<HostMatrix> dia_tex;
    ell_type<HostMatrix> ell;
    ell_tex_type<HostMatrix> ell_tex;

    //spmv.add_variant(&coo);
    spmv.add_variant(&csr);
    //spmv.add_variant(&csr_tex);
    spmv.add_variant(&csr_vector);
    //spmv.add_variant(&csr_vector_tex);
    spmv.add_variant(&dia);
    //spmv.add_variant(&dia_tex);
    spmv.add_variant(&ell);
    //spmv.add_variant(&ell_tex);

    spmv.set_default(&csr_vector);

    avg_nz_per_row_type<HostMatrix> avg_nz_per_row;
    row_length_sd_type<HostMatrix> row_length_sd;
    nnz_type<HostMatrix> nnz;
    num_rows_type<HostMatrix> num_rows;
    max_deviation_type<HostMatrix> max_deviation;
    exceptional_rows_type<HostMatrix> exceptional_rows;
    dia_fillin_type<HostMatrix> dia_fillin;
    ell_fillin_type<HostMatrix> ell_fillin;

    spmv.add_input_feature(&avg_nz_per_row);
    spmv.add_input_feature(&row_length_sd);
    //spmv.add_input_feature(&nnz);
    //spmv.add_input_feature(&num_rows);
    spmv.add_input_feature(&max_deviation);
    //spmv.add_input_feature(&exceptional_rows);
    spmv.add_input_feature(&dia_fillin);
    spmv.add_input_feature(&ell_fillin);

    dia_cutoff_type dia_cutoff;
    ell_cutoff_type ell_cutoff;

    spmv.add_constraint(&dia, &dia_cutoff);
    spmv.add_constraint(&dia_tex, &dia_cutoff);
    spmv.add_constraint(&ell, &ell_cutoff);
    spmv.add_constraint(&ell_tex, &ell_cutoff);

    spmv(&host_matrix);
}

int main(int argc, char** argv)
{
    std::string filename = process_args(argc, argv);

    if (args.count("help"))
    {
        usage(argc, argv);
        return 0;
    }

    // select ValueType
    std::string value_type = args.count("value_type") ? args["value_type"] : "double";
    std::cout << "\nComputing SpMV with \'" << value_type << "\' values.\n\n";

    if(value_type == "float") {
      test_all_formats<int,float>(filename);
    } else if (value_type == "double")
    {
        int current_device = -1;
        cudaDeviceProp properties;
        cudaGetDevice(&current_device);
        cudaGetDeviceProperties(&properties, current_device);
        if (properties.major == 1 && properties.minor < 3)
            std::cerr << "ERROR: Support for \'double\' requires Compute Capability 1.3 or greater\n\n";
        else
            test_all_formats<int,double>(filename);
    }
    else
    {
        std::cerr << "ERROR: Unsupported type \'" << value_type << "\'\n\n";
    }

    return 0;
}

