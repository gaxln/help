#include "schedule.h"
#include "../colors.h"
#include "grid.h"
#include <raygui.h>
#include <stdio.h>
#include <string.h>

const int font_size = 28;
const Vector2 size = {110, 75};
const Vector2 _offset = {20.0, 20.0};

Schedule create_schedule(Vector2 offset) {
  Schedule schd = {0};
  schd.num_employes = 0;
  schd.current_week = 0;
  schd.current_month_days = 31;
  schd.start_day = 1;
  schd.calendar =
      init_grid(7, 1, size, (Vector2){5, 5}, _offset, ACCENT, SECONDARY);

  for (int row = 0; row < schd.calendar.rows; row++) {
    for (int col = 0; col < schd.calendar.cols; col++) {
      change_selectability(get_cell(&schd.calendar, row, 0), false);
      change_selectability(get_cell(&schd.calendar, 0, col), false);
    }
  }

  add_grid_content(get_cell(&schd.calendar, 0, 0), "\nempleados", font_size, 0,
                   TEXTC);
  get_cell(&schd.calendar, 0, 0)->is_header = true;
  add_grid_content(get_cell(&schd.calendar, 1, 0), "LUN\n1", font_size, 0,
                   TEXTC);
  get_cell(&schd.calendar, 1, 0)->is_header = true;
  add_grid_content(get_cell(&schd.calendar, 2, 0), "MAR\n 2", font_size, 0,
                   TEXTC);
  get_cell(&schd.calendar, 2, 0)->is_header = true;
  add_grid_content(get_cell(&schd.calendar, 3, 0), "MIE\n 3", font_size, 0,
                   TEXTC);
  get_cell(&schd.calendar, 3, 0)->is_header = true;
  add_grid_content(get_cell(&schd.calendar, 4, 0), "JUE\n 4", font_size, 0,
                   TEXTC);
  get_cell(&schd.calendar, 4, 0)->is_header = true;
  add_grid_content(get_cell(&schd.calendar, 5, 0), "VIE\n 5", font_size, 0,
                   TEXTC);
  get_cell(&schd.calendar, 5, 0)->is_header = true;
  add_grid_content(get_cell(&schd.calendar, 6, 0), "SAB\n 6", font_size, 0,
                   TEXTC);
  get_cell(&schd.calendar, 6, 0)->is_header = true;

  schedule_add_employee(&schd, (Employee){.name = "Carlos"});
  schedule_add_employee(&schd, (Employee){.name = "Juan"});
  schedule_add_employee(&schd, (Employee){.name = "Alberto"});
  return schd;
}

void draw_schedule(Schedule *schedule, Font font[NUM_FONTS]) {
  GuiSetFont(font[1]);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
  schedule->container = (Rectangle){.x = _offset.x - 10.0,
                                    .y = _offset.y - 10.0,
                                    .width = schedule->calendar.size.x,
                                    .height = schedule->calendar.size.y};

  DrawRectangleRounded(schedule->container, 0.1, 1, SECONDARY);
  draw_grid(schedule->calendar, font[0]);

  schedule->btn_prev_week =
      GuiButton((Rectangle){.x = schedule->container.x + 10.0,
                            .y = schedule->container.y + 10.0,
                            .width = size.x / 2.0 + 1,
                            .height = 40.0},
                "");
  schedule->btn_next_week =
      GuiButton((Rectangle){.x = schedule->container.x + size.x / 2.0 + 11.0,
                            .y = schedule->container.y + 10.0,
                            .width = size.x / 2.0,
                            .height = 40.0},
                "");
}

