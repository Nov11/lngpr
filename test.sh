#!/bin/bash
begnum=100000
endnum=1100000
interv=100000
echo base case run getpidn
for ((i = $begnum; i < $endnum; i += $interv )) do
	./getpidn $i
done
echo enable recording 
for ((i = $begnum; i < $endnum; i += $interv )) do
	./setupn $i
done
echo enable replaying
for ((i = $begnum; i < $endnum; i += $interv )) do
	./replayn $i
done
