#pragma once
#include "components/grid.h"
#include "components/schedule.h"
#include "defs.h"
#include "model.h"
#include <raylib.h>

#define MAX_NAME_CHARS 64
#define MAX_EMAIL_CHARS 128
#define MAX_ID_CHARS 12
#define MAX_PHONE_CHARS 10

typedef struct {
  char name[MAX_NAME_CHARS];
  char id[MAX_ID_CHARS];
  char email[MAX_EMAIL_CHARS];
  char phone[MAX_PHONE_CHARS];
} ClientFormData;

typedef struct {
  Schedule schd;
  Rectangle cf_container;
  int btn_send_client_form;
  ClientFormData cf_data;
  Rectangle ef_container;
  int btn_send_employee_form;
} View;

void view_init(View *view);
void view_update(View *view);
void view_draw(View *view, Font fonts[NUM_FONTS]);
void draw_client_form(View *view, Font fonts[NUM_FONTS]);
void draw_employee_form(View *view, Font fonts[NUM_FONTS]);
