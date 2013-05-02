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
BLUEOUTDIR=~/data-sb
KRATOS=-O7 -DKRATOS
BLUE=-O3 -DBLUE

all:
	mpicc $(SOURCES) -o $(OUTPUT) $(KRATOS) -lm
	# run with "mpirun -np 4 ./aco_tsp input/FILE_NAME NUM_CITIES"
	
path_dist:
	gcc src/tsp_path_distance.c -o tsp_path_distance -Wall -lm
	# run with "./tsp_path_distance input/INPUT_FILE input/PATH_FILE NUM_CITIES"
	
blue:
	mpicc $(SOURCES) $(BLUE) -o $(BLUEOUTDIR)/$(OUTPUT)

clean:
	rm -r *~ $(OUTPUT)