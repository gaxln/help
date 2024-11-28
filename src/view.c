#include "view.h"
#include "colors.h"
#include "components/schedule.h"
#include "defs.h"
#include "model.h"
#include <raygui.h>
#include <raylib.h>
#include <stdio.h>
#include <string.h>

void view_init(View *view) {
  view->cf_data = (ClientFormData){
      "",
      "",
      "youremail@email.com",
      "",
  };
  view->schd = create_schedule((Vector2){20.0, 20.0});
  view->btn_send_client_form = 0;
  view->cf_container = (Rectangle){
      .x = view->schd.calendar.size.x + 20,
      .y = 10,
      .width = 1340 - (view->schd.calendar.size.x),
      .height = 448,
  };
  int font_size = 25;
}

void view_draw(View *view, Font fonts[NUM_FONTS]) {
  draw_schedule(&view->schd, fonts);
  DrawRectangleRounded(view->cf_container, 0.1, 1, PRIMARY);
  draw_client_form(view, fonts);
  // draw_grid(&view->test_grid, font);
}

void draw_client_form(View *view, Font fonts[NUM_FONTS]) {
  GuiSetFont(fonts[0]);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 28);

  static int activeTextBox = -1;

  view->btn_send_client_form = GuiButton(
      (Rectangle){
          .x = view->cf_container.width + view->cf_container.x - 130,
          .y = view->cf_container.height - view->cf_container.y - 40,
          .width = 110,
          .height = 40,
      },
      "Reservar");
  GuiLabel(
      (Rectangle){
          .x = view->cf_container.x + 20,
          .y = view->cf_container.y + 20,
          .width = 200,
          .height = 40,
      },
      "Nombre Completo");

  if (GuiTextBox(
          (Rectangle){
              .x = view->cf_container.x + 20,
              .y = view->cf_container.y + 55,
              .width = view->cf_container.width - 40,
              .height = 40,
          },
          view->cf_data.name, MAX_NAME_CHARS, activeTextBox == 0)) {
    activeTextBox = 0; // Activar la caja de texto Nombre
  }

  // Correo Electrónico
  GuiLabel(
      (Rectangle){
          .x = view->cf_container.x + 20,
          .y = view->cf_container.y + 100,
          .width = 200,
          .height = 40,
      },
      "Correo Electrónico");

  if (GuiTextBox(
          (Rectangle){
              .x = view->cf_container.x + 20,
              .y = view->cf_container.y + 55 + 80,
              .width = view->cf_container.width - 40,
              .height = 40,
          },
          view->cf_data.email, MAX_NAME_CHARS, activeTextBox == 1)) {
    activeTextBox = 1; // Activar la caja de texto Correo
  }

  // Número de celular
  GuiLabel(
      (Rectangle){
          .x = view->cf_container.x + 20,
          .y = view->cf_container.y + 180,
          .width = 200,
          .height = 40,
      },
      "Número de celular");

  if (GuiTextBox(
          (Rectangle){
              .x = view->cf_container.x + 20,
              .y = view->cf_container.y + 55 + (80 * 2),
              .width = view->cf_container.width - 40,
              .height = 40,
          },
          view->cf_data.phone, MAX_NAME_CHARS, activeTextBox == 2)) {
    activeTextBox = 2; // Activar la caja de texto Número de celular
  }

  // CC
  GuiLabel(
      (Rectangle){
          .x = view->cf_container.x + 20,
          .y = view->cf_container.y + 260,
          .width = 200,
          .height = 40,
      },
      "CC");

  if (GuiTextBox(
          (Rectangle){
              .x = view->cf_container.x + 20,
              .y = view->cf_container.y + 55 + (80 * 3),
              .width = view->cf_container.width - 40,
              .height = 40,
          },
          view->cf_data.id, MAX_NAME_CHARS, activeTextBox == 3)) {
    activeTextBox = 3; // Activar la caja de texto CC
  }
}

void view_update(View *view) {
  update_schedule(&view->schd);

  // Verificar si se presionó el botón "Reservar"
  if (view->btn_send_client_form) {
    // Obtener los datos del formulario
    char *name = view->cf_data.name;
    char *email = view->cf_data.email;
    char *phone = view->cf_data.phone;
    char *id = view->cf_data.id;

    // Valida que el formulario esté completo
    if (strlen(name) == 0 || strlen(email) == 0 || strlen(phone) == 0 ||
        strlen(id) == 0) {
      printf("Error: Todos los campos del formulario son obligatorios.\n");
      view->btn_send_client_form = 0;
      return;
    }

    // Encuentra la celda seleccionada en el calendario
    Cell *selected_cell = view->schd.calendar.currented_selected_cell;
    if (selected_cell && selected_cell->is_available) {
      // Realiza la reserva
      if (view->schd.calendar.reservation_count < MAX_RESERVATIONS) {
        view->schd.calendar
            .reservations[view->schd.calendar.reservation_count++] =
            (Reservation){.day = selected_cell->date.day,
                          .week = view->schd.current_week,
                          .employee = selected_cell->employee,
                          .client_name = strdup(name)};

        // Actualiza la celda seleccionada en el calendario
        selected_cell->is_available = false;
        selected_cell->current_color = ERRC; // Marca como no disponible
        add_grid_content(selected_cell, strdup("Reservado"), 25, 0, GRAY);

        printf("Reserva realizada: Cliente %s, Día %d, Semana %d\n", name,
               selected_cell->date.day, view->schd.current_week);
      } else {
        printf("Error: Límite máximo de reservas alcanzado.\n");
      }
    } else {
      printf("Error: No hay una celda seleccionada o la celda no está "
             "disponible.\n");
    }

    // Resetear el botón para que no se repita el evento
    view->btn_send_client_form = 0;
  }
}
