#pragma once
#include "../defs.h"
#include "grid.h"
#include "raylib.h"

typedef struct {
  Rectangle container;
  Grid sidebar;
  Grid calendar;
  int num_employes;
  bool btn_next_week;
  bool btn_prev_week;
  int current_week;
  int current_month_days;
  int start_day;
} Schedule;

Schedule create_schedule(Vector2 offset);

void draw_schedule(Schedule *schedule, Font font[NUM_FONTS]);

void schedule_add_employee(Schedule *schedule, Employee employee);

void update_schedule(Schedule *schedule);

void advance_week(Schedule *schedule);
void previous_week(Schedule *schedule);
void update_calendar_view(Schedule *schedule);
