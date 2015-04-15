all: spmv

LIBPATH = -L $(LIBSVM_PATH)
INCPATH = ../../include
INCS = -I $(LIBSVM_PATH)/ -I $(CUSP_PATH)
CXX = nvcc
LIBS = -lsvm -lcuda

GENCODE_SM = -gencode=arch=compute_$(SHADER_MODEL),code=\"sm_$(SHADER_MODEL),compute_$(SHADER_MODEL)\"
CXXARGS = -m64 -O2 -std=c++11
CXXARGS += $(GENCODE_SM)
CXXARGS += -Xcompiler -DNDEBUG

SM_MESSAGE = ''

ifeq ($(strip $(SHADER_MODEL)),)
	SHADER_MODEL = 20
	SM_MESSAGE = '\e[1m[Note: SHADER_MODEL not set]\n\n \e[0m'
endif

spmv: spmv.cu
	@echo -e -n $(SM_MESSAGE)
	$(CXX) spmv.cu $(CXXARGS) $(LIBS) $(INCS) $(LIBPATH) -I$(INCPATH) -o spmv

clean:
	rm -f spmv *.o
	rm -f *.cp *.perf *.tsperf *.ts *.t *.t.scale *.acc *_tuned *_codepath
	rm -f *.log
