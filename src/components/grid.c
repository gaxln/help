#include "grid.h"
#include "../colors.h"
#include "raygui.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>

Grid init_grid(int rows, int cols, Vector2 cell_size, Vector2 padding,
               Vector2 offset, Color selected_color, Color unselected_color) {
  Grid grid = {0};
  grid.rows = rows;
  grid.cols = cols;
  grid.client_name[0] = ' ';
  grid.padding = padding;
  grid.offset = offset;
  grid.reservation_count = 0;
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) {
      grid.cells[row][col] =
          (Cell){.current_color = unselected_color,
                 .unselected_color = unselected_color,
                 .selected_color = selected_color,
                 .size = cell_size,
                 .rect = (Rectangle){
                     .x = grid.offset.x + (cell_size.x + padding.x) * row,
                     .y = grid.offset.y + (cell_size.y + padding.y) * col,
                     .width = cell_size.x,
                     .height = cell_size.y,
                 }};
    }
  }
  grid.size =
      (Vector2){grid.cells[grid.rows - 1][grid.cols - 1].rect.x + cell_size.x,
                grid.cells[grid.rows - 1][grid.cols - 1].rect.y + cell_size.y};
  return grid;
}

void add_grid_col(Grid *grid, Color unselected_color, Color selected_color,
                  Vector2 size) {
  // Verificar si hay espacio para agregar una nueva columna
  if (grid->cols >= MAX_GRID_COLS) {
    printf(
        "Error: No se pueden agregar más columnas. Límite máximo alcanzado.\n");
    return;
  }

  // Iterar sobre todas las filas existentes para configurar la nueva columna
  for (int row = 0; row < grid->rows; row++) {
    // Configurar la nueva celda
    grid->cells[row][grid->cols] = (Cell){
        .current_color = unselected_color,
        .selected_color = selected_color,
        .unselected_color = unselected_color,
        .selectable = true,
        .selected = false,
        .size = size,
        .rect = (Rectangle){
            .x = grid->offset.x + (size.x + grid->padding.x) * row,
            .y = grid->offset.y + (size.y + grid->padding.y) * grid->cols,
            .width = size.x,
            .height = size.y}};
  }

  // Incrementar el número de columnas
  grid->cols++;

  // Actualizar el tamaño de la cuadrícula
  grid->size =
      (Vector2){grid->cells[grid->rows - 1][grid->cols - 1].rect.x + size.x,
                grid->cells[grid->rows - 1][grid->cols - 1].rect.y + size.y};
}

Cell *get_cell_at_position(Grid *grid, Vector2 position) {
  for (int row = 0; row < grid->rows; row++) {
    for (int col = 0; col < grid->cols; col++) {
      if (CheckCollisionPointRec(position, grid->cells[row][col].rect)) {
        return &grid->cells[row][col];
      }
    }
  }
  return NULL;
}

void update_grid(Grid *grid) { handle_grid_input(grid); }

void toggle_grid_cell(Grid *grid, Cell *cell) {
  if (cell != NULL && cell->selectable && !cell->is_header) {
    if (!cell->is_available) {
      printf("Esta celda no está disponible.\n");
      return; // Bloquea la selección de celdas no disponibles
    }

    deselect_all_grid_cells(grid, cell);

    cell->selected = true;
    cell->current_color = cell->selected_color;
    grid->currented_selected_cell = cell;

    printf("Celda seleccionada: Día %d, Semana %d, Empleado: %s\n",
           cell->date.day, cell->date.week, cell->employee.name);
  }
}

bool is_available(Grid *schd, int day, int week, Employee employee) {
  for (int i = 0; i < schd->reservation_count; i++) {
    if (schd->reservations[i].day == day &&
        schd->reservations[i].week == week &&
        strcmp(schd->reservations[i].employee.name, employee.name) == 0) {
      return false; // Ya está reservado
    }
  }
  return true; // Disponible
}

bool add_reservation(Grid *schd, int day, int week, Employee employee,
                     char *client_name) {
  if (!is_available(schd, day, week, employee)) {
    printf("Error: El empleado %s no está disponible el día %d, semana %d\n",
           employee.name, day, week);
    return false;
  }
  if (schd->reservation_count >= MAX_RESERVATIONS) {
    printf("Error: No se pueden agregar más reservas.\n");
    return false;
  }
  schd->reservations[schd->reservation_count++] = (Reservation){
      .day = day,
      .week = week,
      .employee = employee,
      .client_name = strdup(client_name),
  };
  printf("Reserva exitosa para %s con %s el día %d de la semana %d\n",
         client_name, employee.name, day, week);
  return true;
}

