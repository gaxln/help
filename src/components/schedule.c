#include "schedule.h"
#include "../colors.h"
#include "grid.h"
#include <errno.h>
#include <raygui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const char *month_name[] = {"Enero",      "Febrero", "Marzo",     "Abril",
                            "Mayo",       "Junio",   "Julio",     "Agosto",
                            "Septiembre", "Octubre", "Noviembre", "Diciembre"};

const int font_size = 28;
const Vector2 size = {110, 75};
const Vector2 _offset = {20.0, 20.0};

Schedule create_schedule(Vector2 offset) {
  Schedule schd = {0};
  schd.num_employes = 0;
  schd.current_week = 0;
  schd.current_month = 0; // Iniciar en enero
  schd.current_month_days = days_in_month[schd.current_month];
  schd.start_day = 1;
  schd.calendar =
      init_grid(7, 1, size, (Vector2){5, 5}, _offset, ACCENT, SECONDARY);

  // Limpiar reservaciones existentes al iniciar
  schd.calendar.reservation_count = 0;

  for (int row = 0; row < schd.calendar.rows; row++) {
    for (int col = 0; col < schd.calendar.cols; col++) {
      change_selectability(get_cell(&schd.calendar, row, col), false);
      change_selectability(get_cell(&schd.calendar, 0, col), false);
    }
  }

  char *day_names[] = {"LUN", "MAR", "MIE", "JUE", "VIE", "SAB", "DOM"};
  get_cell(&schd.calendar, 0, 0)->is_header = true;
  add_grid_content(get_cell(&schd.calendar, 0, 0), "\nempleados", font_size, 0,
                   TEXTC);

  for (int i = 0; i < 7; i++) {
    if (i > 0) {
      get_cell(&schd.calendar, i, 0)->is_header = true;
      add_grid_content(get_cell(&schd.calendar, i, 0), strdup(day_names[i - 1]),
                       font_size, 0, TEXTC);
    }
  }

  schedule_add_employee(&schd, (Employee){.name = "Jon Planas"});
  schedule_add_employee(&schd, (Employee){.name = "Patricia"});
  schedule_add_employee(&schd, (Employee){.name = "Christian"});
  schedule_add_employee(&schd, (Employee){.name = "Anselmo"});
  schedule_add_employee(&schd, (Employee){.name = "Aimar"});
  schedule_add_employee(&schd, (Employee){.name = "Yago"});
  schedule_add_employee(&schd, (Employee){.name = "Nestor"});
  return schd;
}

void draw_schedule(Schedule *schedule, Font font[NUM_FONTS]) {
  schedule->container = (Rectangle){.x = _offset.x - 10.0,
                                    .y = _offset.y - 10.0,
                                    .width = schedule->calendar.size.x,
                                    .height = schedule->calendar.size.y};

  DrawRectangleRounded(schedule->container, 0.1, 1, SECONDARY);
  draw_grid(schedule->calendar, font[0]);

  // Dibujar mes
  char month_text[50];
  snprintf(month_text, sizeof(month_text), "%s",
           month_name[schedule->current_month]);

  GuiSetStyle(DEFAULT, TEXT_SIZE, 28);
  GuiSetFont(font[0]);
  GuiLabel(
      (Rectangle){
          .x = schedule->container.x + schedule->container.width / 2 - 100,
          .y = schedule->container.y + schedule->container.height + 10,
          .width = 200,
          .height = 40,
      },
      month_text);

  GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
  GuiSetFont(font[1]);
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
      // snprintf(cell_text, sizeof(cell_text), "Disp\n%d", day_number);
      add_grid_content(cell, "Disp", font_size, 0, TEXTC);

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

void update_calendar_view(Schedule *schedule) {
  char *day_names[] = {"LUN", "MAR", "MIE", "JUE", "VIE", "SAB", "DOM"};

  int first_day_of_month = schedule->start_day; // Primer día del mes
  int current_month_days = schedule->current_month_days;

  for (int row = 1; row < schedule->calendar.rows; row++) {
    // Calcular el día del mes para esta fila
    int day_number =
        first_day_of_month + (schedule->current_week * 7) + (row - 1);

    // Validar si el día está dentro del mes actual
    if (day_number < 1 || day_number > current_month_days) {
      // Celda fuera del mes actual
      char day_text[50];
      snprintf(day_text, sizeof(day_text), "%s\n-", day_names[row - 1]);

      for (int col = 1; col < schedule->calendar.cols; col++) {
        Cell *employee_cell = get_cell(&schedule->calendar, row, col);
        set_cell_day_employee(employee_cell, 0, (Employee){""}, false);
        employee_cell->current_color = ERRC;
        add_grid_content(employee_cell, strdup("-"), font_size, 0, GRAY);
      }

      add_grid_content(get_cell(&schedule->calendar, row, 0), strdup(day_text),
                       font_size, 0, TEXTC);
      continue;
    }

    // Día válido del mes actual
    char day_text[50];
    snprintf(day_text, sizeof(day_text), "%s\n%d", day_names[row - 1],
             day_number);

    for (int col = 1; col < schedule->calendar.cols; col++) {
      Cell *employee_cell = get_cell(&schedule->calendar, row, col);

      // Actualizar la fecha de la celda
      employee_cell->date.day = day_number;
      employee_cell->date.week = schedule->current_week;
      employee_cell->date.month = schedule->current_month;

      // Buscar reservación correspondiente
      bool reserved = false;
      for (int i = 0; i < schedule->calendar.reservation_count; i++) {
        Reservation *res = &schedule->calendar.reservations[i];

        // Validar que la reservación coincide con el día y el mes actual
        if (res->day == day_number && res->month == schedule->current_month &&
            strcmp(res->employee.name, employee_cell->employee.name) == 0) {
          reserved = true;
          break;
        }
      }

      // Actualizar la celda con el estado correspondiente
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
        add_grid_content(employee_cell, strdup(cell_text), font_size, 0, TEXTC);
      }
    }

    // Actualizar la columna de días
    add_grid_content(get_cell(&schedule->calendar, row, 0), strdup(day_text),
                     font_size, 0, TEXTC);
  }
}

