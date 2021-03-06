/******************************************************************************/
/******************************************************************************/
/***** Forest Trimble,               ******************************************/
/***** Scott Todd,                   ******************************************/
/***** {trimbf,todds}@rpi.edu        ******************************************/
/***** Project:                      ******************************************/
/*****   Ant Colony Optimization,    ******************************************/
/*****   Travelling Salesman Problem ******************************************/
/***** Due: May 7, 2013              ******************************************/
/******************************************************************************/
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> // requires -lm at compilation
#include <unistd.h>
#include <float.h>

double **distances = NULL;

int num_cities;


/* ----------------------------------------------------------- */
/* | Distance setup tools                                    | */
struct city {
  int id_number;
  double x_coord, y_coord;
};

double city_distance(struct city city1, struct city city2) {
  return sqrt( pow((city1.x_coord - city2.x_coord), 2) + 
	       pow((city1.y_coord - city2.y_coord), 2) );
}
/* |                                                         | */
/* ----------------------------------------------------------- */


/* This function allocates a contiguous block of memory *
 * so that MPI_Isend/Irecv operations are much easier.  */
double ** alloc2dcontiguous(int rows, int cols) {
  int i;
  double *data = (double *)calloc(rows*cols, sizeof(double));
  double **array = (double **)calloc(rows, sizeof(double));

  for (i = 0; i < rows; ++i) {
    array[i] = &data[cols*i];
  }

  return array;
}

void print_matrix(double ** matrix, int rows, int cols) {
  int i, j;
  for (i=0; i < rows; i++) {
    for (j=0; j < cols; j++) {
      printf("%f ", matrix[i][j]);
    }
    printf("\n");
  }
}

// after calling this function, distances will be filled
// returns -1 on failure and 0 on success
int parse_input_file(char* input_file_path, int num_cities) {
  int i, j;
  char * this_line;
  char buffer[100];
    
  struct city* cities = (struct city*)calloc(num_cities, sizeof(struct city));
    
  FILE *input_file = fopen(input_file_path, "rb");
  if (input_file == NULL) {
    perror ("The following error occurred");
    return -1;
  }
    
  // skip header information
  this_line = fgets(buffer, 100, input_file); // NAME
  this_line = fgets(buffer, 100, input_file); // TYPE
  this_line = fgets(buffer, 100, input_file); // COMMENT
  this_line = fgets(buffer, 100, input_file); // DIMENSION
  this_line = fgets(buffer, 100, input_file); // EDGE_WEIGHT_TYPE
  this_line = fgets(buffer, 100, input_file); // NODE_COORD_SECTION
    
  // read cities
  for (i=0; i < num_cities; i++) {
    this_line = fgets(buffer, 100, input_file);
        
    sscanf(this_line, " %d %lf %lf", 
	   &(cities[i].id_number), 
	   &(cities[i].x_coord), 
	   &(cities[i].y_coord)
	   );
  }
    
  // calculate and store distances and their inverses
  for (i=0; i < num_cities; i++)
    for (j=0; j < num_cities; j++)
      distances[i][j] = city_distance(cities[i], cities[j]);
    
  free(cities);
  fclose(input_file);
    
  return 0;
}


int parse_path_file(char* path_file_path, int num_cities) {
  int i;
  char buffer[100];
    
  FILE *path_file = fopen(path_file_path, "rb");
  if (path_file == NULL) {
    perror ("The following error occurred");
    return -1;
  }
    
  // skip header information
  fgets(buffer, 100, path_file); // NAME
  fgets(buffer, 100, path_file); // TYPE
  fgets(buffer, 100, path_file); // COMMENT
  fgets(buffer, 100, path_file); // DIMENSION
  fgets(buffer, 100, path_file); // TOUR_SECTION
    
  // read cities
  int first_city_number, prev_city_number, current_city_number;
  double distance = 0;
  fscanf(path_file, "%d", &prev_city_number);
  first_city_number = prev_city_number;

  for (i=1; i < num_cities; i++) {
    fscanf(path_file, "%d", &current_city_number);
        
    distance += distances[prev_city_number-1][current_city_number-1];
        
    prev_city_number = current_city_number;
  }

  distance += distances[prev_city_number-1][first_city_number-1]; // returns to the start
    
  printf("distance: %f\n", distance);
    
  fclose(path_file);
    
  return 0;
}




int main(int argc, char *argv[]) {

  if (argc != 4) {
    fprintf(stderr, "ERROR: Invalid usage.\nUSAGE: %s LOCATIONS PATH COUNT\n", argv[0]);
    return EXIT_FAILURE;
  }
    
  sscanf(argv[3], "%d", &num_cities);
    
  distances = alloc2dcontiguous(num_cities, num_cities);
    
  if ( parse_input_file(argv[1], num_cities) != 0) return EXIT_FAILURE;
  if (parse_path_file(argv[2], num_cities) != 0) return EXIT_FAILURE;
    
  /* --------------------------------------------------------------------- */
  /* | Memory deallocation and exit                                      | */
    
  /* frees up the memory allocated for our arrays. *
   * recall that the array was initiated in one    *
   * contiguous chunk, so one call to free should  *
   * deallocate the whole underlying structure.    */
     
  free(&distances[0][0]);                     free(distances);
    
  return EXIT_SUCCESS;
  /* |                                                                   | */
  /* --------------------------------------------------------------------- */
}
