#include "Grid.h"
#include "geometry_utilities.h"
#include "grid_utilities.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define EPSILON 1e-6

Grid::Grid(double xll_, double yll_, double angle_deg_, double dx_, double dy_, int nrows_, int ncols_) {
    this->lower_left.x = xll_;
    this->lower_left.y = yll_;

    this->angle_deg = angle_deg_;
    this->angle_rad = angle_deg_ * M_PI / 180.;

    this->dx = dx_;
    this->dy = dy_;

    this->nrows = nrows_;
    this->ncols = ncols_;

    this->rotation_matrix = create_rotation_matrix(angle_deg_);

    memset(this->name, '\0', 100);
}

Grid::Grid(Grid& other) {
    this->lower_left = other.lower_left;

    this->angle_deg = other.angle_deg;
    this->angle_rad = other.angle_rad;

    this->dx = other.dx;
    this->dy = other.dy;

    this->nrows = other.nrows;
    this->ncols = other.ncols;

    this->rotation_matrix = create_rotation_matrix(other.get_angle_deg());

    strncpy(this->name, other.get_name(), 100);
}

Node Grid::get_lower_left() {
    return this->lower_left;
}

double Grid::get_angle_deg() {
    return this->angle_deg;
}

double Grid::get_angle_rad() {
    return this->angle_rad;
}

void Grid::get_size(int& rows, int& cols) {
    rows = this->nrows;
    cols = this->ncols;
}

void Grid::get_resolution(double& dx, double& dy) {
    dx = this->dx;
    dy = this->dy;
}

bool Grid::get_node(int irow, int icol, Node& node) {
    if (irow < 0 || irow > this->nrows)
        return false;
    if (icol < 0 || icol > this->ncols)
        return false;

    node.x = this->lower_left.x + icol * dx;
    node.y = this->lower_left.y + irow * dy;

    node = rotate_coordinates(node, this->rotation_matrix, this->lower_left);
    return true;
}

Grid* Grid::get_bounding_box(double scale) {
    if (abs(this->angle_deg) < EPSILON)
        return new Grid(*this);

    bool ok = false;
    Node low_right;
    Node up_right;
    Node up_left;

    ok = this->get_node(0, this->ncols, low_right);
    ok = this->get_node(this->nrows, this->ncols, up_right);
    ok = this->get_node(this->nrows, 0, up_left);

    double min_x = fmin(this->lower_left.x, fmin(low_right.x, fmin(up_right.x, up_left.x)));
    double max_x = fmax(this->lower_left.x, fmax(low_right.x, fmax(up_right.x, up_left.x)));

    double min_y = fmin(this->lower_left.y, fmin(low_right.y, fmin(up_right.y, up_left.y)));
    double max_y = fmax(this->lower_left.y, fmax(low_right.y, fmax(up_right.y, up_left.y)));

    int new_rows = ceil((max_y - min_y) / (scale * dy));
    int new_cols = ceil((max_x - min_x) / (scale * dx));

    return new Grid(min_x, min_y, 0.0, scale * dx, scale * dy, new_rows, new_cols);
}

char* Grid::get_name() {
    char* out = malloc(strlen(this->name) + 1);
    memset(out, '\0', strlen(this->name) + 1);
    strncpy(out, this->name, strlen(this->name));
    return out;
}

void Grid::set_name(const char* name_) {
    strncpy(this->name, name_, strlen(name_));
}

void Grid::display_info() {
    if (strlen(this->name) > 0)
        printf("Grid :: %s \n", this->name);
    else
        printf(" ** Grid ** \n");

    printf("Lower-left node :: (%f, %f) \n", this->lower_left.x, this->lower_left.y);
    printf("     Resolution :: (%f, %f) \n", this->dx, this->dy);
    printf("           Size :: (%d, %d) \n", this->nrows, this->ncols);
    printf(" Rotation angle :: %f deg \n", this->angle_deg);
}
