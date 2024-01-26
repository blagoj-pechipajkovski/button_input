# Button

## Description

An Arduino library for reading a button with functions for: on click, on hold, and on release.

## Usage

### Instantiation

Include library and create an instance.

```c++
#include <button_input.h>
ButtonInput btn;
```

### Initialization

Specify pin, active state, and input type. Active state means the state when the button is pressed.

```c++
btn.begin(27, LOW, INPUT_PULLUP);
```

### Handlers

There are 3 handler function that can be specified, check out *Opertaion* bellow for details.

```c++
// Specify handlers as lambdas

btn.on_press([]() {
    Serial.println("Press");
});

btn.on_hold([]() {
    Serial.println("Hold");
});

btn.on_release([]() {
    Serial.println("Release");
});
```

In the case that the hold function should be the same as the press function, that can be done with the following code:

```c++
btn.set_on_press_as_hold();
```

*Just make sure that the press handler is already specified before calling this function*

### Loop

Make sure to call the loop function inside loop and make sure that your loop has no blocking code like ```delay()```.

```
ButtonInput::loop_all();
```

## Operation

This library has software de-bouncing, the state of the pin is read at max every 5ms. That sould be faster than someone could lift thier finger, but still fast enough so that the delay is not noticable, and slow enough for the state of the pin to stabilise.

On the initial press of the button the ```on_press``` handler is called. After executing there is an initial delay before starting to execute the hold function. The initial delay between ```on_press``` and ```on_hold``` is 1000ms, after which the ```on_hold``` handler is called for the first time. After the first execution, the ```on_hold``` handler is repeatedly called every 500ms. Lastly when the user releases the button the ```on_release``` handler is called.

```
               [hold_wait]                [hold_interval]           [hold_interval]
    | ----------------------------- | ----------------------- | ----------------------- | -------------- | 
[on_press]                      [on_hold]                 [on_hold]                 [on_hold]      [on_release]
```

## Custom timing

The values for both hold_wait and hold_interval can be changed. The default for all buttons is:

```c++
    btn.hold_wait = 1000;
    btn.hold_interval = 500;
```
