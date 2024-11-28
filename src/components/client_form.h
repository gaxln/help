#pragma once

#include "../defs.h"
#include <raylib.h>

struct View;

typedef struct {
  Rectangle container;
  int btn_send;
} ClientForm;

ClientForm create_client_form(View *view);
void draw_client_form(ClientForm *cf, View *view, Font fonts[NUM_FONTS]);
