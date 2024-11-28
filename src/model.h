#pragma once

#define MAX_CLIENTS 100
#define MAX_EMPLOYEES 100

typedef struct {
  char *name;
  int id;
  int phone;
  char *email;
  char *direction;
} Client;

typedef struct {
  char *name;
} Employee;

typedef struct {
  Client db[MAX_CLIENTS];
  Employee employees[MAX_CLIENTS];
  int employees_count;
} Model_;

void model_init(Model_ *model);
void model_new_employee(Model_ *model, char *name);
