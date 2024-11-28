#include "model.h"
#include <stdio.h>

void model_init(Model_ *model) {
  model->employees_count = 0;
  model_new_employee(model, "Juan");
  model_new_employee(model, "Carlos");
}

void model_new_employee(Model_ *model, char *name) {
  if (model->employees_count > MAX_EMPLOYEES) {
    printf("Reached maximum number of employees");
    return;
  }
  model->employees[model->employees_count] = (Employee){name};
  model->employees_count++;
}
