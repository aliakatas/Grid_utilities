#ifndef GRID_H
#define GRID_H

#include "Node.h"

class Grid {
private:
    Node lower_left;
    double angle_deg;
    double angle_rad;
    double dx;
    double dy;
    int nrows;
    int ncols;
    double** rotation_matrix;
    char name[100];

public:
    Grid(double xll_, double yll_, double angle_deg_, double dx_, double dy_, int nrows_, int ncols_);
    Grid(Grid& other);

    Node get_lower_left();
    double get_angle_deg();
    double get_angle_rad();
    void get_size(int& rows, int& cols);
    void get_resolution(double& dx, double& dy);
    bool get_node(int irow, int icol, Node& node);
    Grid* get_bounding_box(double scale = 0.5);

    char* get_name();
    void set_name(const char* name_);

    void display_info();

};

#endif
