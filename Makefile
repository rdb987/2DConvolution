####### COMPILER SETTINGS: DO NOT CHANGE!!! ####### 

CXX = g++
MPICXX = mpic++ 
CXXFLAGS = -O2 -march=native -Wall -Wextra -std=c++11 -I$(INCLUDE_DIR) -fopenmp

#####################################################


############### HEADER FILE SETTINGS ###############

# PUT HERE THE FOLDER WITH YOUR HEADER FILES, IF ANY
# FOR EXAMPLE:
# INCLUDE_DIR = include
INCLUDE_DIR = include

# PUT HERE THE PATH TO YOUR HEADER FILES TO CORRECTLY RECOMPILE AT ANY CHANGE, IF ANY
# FOR EXAMPLE:
# DEPS = $(INCLUDE_DIR)/*.h*
DEPS = $(INCLUDE_DIR)/*.h*

#####################################################

############## SOURCE FILE SETTINGS ###############

# PUT HERE THE FOLDER WITH YOUR SOURCe FILES
# FOR EXAMPLE:
# SRC = src
SRC = src

# FOR EVERY VERSION OF THE PROGRAM, PUT HERE THE LIST OF SOURCE FILES TO BE COMPILED
# FOR EXAMPLE
# SERIAL_SRC = $(SRC)/hpc_conv_st.cpp $(SRC)/main_st.cpp

SERIAL_SRC = $(SRC)/hpc_conv_st.cpp $(SRC)/main_st.cpp

OMP_SRC = $(SRC)/hpc_conv_omp.cpp $(SRC)/main_omp.cpp

MPI_SRC = $(SRC)/hpc_conv_mpi.cpp $(SRC)/main_mpi.cpp

HYBRID_SRC = $(SRC)/hpc_conv_hybrid.cpp $(SRC)/main_hybrid.cpp

#####################################################

########### MAKE RULES: DO NOT CHANGE!!! ############

all: hpc_conv_st hpc_conv_omp hpc_conv_mpi hpc_conv_hybrid

hpc_conv_st: $(SERIAL_SRC) $(DEPS)
	$(CXX) $(CXXFLAGS) $(SERIAL_SRC) -o $@
	
hpc_conv_omp: $(OMP_SRC) $(DEPS)
	$(CXX) $(CXXFLAGS) $(OMP_SRC) -o $@
	
hpc_conv_mpi: $(MPI_SRC) $(DEPS)
	$(MPICXX) $(CXXFLAGS) $(MPI_SRC) -o $@
	
hpc_conv_hybrid: $(HYBRID_SRC) $(DEPS)
	$(MPICXX) $(CXXFLAGS) $(HYBRID_SRC) -o $@

clean:
	rm -f hpc_conv* *~
	
#####################################################
