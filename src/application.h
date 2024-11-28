#pragma once
#include "model.h"
#include "view.h"

typedef struct {
  int width;
  int height;
  char *title;
} AppConfig;

typedef struct {
  Font fonts[NUM_FONTS];
  AppConfig config;
  View _view;
  Model_ _model;
} App;

void app_init(App *app, AppConfig config);
void _app_update(App *app);
void _app_draw(App *app);
void app_close(App *app);
void app_run(App *app);
