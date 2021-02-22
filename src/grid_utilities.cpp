#include "grid_utilities.h"
#include "Node.h"

Node rotate_coordinates(Node node, double** rotation_matrix, Node about_point) {
    // translate point
    node.x -= about_point.x;
    node.y -= about_point.y;

    // rotate point
    double xnew = node.x * rotation_matrix[0][0] + node.y * rotation_matrix[0][1];
    double ynew = node.x * rotation_matrix[1][0] + node.y * rotation_matrix[1][1];

    // translate point back:
    node.x = xnew + about_point.x;
    node.y = ynew + about_point.y;
    return node;
}


