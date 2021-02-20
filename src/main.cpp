#include "Node.h"
#include "Grid.h"

#include <stdio.h>

void print_easy(const Grid& a_grid, int row, int col) {
    Node a_node;
    bool ok = a_grid.get_node(row, col, a_node);
    if (ok) {
        printf("Node at (%d, %d) :: (%f, %f) \n", row, col, a_node.x, a_node.y);
    }
    else
        printf("No node at (%d, %d) \n", row, col);
}


int main(int argc, char** argv) {
    printf("Running Bounding_box_grid... \n");

    double angle_deg = 45.;
    double xll = 2.0;
    double yll = 1.0;
    double resolution_x = 0.5;
    double resolution_y = 0.5;
    int nrows = 10;
    int ncols = 5;

    Grid my_grid(xll, yll, angle_deg, resolution_x, resolution_y, nrows, ncols);

    my_grid.display_info();
    print_easy(my_grid, 0, 0);
    print_easy(my_grid, 5, 5);
    print_easy(my_grid, 3, 2);

    print_easy(my_grid, 2, 10);
    print_easy(my_grid, 10, 2);

    Grid* my_bbox = my_grid.get_bounding_box(1.);
    my_bbox->set_name("Bounding box");
    my_bbox->display_info();

    printf("Done.\nGoodbye!\n");
    return 0;
}
