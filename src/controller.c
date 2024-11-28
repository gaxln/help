#include "controller.h"
#include "model.h"
#include <stdio.h>

void controller_update(Model_ *model, View *view) {
  if (view->btn_send_client_form == 1) {
    printf("%s, %s, %s\n", view->cf_data.name, view->cf_data.email,
           view->cf_data.id);
  }
}
