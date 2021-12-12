#!/bin/bash
#cd $PBS_O_WORKDIR
for i in {2..24}
do
    mpirun -np $i ./tring 1>$i.txt 2>null
done
