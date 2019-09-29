#!/bin/bash

for i in `seq $1 $2`
do
    nn=$(printf "./genTeste70_6_3_6_2_map%d.sh > saida70_6_3_6_2_map%d.out" $i $i);
    echo $nn
    
    eval $nn 
done
