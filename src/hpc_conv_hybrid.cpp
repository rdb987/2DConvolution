#include<iostream>
#include "conv_hybrid.h"
#include "stopwatch.hpp"
#ifdef _OPENMP
#include<omp.h>
#endif

ConvolutionH::ConvolutionH(std::vector<int> input_matrix, std::vector<int> kernel_matrix, int rows, int columns, int krows, int kcolumns)
		: rows(rows),columns(columns),krows(krows), kcolumns(kcolumns)
		{
			Reset();
			input.reserve(rows*columns);
			kernel.reserve(krows*kcolumns);
			input=input_matrix;
			kernel=kernel_matrix;
		}

void ConvolutionH::Reset(){
	output.resize(rows*columns,0);
}

void ConvolutionH::Compute(){
	int index1, index2, a, b,x,y;
	#pragma omp parallel
	#pragma omp for private(x,y,a, b, index1, index2) 
	for(int z=0;z<rows*columns;++z){
		x=z/columns;
		y=z%columns;
		for(int i=0;i<krows;++i){
			for(int j=0;j<kcolumns;++j){
				a=x+i-krows/2;	
				b=y+j-kcolumns/2;					
				if(a<0)
					index1=rows+a;
				else if(a>rows-1)
					index1=a-rows;
				else 
					index1=a;
					
				if(b<0)
					index2=columns+b;
				else if(b>columns-1)
					index2=b-columns;
				else 
					index2=b;
				output[x*columns+y]+=input[index1*columns+index2]*kernel[i*kcolumns+j];
			}
		}
	}
}

void ConvolutionH::ComputeParallel(std::ofstream &f){
	int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int p;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
	stopwatch sw;

	MPI_Barrier(MPI_COMM_WORLD);
	sw.reset();
	sw.start();
	if(p==1 || p>rows)
		Compute();
	else{
		int half_kernel=krows/2;
		
		int forsize1=columns*half_kernel;
	  
	    // Scattering matrix per row to processes
		std::vector<int> prows(p); // number of rows per process
		std::vector<int> firstrow(p); // Index of first row per process
		int splitr;
		int extrar;
		splitr = rows / p;
		extrar = rows % p;
		
		for (int i = 0; i < extrar; ++i) 
			prows[i] = splitr + 1;
		for (int i = extrar; i < p; ++i) 
			prows[i] = splitr;
		for (int i = 1; i < p; ++i) 
			firstrow[i] = firstrow[i-1] + prows[i-1];
				
		int local_r = prows[rank];
		std::vector<int> local_mat(local_r*columns);
		for(int i=0;i<p;++i)
			prows[i]*=columns;
		for(int i=0;i<p;++i)
			firstrow[i]*=columns;
		
				
		MPI_Scatterv(&input[0], &prows[0], &firstrow[0], MPI_INT,
			&local_mat[0], (local_r*columns), MPI_INT,
		0, MPI_COMM_WORLD);
		
		// Exchange rows between processes
		if(half_kernel!=0  ) // For bigger kernels not needed
		{
			int slpartner = (rank + 1) % p;
			int rlpartner = (rank - 1 + p) % p;
			std::vector<int> first_rows(forsize1);
			std::vector<int> last_rows(forsize1);
			for(int l=0;l<forsize1;++l)
				first_rows[l]=local_mat[l];
		
			int tempvariable=(local_r-half_kernel)*columns;
			for(int l=0;l<forsize1;++l)
				last_rows[l]=local_mat[tempvariable+l];
		
			std::vector<int> first1(forsize1);
			std::vector<int> last1(forsize1);
		
		
			MPI_Sendrecv(&last_rows[0], forsize1, MPI_INT, slpartner, 0, &first1[0],forsize1 , MPI_INT, rlpartner, 0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			MPI_Sendrecv(&first_rows[0], forsize1, MPI_INT, rlpartner, 0,&last1[0], forsize1, MPI_INT, slpartner, 0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		 
			std::vector<int> local_mat_completed;
			local_mat_completed.reserve(2*forsize1+local_r*columns);
		
			local_mat_completed.insert(local_mat_completed.end(),first1.begin(),first1.end());
			local_mat_completed.insert(local_mat_completed.end(),local_mat.begin(),local_mat.end());
			local_mat_completed.insert(local_mat_completed.end(),last1.begin(),last1.end());
			local_mat=local_mat_completed;
		}
		
		std::vector<int> output_local_mat(local_r*columns,0);
		int index, b,x,y;
		#pragma omp parallel
		# pragma omp for private(x,y, b, index) 
		for(int z=0;z<local_r*columns;++z){
			x=z/columns;
			y=z%columns;
			for(int i=0;i<krows;++i){
				for(int j=0;j<kcolumns;++j){
				
					b=y+j-kcolumns/2;
					if(b<0)
						index=columns+b;
					else if(b>columns-1)
						index=b-columns;
					else 
						index=b;
					output_local_mat[x*columns+y]+=local_mat[(x+i)*columns+index]*kernel[i*kcolumns+j];
				}
			}
		}
		
		MPI_Gatherv(&output_local_mat[0], local_r*columns, MPI_INT,
		&output[0], &prows[0], &firstrow[0], MPI_INT,
		0, MPI_COMM_WORLD);
	}
	sw.stop();
	if(rank==0){
		std::cout << "Elapsed time: " << sw.elapsed_ms() << " ms" << std::endl;
		Print_to_File(f);
	}	
}
void ConvolutionH::Print_to_File(std::ofstream &f)
{
	if(!f)
	{
		std::cerr<<"Cannot open output file"<<std::endl;
		return ;
	}
	for(int i = 0; i < rows; i++){
			for(int j = 0; j < columns; j++){
				if(j!=columns-1)
					f <<output[i*columns+j]<<' ';
				else
					f <<output[i*columns+j];
			}
			f<<std::endl;
	}
	f.close();
}

ConvolutionH::~ConvolutionH() {}
					
					
				

