#ifndef GRID_H
#define GRID_H

#include "Node.h"
#include "Neighbours.h"

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
    double* datax;
    double* datay;
    double* datamag;
    double* datadir;

    void assign_to_neighbour4(Neighbours4& neighbours, int nid, int row, int col, const Node& node);
    void assign_to_neighbour16(Neighbours16& neighbours, int nid, int row, int col, const Node& node);

public:
    Grid(double xll_, double yll_, double angle_deg_, double dx_, double dy_, int nrows_, int ncols_);
    Grid(Grid& other);
    ~Grid();

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

    bool allocate_x();
    bool allocate_y();
    bool allocate_mag();
    bool allocate_dir();
    bool allocate_data();

    void get_neighbours4(Neighbours4& neighbours, const Node& node);
    void get_neighbours16(Neighbours16& neighbours, const Node& node);

};

#endif
