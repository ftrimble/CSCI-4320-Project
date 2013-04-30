###############################################################################
###############################################################################
##### Forest Trimble,                   #######################################
##### Scott Todd                        #######################################
##### {trimbf,todds}@rpi.edu            #######################################
##### Project:                          #######################################
#####   Ant Colony Optimization,        #######################################
#####   Travelling Salesman Problem     #######################################
##### Due: May 7, 2013                  #######################################
###############################################################################
###############################################################################

SOURCES=src/aco_tsp.c
OUTPUT=aco_tsp
KRATOS=-O7 -DKRATOS
BLUE=-O3 -DBLUE

all:
	mpicc $(SOURCES) -o $(OUTPUT) $(KRATOS)
	# run with "mpirun -np 4 ./aco_tsp input/att48.tsp"

clean:
	rm -r *~ $(OUTPUT)
