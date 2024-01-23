#include <button_input.h>

ButtonInput btn;

void setup() {
    Serial.begin(115200);

    btn.begin(27, LOW, INPUT_PULLUP);
    
    btn.on_press([]() {
        Serial.println("Press");
    });
    
    btn.on_hold([]() {
        Serial.println("Hold");
    });
    
    btn.on_release([]() {
        Serial.println("Release");
    });
    
    // btn.hold_wait = 1000; // wait between press and start of on_hold function // 1000 default
    // btn.hold_interval = 500; // interval after first on_hold // 500 default
}

void loop() {
    ButtonInput::loop_all();
}