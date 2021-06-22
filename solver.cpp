#include "solver.h"

/*************************** DECLARE YOUR HELPER FUNCTIONS HERE ************************/



std::vector<std::vector<unsigned int>> partial_solns;
std::vector<std::vector<unsigned int>> worker_partial_solns;
std::vector<unsigned int> worker_solns;


//std::bitset<30> row,d1,d2;

int row[30] = {0};
int d1[30] = {0};
int d2[30] = {0};
int partial_soln_counter = 0;
int to_worker_counter = 0;
int worker_solns_count = 0;
int global_count = 0;
int c, cl, m = 0;
int num_procs;
int flag = 0;

//MPI_Request requests = (MPI_Request*) malloc(2*nprocs*sizeof(MPI_Request));

void worker_solver(int cl, int n, std::vector<unsigned int>& worker_partial_solns, int* worker_solns_count);

/*************************** solver.h functions ************************/


void seq_solver(unsigned int n, std::vector<std::vector<unsigned int> >& all_solns) {

	// TODO: Implement this function



}

void nqueen_master(	unsigned int n,
					unsigned int k,
					std::vector<std::vector<unsigned int> >& all_solns) {

	global_count++;
	if(global_count == 1){
		printf("In global COUNTER");
		m = k;
		k = 0;
		all_solns.push_back(std::vector<unsigned int>());
		partial_solns.push_back(std::vector<unsigned int>());
	}
	else 
		
		c = k;

	if(k==m){

		printf("Reached first partial solution!!!!");
		//check if there are any pending messages to be received
		//If yes, add them to the 'all_solns'
		//Per the 'counter', send the next partial solution to the waiting worker
		//Continue with discovering the partial solutions
		
		/*Create an MPI_IRecv() request*/
		/*Check if there is any waiting worker*/
		/*If yes, read the vector and paste it into the 'all_solns' vector*/
		/*As per the 'counter', provide the waiting worker with a new partial solution vector from the 2d vector*/
		/*Increment the 'counter'*/
		/*Delete the row from the partial solution vector*/
		/*If no, continue. Push a new vector to the partial solution vector*/
		/*And finally return*/
		MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
		
		MPI_Status status;
		MPI_Request request;
		int num_received = 0;
		int num = 1;
		
		if(partial_soln_counter < num_procs-1){
			//for(int j=0;j<num_procs;j++){
			MPI_Send(&num, 1, MPI_INT, partial_soln_counter+1, 1, MPI_COMM_WORLD);
			printf("Sent first fucking number!!!");
			//MPI_Send(&partial_solns.at(to_worker_counter)[0], m, MPI_INT, partial_soln_counter+1, 1, MPI_COMM_WORLD);
			MPI_Send(&partial_solns.back()[0], m, MPI_INT, partial_soln_counter+1, 1, MPI_COMM_WORLD);
			//MPI_Send(&all_solns.at(to_worker_counter), m, MPI_INT, partial_soln_counter+1, 1, MPI_COMM_WORLD);
			printf("Sent the whole partial row!!!!");
			MPI_Send(&row[0], n, MPI_INT, partial_soln_counter+1, 1, MPI_COMM_WORLD);
			MPI_Send(&d1[0], n, MPI_INT, partial_soln_counter+1, 1, MPI_COMM_WORLD);
			MPI_Send(&d2[0], n, MPI_INT, partial_soln_counter+1, 1, MPI_COMM_WORLD);
			to_worker_counter++;
			//partial_solns.pop_back();
			//}
		}
		
		//MPI_Irecv(&num_received,1,MPI_INT,MPI_ANY_SOURCE,1, MPI_COMM_WORLD, &request);
		MPI_Recv(&num_received,1,MPI_INT,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,&status);
		
		all_solns.push_back(partial_solns.back());
		//MPI_Recv(&all_solns.back()[0], num_received, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);		
		printf("Past MPI_Irecv");
		printf("%d\n", num_received);
		//MPI_Test(&request,&flag,&status);
		//MPI_Wait(&request,&status);
		printf("Past Test");
		
		if(1){
			printf("In flag");
		  	//int source = status.MPI_SOURCE;

			/*
			for(int j=0;j<num_received;j++){
				MPI_Recv(&partial_solns.back()[0], n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			}
			*/
			
			MPI_Recv(&all_solns.back()[0], num_received, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
			printf("I AM DONE FOR TODAY");
			printf("%d/n",num_received);
			
			int source = status.MPI_SOURCE;
			MPI_Send(&num, 1, MPI_INT, source, 1, MPI_COMM_WORLD);
		  	MPI_Send(&partial_solns.back()[0], m, MPI_INT, source, 1, MPI_COMM_WORLD);
			//MPI_Send(&all_solns.at(to_worker_counter)[0], m, MPI_INT, source, 1, MPI_COMM_WORLD);
			MPI_Send(&row[0], n, MPI_INT, source, 1, MPI_COMM_WORLD);
		  	MPI_Send(&d1[0], n, MPI_INT, source, 1, MPI_COMM_WORLD);
		  	MPI_Send(&d2[0], n, MPI_INT, source, 1, MPI_COMM_WORLD);
			
		  	to_worker_counter+= num_received;
		  	//partial_solns.pop_back();
			//all_solns.pop_back();
			flag = 0;
		}
		
		printf("Near return in MASTER");
 		partial_soln_counter++;
		partial_solns.push_back(partial_solns.back());
		//all_solns.push_back(std::vector<unsigned int>());
		//all_solns.push_back(all_solns.back());
		printf("In front of return");
		
		return;
	}

	
	for(int r=0;r<n;r++){
		if(!row[r] && !d1[k-r+n-1] && !d2[r+k]){
			printf("Before FUNCTION CALL");
			row[r] = d1[k-r+n-1] = d2[r+k] = 1;
			partial_solns.back().push_back(r);
			//all_solns.back().push_back(r);
			global_count++;
			printf("JUST BEFORE FUNCTION CALL");
			
			//nqueen_master(n,c+1,&partial_solns);
			nqueen_master(n,k+1,all_solns);
			printf("Popped Back one position for next iteration");
			row[r] = d1[k-r+n-1] = d2[r+k] = 0;
			partial_solns.back().pop_back();
			//all_solns.back().pop_back();
		}
	}
	
}

void nqueen_worker(	unsigned int n,
					unsigned int k) {

	/*MPI_Recv() to receive from the Master*/
	/*If Partial solution, then call worker_solver()*/
	/*After getting all the partial solutions, fill the 'worker_partial_solns' vector*/
	/*Send the vector 'worker_partial_solns' to the Master*/
	
	MPI_Status status;
	int num;
	std::vector<unsigned int> worker_solns;

	worker_solns.push_back(1);

	MPI_Recv(&num, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);

	printf("Received first fucking signal!!!");
	MPI_Recv(&worker_solns[0], k, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
	printf("Received the first fucking array!!!!");
	MPI_Recv(&row[0], n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
	printf("Received the ROW");
	MPI_Recv(&d1[0], n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
	printf("Received the D1");
	MPI_Recv(&d2[0], n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
	printf("Received the D2");
	

	while(num != 0){
		worker_solver(k+1,n,worker_solns,&worker_solns_count);
		
		worker_solns_count = n*worker_solns_count;
		printf("Solved the first fucking complete solution on worker");
		printf("%d\n", worker_solns_count);
		MPI_Request request;
		MPI_Send(&worker_solns_count, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
		/*
		for(int j=0;j<worker_solns_count;j++){
			MPI_Send(&worker_partial_solns.back()[0], n, MPI_INT, 0, 1, MPI_COMM_WORLD);
			worker_partial_solns.back().pop_back();
		}
		*/
		//MPI_Wait(&request,&status);

		MPI_Send(&worker_partial_solns.back()[0], worker_solns_count, MPI_INT, 0, 1, MPI_COMM_WORLD);
		printf("DID IT HAPPEN????");
		worker_solns_count = 0;
		worker_solns.clear();
		worker_solns.push_back(1);
		MPI_Recv(&num, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		printf("LALALAL");
		MPI_Recv(&worker_solns[0], k, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		printf("OWOWOWOW");
		MPI_Recv(&row[0], n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&d1[0], n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&d2[0], n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		
	}

}



/*************************** DEFINE YOUR HELPER FUNCTIONS HERE ************************/
void worker_solver(int cl, int n, std::vector<unsigned int>& worker_solns, int* worker_solns_count){
	if(cl==n){
		//worker_partial_solns.push_back(std::vector<unsigned int>());
		printf("Reached partial solution in worker!!!!");
		worker_partial_solns.push_back(worker_solns);
		*worker_solns_count = *worker_solns_count + 1;
		return;
		/**/
	}
	for(int r=0;r<n;r++){
		if(!row[r] && !d1[cl-r+n-1] && !d2[r+cl]){
			row[r] = d1[c-r+n-1] = d2[r+c] = 1;
			worker_solns.push_back(r);
			
			worker_solver(cl+1,n,worker_solns,worker_solns_count);
			
			row[r] = d1[c-r+n-1] = d2[r+c] = 0;
			worker_solns.pop_back();
		}
	}
}






