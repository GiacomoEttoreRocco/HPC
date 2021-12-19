#!/bin/bash
#cd $PBS_O_WORKDIR
for i in {2..48}
do
    mpirun -np $i ./tring 1>$i.txt 2>null
done
