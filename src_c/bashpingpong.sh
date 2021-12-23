cd $PBS_O_WORKDIR
make clean
module load openmpi-4.1.1+gnu-9.3.0
make
#su coda DSSC (thin nodes) e su coda DSSC_GPU (gpu nodes):
#openMPI:
#INFINIBAND:
	mpirun -np 2 --report-bindings --map-by node  ./IMB-MPI1 PingPong | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > oMinfmbn.txt
	mpirun -np 2 --report-bindings --map-by socket ./IMB-MPI1 PingPong | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > oMinfmbs.txt
	mpirun -np 2 --report-bindings --map-by core ./IMB-MPI1 PingPong  | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > oMinfmbc.txt
#TCP:
	mpirun -np 2 --mca pml ob1 --mca btl tcp,self --mca btl_tcp_if_include br0 --report-bindings --map-by node ./IMB-MPI1 PingPong | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > oMtcpmbn.txt
	mpirun -np 2 --mca pml ob1 --mca btl tcp,self --mca btl_tcp_if_include br0 --report-bindings --map-by socket ./IMB-MPI1 PingPong | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > oMtcpmbs.txt
	mpirun -np 2 --mca pml ob1 --mca btl tcp,self --mca btl_tcp_if_include br0 --report-bindings --map-by core ./IMB-MPI1 PingPong | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > oMtcpmbc.txt
#Vader:
	mpirun -np 2 --mca pml ob1 --mca btl vader,self --mca btl_tcp_if_include br0 --report-bindings --map-by socket ./IMB-MPI1 PingPong | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > oMvadmbs.txt
	mpirun -np 2 --mca pml ob1 --mca btl vader,self --mca btl_tcp_if_include br0 --report-bindings --map-by core ./IMB-MPI1 PingPong | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > oMvadmbc.txt
make clean 
module load intel
make
#Intel:
#INIFINIBAND:
	mpiexec -np 2 -ppn 1 ./IMB-MPI1 PingPong | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > intelinfmbn.txt
	mpiexec -np 2 -env I_MPI_DEBUG 5 -genv I_MPI_PIN_PROCESSOR_LIST 0,1 ./IMB-MPI1 PingPong | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > intelinfmbs.txt
	mpiexec -np 2 -env I_MPI_DEBUG 5 -genv I_MPI_PIN_PROCESSOR_LIST 0,2 ./IMB-MPI1 PingPong | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > intelinfmbc.txt
#TCP: 	
	mpiexec -np 2 -ppn 1 -genv I_MPI_OFI_PROVIDER tcp ./IMB-MPI1 PingPong | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > inteltcpmbn.txt
	mpiexec -np 2 -genv I_MPI_PIN_PROCESSOR_LIST 0,1 -genv I_MPI_OFI_PROVIDER tcp ./IMB-MPI1 PingPong | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > inteltcpmbs.txt
	mpiexec -np 2 -genv I_MPI_PIN_PROCESSOR_LIST 0,2 -genv I_MPI_OFI_PROVIDER tcp ./IMB-MPI1 PingPong | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > inteltcpmbc.txt
#SHM: 	
	mpiexec -np 2 -genv I_MPI_PIN_PROCESSOR_LIST 0,1 -genv I_MPI_OFI_PROVIDER shm ./IMB-MPI1 PingPong | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > intelvadmbs.txt
	mpiexec -np 2 -genv I_MPI_PIN_PROCESSOR_LIST 0,2 -genv I_MPI_OFI_PROVIDER shm ./IMB-MPI1 PingPong | grep -v ^# | grep -v -e '^$' |  tr -s ' '| sed 's/^[ \t]+*//g' | sed 's/[ \t]+*/,/g' > intelvadmbc.txt