void handle_grid_input(Grid *grid) {
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 mouse_pos = GetMousePosition();
    Cell *cell = get_cell_at_position(grid, mouse_pos);
    if (cell != NULL) {
      toggle_grid_cell(grid, cell);
    }
  }
}

void draw_grid(Grid grid, Font font) {
  for (int row = 0; row < grid.rows; row++) {
    for (int col = 0; col < grid.cols; col++) {
      Cell cell = grid.cells[row][col];
      DrawRectangleRounded(cell.rect, 0.2, 64, cell.current_color);
      Vector2 text_measure = MeasureTextEx(font, cell.text.content,
                                           cell.text.size, cell.text.spacing);

      DrawTextEx(
          font, cell.text.content,
          (Vector2){cell.rect.x + (cell.rect.width - text_measure.x) / 2,
                    cell.rect.y + (cell.rect.height - text_measure.y) / 2},
          cell.text.size, cell.text.spacing, cell.text.color);
    }
  }
}

void draw_outline_grid(Grid *grid, int from_row, int from_col, int to_row,
                       int to_col) {

  from_row = (from_row < 0)             ? 0
             : (from_row >= grid->rows) ? grid->rows - 1
                                        : from_row;
  from_col = (from_col < 0)             ? 0
             : (from_col >= grid->cols) ? grid->cols - 1
                                        : from_col;
  to_row = (to_row < 0) ? 0 : (to_row >= grid->rows) ? grid->rows - 1 : to_row;
  to_col = (to_col < 0) ? 0 : (to_col >= grid->cols) ? grid->cols - 1 : to_col;

  // Dibujar los contornos de las celdas en el rango especificado
  for (int row = from_row; row <= to_row; row++) {
    for (int col = from_col; col <= to_col; col++) {
      Cell cell = grid->cells[row][col];
      // Dibujar un rectángulo de líneas alrededor de la celda
      DrawRectangleLinesEx(cell.rect, 2, BLACK);
    }
  }
}

void add_grid_content(Cell *cell, char *content, int font_size, int spacing,
                      Color tint) {
  cell->text.content = content;
  cell->text.size = font_size;
  cell->text.spacing = spacing;
  cell->text.color = tint;
}

Cell *get_cell(Grid *grid, int row, int col) { return &grid->cells[row][col]; }

void change_cell_selected_color(Cell *cell, Color new_selected_color) {
  cell->selected_color = new_selected_color;
}

void change_cell_unselected_color(Cell *cell, Color new_unselected_color) {

  cell->unselected_color = new_unselected_color;
}

void resize_grid_row_width(Grid *grid, int row_index, float new_width) {
  if (row_index < 0 || row_index >= grid->rows) {
    return; // Salir si el índice está fuera de rango
  }
  for (int col = 0; col < grid->cols; col++) {
    Cell *cell = &grid->cells[row_index][col];
    cell->rect.width = new_width; // Ajustar el ancho de la celda
    if (col > 0) {
      // Ajustar la posición X para evitar solapamientos
      cell->rect.x = grid->cells[row_index][col - 1].rect.x + new_width;
    }
  }
}

void deselect_all_grid_cells(Grid *grid, Cell *cell_to_exclude) {
  for (int row = 0; row < grid->rows; row++) {
    for (int col = 0; col < grid->cols; col++) {
      Cell *cell = &grid->cells[row][col];

      if (cell->is_header) {
        continue;
      }

      if (cell != cell_to_exclude) { // Excluye la celda seleccionada
        cell->selected = false;

        // Restaurar color según disponibilidad
        if (cell->is_available) {
          cell->current_color = cell->unselected_color;
        } else {
          cell->current_color = ERRC; // Color de "no disponible"
        }
      }
    }
  }
}

void change_selectability(Cell *cell, bool is_selectable) {
  if (cell != NULL) {
    cell->selectable = is_selectable;
    if (!is_selectable) {
      cell->current_color = BG;
      cell->selected = false;
    }
  }
}

void set_cell_day_employee(Cell *cell, int day, Employee employee,
                           bool is_available) {
  if (cell == NULL) {
    return;
  }
  cell->date.day = day;
  cell->employee.name = employee.name;
  cell->is_available = is_available;
}

void list_reservations(Grid *grid) {
  printf("Reservas actuales:\n");
  for (int i = 0; i < grid->reservation_count; i++) {
    printf("Cliente: %s, Empleado: %s, Día: %d, Semana: %d\n",
           grid->reservations[i].client_name,
           grid->reservations[i].employee.name, grid->reservations[i].day,
           grid->reservations[i].week);
  }
}
