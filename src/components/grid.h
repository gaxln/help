#pragma once
#include "../model.h"
#include <raylib.h>

#define MAX_GRID_ROWS 20
#define MAX_GRID_COLS 20

typedef struct {
  char *content;
  int size;
  int spacing;
  Color color;
} Text;

typedef struct {
  int day;
  int week;
  int month;
} Date;

typedef struct {
  Rectangle rect;
  Color current_color;
  Color unselected_color;
  Color selected_color;
  bool selected;
  Text text;
  Vector2 size;
  bool selectable;
  Date date;
  Employee employee;
  bool is_available;
  bool is_header;
} Cell;

#define MAX_RESERVATIONS 100

typedef struct {
  int day;
  int week;
  Employee employee;
  int month;
  char *client_name; // Nombre del cliente
  Cell *ref_cell;
} Reservation;

typedef struct {
  Cell cells[MAX_GRID_ROWS][MAX_GRID_COLS];
  int rows;
  int cols;
  Vector2 size;
  Vector2 offset;
  Vector2 padding;
  Cell *currented_selected_cell;
  Font curr_font;
  Reservation reservations[MAX_RESERVATIONS];
  int reservation_count;
  char client_name[64];
} Grid;

Grid init_grid(int rows, int cols, Vector2 cell_size, Vector2 padding,
               Vector2 offset, Color selected_color, Color unselected_color);

void add_grid_col(Grid *grid, Color unselected_color, Color selected_color,
                  Vector2 size);

void change_selectability(Cell *cell, bool is_selectable);

void draw_grid(Grid grid, Font font);
void update_grid(Grid *grid);

void toggle_grid_cell(Grid *grid, Cell *cell);

void handle_grid_input(Grid *grid);

Cell *get_cell(Grid *grid, int row, int col);

void change_cell_selected_color(Cell *cell, Color new_selected_color);
void change_cell_unselected_color(Cell *cell, Color new_unselected_color);

void draw_outline_grid(Grid *grid, int from_row, int from_col, int to_row,
                       int to_col);

void deselect_all_grid_cells(Grid *grid, Cell *cell);

void add_grid_content(Cell *cell, char *content, int font_size, int spacing,
                      Color tint);
void resize_grid_row_width(Grid *grid, int row_index, float new_width);

void set_cell_day_employee(Cell *cell, int day, Employee employee,
                           bool is_available);

bool add_reservation(Grid *schd, int day, int week, Employee employee,
                     char *client_name);

bool is_available(Grid *schd, int day, int week, Employee employee);

void list_reservations(Grid *grid);
