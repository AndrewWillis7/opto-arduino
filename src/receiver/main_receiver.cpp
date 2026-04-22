#include "../lib/Configuration.h"
#include "../lib/receiver_app.h"

using namespace Hardware;

App app;

void setup() {
  app.begin();
}

void loop() {
  app.update();
}