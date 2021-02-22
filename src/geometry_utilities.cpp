#include "geometry_utilities.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

double** create_rotation_matrix(double angle_deg) {
    double angle_rad = angle_deg * M_PI / 180.;
    double** rotation_matrix;
    rotation_matrix = (double**)malloc(2 * sizeof(double*));
    rotation_matrix[0] = (double*)malloc(2 * sizeof(double));
    rotation_matrix[1] = (double*)malloc(2 * sizeof(double));

    rotation_matrix[0][0] = cos(angle_rad);
    rotation_matrix[0][1] = -sin(angle_rad);
    rotation_matrix[1][0] = sin(angle_rad);
    rotation_matrix[1][1] = cos(angle_rad);
    return rotation_matrix;
}

double distance_node_to_node(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}