void schedule_add_employee(Schedule *schedule, Employee employee) {
  add_grid_col(&schedule->calendar, SECONDARY, ACCENT, size);

  // Obtener la última columna añadida
  int last_col = schedule->calendar.cols - 1;

  // Añadir el nombre del empleado a la primera fila (encabezado)
  add_grid_content(get_cell(&schedule->calendar, 0, last_col), employee.name,
                   font_size, 0, TEXTC);
  get_cell(&schedule->calendar, 0, last_col)->is_header = true;

  // Desactivar selectabilidad de la celda de encabezado
  change_selectability(get_cell(&schedule->calendar, 0, last_col), false);
  for (int row = 1; row < schedule->calendar.rows; row++) {
    Cell *cell = get_cell(&schedule->calendar, row, last_col);

    // Calcular el número de día
    int day_number =
        row + (schedule->current_week * 7) - schedule->start_day + 1;

    // Establecer la celda como disponible si el día es válido
    if (day_number > 0 && day_number <= schedule->current_month_days) {
      set_cell_day_employee(cell, day_number, employee, true);

      // Actualizar el texto de la celda
      char cell_text[50];
      snprintf(cell_text, sizeof(cell_text), "Disp\n%d", day_number);
      add_grid_content(cell, strdup(cell_text), font_size, 0, TEXTC);

      // Establecer los colores de la celda basados en disponibilidad
      cell->unselected_color = SUCCEC;
      cell->current_color = SUCCEC;
    } else {
      // Si el día está fuera de rango
      set_cell_day_employee(cell, 0, employee, false);
      add_grid_content(cell, strdup("-"), font_size, 0, GRAY);

      // Establecer los colores de la celda basados en disponibilidad
      cell->unselected_color = ERRC;
      cell->current_color = ERRC;
    }
  }
  // Actualizar la vista del calendario para reflejar los cambios
  update_calendar_view(schedule);
}

void update_schedule(Schedule *schedule) {
  update_grid(&schedule->calendar);
  if (schedule->btn_prev_week) {
    previous_week(schedule);
  }
  if (schedule->btn_next_week) {
    advance_week(schedule);
  }
}

void advance_week(Schedule *schedule) {
  if ((schedule->current_week + 1) * 7 < schedule->current_month_days) {
    schedule->current_week++;
    update_calendar_view(schedule);
  }
}

void previous_week(Schedule *schedule) {
  if (schedule->current_week > 0) {
    schedule->current_week--;
    update_calendar_view(schedule);
  }
}

void update_calendar_view(Schedule *schedule) {

  for (int row = 1; row < schedule->calendar.rows; row++) {
    int day_number =
        row + (schedule->current_week * 7) - schedule->start_day + 1;

    char day_text[20];
    char *day_names[] = {"LUN", "MAR", "MIE", "JUE", "VIE", "SAB"};

    if (day_number > 0 && day_number <= schedule->current_month_days) {
      snprintf(day_text, sizeof(day_text), "%s\n%d", day_names[row - 1],
               day_number);

      for (int col = 1; col < schedule->calendar.cols; col++) {

        Cell *employee_cell = get_cell(&schedule->calendar, row, col);
        employee_cell->date.day = day_number;
        employee_cell->date.week = schedule->current_week;

        // Revisa si hay una reserva para esta celda
        bool reserved = false;
        for (int i = 0; i < schedule->calendar.reservation_count; i++) {
          if (schedule->calendar.reservations[i].day == day_number &&
              schedule->calendar.reservations[i].week ==
                  schedule->current_week &&
              strcmp(schedule->calendar.reservations[i].employee.name,
                     employee_cell->employee.name) == 0) {
            reserved = true;
            break;
          }
        }

        if (reserved) {
          employee_cell->is_available = false;
          employee_cell->current_color = ERRC;
          add_grid_content(employee_cell, strdup("Reservado"), font_size, 0,
                           GRAY);
        } else {
          employee_cell->is_available = true;
          employee_cell->current_color = SUCCEC;
          char cell_text[50];
          snprintf(cell_text, sizeof(cell_text), "Disp\n%d", day_number);
          add_grid_content(employee_cell, strdup(cell_text), font_size, 0,
                           TEXTC);
        }
      }
    } else {
      snprintf(day_text, sizeof(day_text), "%s\n-", day_names[row - 1]);
      for (int col = 1; col < schedule->calendar.cols; col++) {
        Cell *employee_cell = get_cell(&schedule->calendar, row, col);
        set_cell_day_employee(employee_cell, 0, (Employee){""}, false);
        employee_cell->current_color = ERRC;
        add_grid_content(employee_cell, strdup("-"), font_size, 0, GRAY);
      }
    }

    // Actualizar la columna de días
    add_grid_content(get_cell(&schedule->calendar, row, 0), strdup(day_text),
                     font_size, 0, TEXTC);
  }
}
