#include "Grid.h"
#include "geometry_utilities.h"
#include "grid_utilities.h"
#include "Neighbours.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

#define EPSILON     1e-6
#define NODATA      -9999.9
#define MAX_WEIGHT  4.
#define NO_INDEX    -100

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
    this->datax = nullptr;
    this->datay = nullptr;
    this->datamag = nullptr;
    this->datadir = nullptr;
}

Grid::Grid(Grid& other) {
    int rows = other.nrows;
    int cols = other.ncols;

    this->lower_left = other.lower_left;

    this->angle_deg = other.angle_deg;
    this->angle_rad = other.angle_rad;

    this->dx = other.dx;
    this->dy = other.dy;

    this->nrows = rows;
    this->ncols = cols;

    this->rotation_matrix = create_rotation_matrix(other.get_angle_deg());

    strncpy(this->name, other.get_name(), 100);

    size_t data_size = rows * cols * sizeof(double);
    if (other.datax) {
        this->datax = (double*)malloc(data_size);
        memcpy(this->datax, other.datax, data_size);
    }

    if (other.datay) {
        this->datay = (double*)malloc(data_size);
        memcpy(this->datay, other.datay, data_size);
    }

    if (other.datamag) {
        this->datamag = (double*)malloc(data_size);
        memcpy(this->datamag, other.datamag, data_size);
    }

    if (other.datadir) {
        this->datadir = (double*)malloc(data_size);
        memcpy(this->datadir, other.datadir, data_size);
    }
}

Grid::~Grid() {
    if (this->datax)
        free(this->datax);
    if (this->datay)
        free(this->datay);
    if (this->datamag)
        free(this->datamag);
    if (this->datadir)
        free(this->datadir);
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
    char* out = (char*)malloc(strlen(this->name) + 1);
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

bool Grid::allocate_x() {
    this->datax = (double*)malloc(this->nrows * this->ncols * sizeof(double));
    if (!this->datax)
        return false;

    std::fill_n(this->datax, this->nrows * this->ncols, NODATA);
    return true;
}

bool Grid::allocate_y() {
    this->datay = (double*)malloc(this->nrows * this->ncols * sizeof(double));
    if (!this->datay)
        return false;

    std::fill_n(this->datay, this->nrows * this->ncols, NODATA);
    return true;
}

bool Grid::allocate_mag() {
    this->datamag = (double*)malloc(this->nrows * this->ncols * sizeof(double));
    if (!this->datamag)
        return false;

    std::fill_n(this->datamag, this->nrows * this->ncols, NODATA);
    return true;
}

bool Grid::allocate_dir() {
    this->datadir = (double*)malloc(this->nrows * this->ncols * sizeof(double));
    if (!this->datadir)
        return false;

    std::fill_n(this->datadir, this->nrows * this->ncols, NODATA);
    return true;
}

bool Grid::allocate_data() {
    if (!this->allocate_x())
        return false;
    if (!this->allocate_y())
        return false;
    if (!this->allocate_mag())
        return false;
    if (!this->allocate_dir())
        return false;
    return true;
}

void Grid::get_neighbours4(Neighbours4& neighbours, const Node& node) {
    double xtemp = node.x - this->lower_left.x;
    double ytemp = node.y - this->lower_left.y;

    double xratio = xtemp / dx;
    double yratio = ytemp / dy;

    int icol = floor(xratio);
    int icol_next = ceil(xratio);

    int irow = ceil(yratio);
    int irow_next = ceil(yratio);

    int rows[4] = {irow, irow, irow_next, irow_next};
    int cols[4] = {icol, icol_next, icol, icol_next};

    for (auto i = 0; i < 4; ++i)
        this->assign_to_neighbour4(neighbours, i, rows[i], cols[i], node);
}

void Grid::get_neighbours16(Neighbours16& neighbours, const Node& node) {
    double xtemp = node.x - this->lower_left.x;
    double ytemp = node.y - this->lower_left.y;

    double xratio = xtemp / dx;
    double yratio = ytemp / dy;

    int icol = floor(xratio);
    int icol_next = ceil(xratio);

    int irow = ceil(yratio);
    int irow_next = ceil(yratio);

    int rows[16] = {irow - 1, irow, irow_next, irow_next + 1,
                    irow - 1, irow, irow_next, irow_next + 1,
                    irow - 1, irow, irow_next, irow_next + 1,
                    irow - 1, irow, irow_next, irow_next + 1};
    int cols[16] = {icol - 1, icol - 1, icol - 1, icol - 1,
                    icol, icol, icol, icol,
                    icol_next, icol_next, icol_next, icol_next,
                    icol_next + 1, icol_next + 1, icol_next + 1, icol_next + 1};

    for (auto i = 0; i < 16; ++i)
        this->assign_to_neighbour16(neighbours, i, rows[i], cols[i], node);
}

void Grid::assign_to_neighbour4(Neighbours4& neighbours, int nid, int row, int col, const Node& node) {
    Node temp_node;
    bool ok = this->get_node(row, col, temp_node);
    if (ok) {
        neighbours.idx[nid] = row * this->ncols + col;
        double d = distance_node_to_node(node.x, node.y, temp_node.x, temp_node.y);
        if (d < 10. * EPSILON)
            neighbours.weight[nid] = MAX_WEIGHT;
        else
            neighbours.weight[nid] = 1. / d;
    }
    else
        neighbours.idx[nid]  = NO_INDEX;
}

void Grid::assign_to_neighbour16(Neighbours16& neighbours, int nid, int row, int col, const Node& node) {
    Node temp_node;
    bool ok = this->get_node(row, col, temp_node);
    if (ok) {
        neighbours.idx[nid] = row * this->ncols + col;
        double d = distance_node_to_node(node.x, node.y, temp_node.x, temp_node.y);
        if (d < 10. * EPSILON)
            neighbours.weight[nid] = MAX_WEIGHT;
        else
            neighbours.weight[nid] = 1. / d;
    }
    else
        neighbours.idx[nid]  = NO_INDEX;
}