void advance_week(Schedule *schedule) {
  int current_month_days = days_in_month[schedule->current_month];
  int max_weeks =
      (current_month_days + 6) / 7; // Calcular número de semanas en el mes

  schedule->current_week++;

  // Si hemos pasado todas las semanas del mes, cambiar al siguiente mes
  if (schedule->current_week >= max_weeks) {
    schedule->current_week = 0;
    schedule->current_month = (schedule->current_month + 1) % 12;
    schedule->current_month_days = days_in_month[schedule->current_month];
  }

  update_calendar_view(schedule);
}

void previous_week(Schedule *schedule) {
  if (schedule->current_week > 0) {
    schedule->current_week--;
  } else {
    // Ir al mes anterior
    schedule->current_month = (schedule->current_month - 1 + 12) % 12;

    // Calcular número de semanas en el mes anterior
    int previous_month_days = days_in_month[schedule->current_month];
    int max_weeks = (previous_month_days + 6) / 7;

    schedule->current_week = max_weeks - 1;
    schedule->current_month_days = previous_month_days;
  }

  update_calendar_view(schedule);
}

void save_reservations(const char *filename, Schedule *schedule) {
  FILE *file = fopen(filename, "w");
  if (!file) {
    perror("Error al abrir el archivo para guardar");
    return;
  }

  for (int i = 0; i < schedule->calendar.reservation_count; i++) {
    Reservation *res = &schedule->calendar.reservations[i];
    fprintf(file, "%d %d %s %s\n", res->day, res->month, res->employee.name,
            res->client_name);
  }

  fclose(file);
  printf("Reservaciones guardadas en %s\n", filename);
}

void load_reservations(const char *filename, Schedule *schedule) {
  // Reset reservation count before loading
  schedule->calendar.reservation_count = 0;

  // Verificar el directorio actual y la ruta completa del archivo
  char current_path[1024];
  if (getcwd(current_path, sizeof(current_path)) != NULL) {
    printf("Directorio actual: %s\n", current_path);
    char full_path[1280];
    snprintf(full_path, sizeof(full_path), "%s/%s", current_path, filename);
    printf("Ruta completa del archivo: %s\n", full_path);
  }

  FILE *file = fopen(filename, "r");
  if (!file) {
    // Informe de error más detallado
    fprintf(stderr, "Error abriendo %s: %s\n", filename, strerror(errno));

    // Verificaciones adicionales
    if (access(filename, F_OK) == -1) {
      printf("El archivo NO EXISTE\n");
    } else if (access(filename, R_OK) == -1) {
      printf("El archivo EXISTE pero NO tiene permisos de lectura\n");
    }
    return;
  }

  // Imprimir contenido completo del archivo
  printf("Contenido del archivo de reservaciones:\n");
  printf("-----------------------------------\n");
  int ch;
  while ((ch = fgetc(file)) != EOF) {
    putchar(ch);
  }
  printf("\n-----------------------------------\n");

  // Reiniciar el puntero del archivo
  rewind(file);

  char line[256];
  int line_count = 0;
  while (fgets(line, sizeof(line), file)) {
    line_count++;

    // Trimming whitespace
    line[strcspn(line, "\n")] = 0;

    // Saltarse líneas vacías
    if (strlen(line) == 0) {
      printf("Línea %d está vacía, saltando\n", line_count);
      continue;
    }

    Reservation res = {0};

    // Parsing más verboso
    int parsed = sscanf(line, "%d %d %49[^ ] %49[^\n]", &res.day, &res.month,
                        res.employee.name, res.client_name);

    if (parsed == 4) {
      printf("Parseado línea %d: Día %d, Mes %d, Empleado %s, Cliente %s\n",
             line_count, res.day, res.month, res.employee.name,
             res.client_name);

      // Validación de datos
      if (res.day > 0 && res.day <= 31 && res.month >= 0 && res.month < 12) {

        // Copiar nombre de cliente
        res.client_name = strdup(res.client_name);

        schedule->calendar
            .reservations[schedule->calendar.reservation_count++] = res;

        // Prevenir desbordamiento
        if (schedule->calendar.reservation_count >= MAX_RESERVATIONS) {
          fprintf(stderr, "Máximo de reservaciones alcanzado\n");
          break;
        }
      } else {
        printf("Datos inválidos en línea %d\n", line_count);
      }
    } else {
      printf("Error parseando línea %d. Parseados: %d\n", line_count, parsed);
    }
  }

  fclose(file);
  printf("Reservaciones cargadas: %d\n", schedule->calendar.reservation_count);

  // Actualizar vista del calendario
  update_calendar_view(schedule);
}
