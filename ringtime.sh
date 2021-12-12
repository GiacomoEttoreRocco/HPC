#!/bin/bash
#cd $PBS_O_WORKDIR
for i in {2..24}
do
    touch timesRing/$i.txt
    mpirun -np $i ./tring > timesRing/$i.txt
done