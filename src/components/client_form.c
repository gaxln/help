#include "client_form.h"
#include <raygui.h>

ClientForm create_client_form(struct View *view) {
  ClientForm cf = {0};
  cf.btn_send = 0;
  cf.container = (Rectangle){
      .x = view->schd.calendar.size.x + 20,
      .y = 10,
      .width = 1340 - (view->schd.calendar.size.x),
      .height = 448,
  };
  return cf;
}

void draw_client_form(ClientForm *cf, struct View *view,
                      Font fonts[NUM_FONTS]) {
  GuiSetFont(fonts[0]);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 28);
  cf->btn_send = GuiButton(
      (Rectangle){
          .x = cf->container.width + cf->container.x - 130,
          .y = cf->container.height - cf->container.y - 40,
          .width = 110,
          .height = 40,
      },
      "Reservar");
}
