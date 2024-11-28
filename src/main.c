#include "application.h"
#include <raylib.h>
#include <stdio.h>

static const AppConfig config = {
    .width = 1366,
    .height = 710,
    .title = "Agenda",
};

int main(int argc, char **argv) {
  App app;

  app_init(&app, config);
  app_run(&app);
  app_close(&app);
  return 0;
}
