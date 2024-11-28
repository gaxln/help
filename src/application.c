#include "model.h"
#define RAYGUI_IMPLEMENTATION
#include "application.h"
#include "colors.h"
#include "controller.h"
#include "view.h"
#include <raygui.h>

int codepoints[2] = {0xf060, 0xf061};

void set_theme(App *app) {
  GuiSetFont(app->fonts[1]);
  GuiSetStyle(DEFAULT, BORDER_WIDTH, 0);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 24);
  GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,
              ColorToInt((Color){236, 239, 244,
                                 255})); // Texto botón normal (Nord Snow Storm)
  GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,
              ColorToInt((Color){
                  59, 66, 82, 255})); // Fondo botón normal (Nord Polar Night)
  //  // Configurar estilo de botón para estado hover
  GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED,
              ColorToInt((Color){236, 239, 244,
                                 255})); // Texto botón hover (Nord Snow Storm)
  GuiSetStyle(
      BUTTON, BASE_COLOR_FOCUSED,
      ColorToInt((Color){
          67, 76, 94, 255})); // Fondo botón hover (Nord Polar Night más claro)

  GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, 0xdacbe1FF);

  GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL,
              ColorToInt(PRIMARY)); // Borde normal
  GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED,
              0xCBA6F7FF); // Borde cuando está enfocado
  GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED,
              0x9D79D6FF); // Borde cuando está presionado
  GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, 0x1E1E2EFF); // Fondo normal
  GuiSetStyle(TEXTBOX, BASE_COLOR_FOCUSED,
              0x2E2E40FF); // Fondo cuando está enfocado
  GuiSetStyle(TEXTBOX, BASE_COLOR_PRESSED,
              0x3B3B52FF); // Fondo cuando está presionado
  GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, 0xFFFFFFFF);  // Texto normal (blanco)
  GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED, 0xFFFFFFFF); // Texto enfocado
  GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED, 0xFFFFFFFF);
}

void app_load_resources(App *app) {
  app->fonts[0] =
      LoadFontEx("resources/Inter/static/Inter_28pt-Regular.ttf", 28, 0, 250);
  app->fonts[1] =
      LoadFontEx("resources/fontawesome/otfs/Font Awesome 6 Free-Solid-900.otf",
                 24, codepoints, 2);

  set_theme(app);
}
void app_init(App *app, AppConfig config) {
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
  InitWindow(config.width, config.height, config.title);
  app->config = config;

  app_load_resources(app);

  view_init(&app->_view);
  model_init(&app->_model);
}

void _app_update(App *app) {
  controller_update(&app->_model, &app->_view);
  view_update(&app->_view);
}

void _app_draw(App *app) {
  ClearBackground(BG);
  view_draw(&app->_view, app->fonts);
}

void app_close(App *app) {
  UnloadFont(app->fonts[0]);
  UnloadFont(app->fonts[1]);
  CloseWindow();
}

void app_run(App *app) {
  while (!WindowShouldClose()) {
    _app_update(app);
    BeginDrawing();
    _app_draw(app);
    EndDrawing();
  }
}
