#ifndef BUTTON_INPUT
#define BUTTON_INPUT

#define BUTTON_INPUT_NUM 16
class ButtonInput {
  private:
    int _pin;
    bool _active_state;
    
    bool _last_state = false;
    
    unsigned long _last_loop = 0;
    
    unsigned long _last_tranzition = 0;
    unsigned long _last_hold_fn_call = 0;
    
    void (*_on_press_fn)();
    void (*_on_release_fn)();
    void (*_on_hold_fn)();
    
    static int _buttons_num;
    static ButtonInput* _buttons[BUTTON_INPUT_NUM];
    
  public:
    
    void on_press(void (*new_on_press)()) {
        _on_press_fn = new_on_press;
    }
    void on_release(void (*new_on_release)()) {
        _on_release_fn = new_on_release;
    }
    void on_hold(void (*new_on_hold)()) {
        _on_hold_fn = new_on_hold;
    }
    
    void set_on_press_as_hold() {
        _on_hold_fn = _on_press_fn;
    }
    
    void begin(int pin, bool active_state, int input_type) {
        _pin = pin;
        _active_state = active_state;
        
        pinMode(_pin, input_type);
        
        if (ButtonInput::_buttons_num < BUTTON_INPUT_NUM) {
            ButtonInput::_buttons[ButtonInput::_buttons_num] = this;
            ButtonInput::_buttons_num++;
        }
    }
    
    unsigned long hold_wait = 1000;
    unsigned long hold_interval = 500;
    
    void loop() {
        if (millis() < _last_loop + 5) return;
        _last_loop = millis();
        
        bool state = (digitalRead(_pin) == _active_state);
        
        if (state != _last_state) {
            _last_tranzition = millis();
            
            if (state && _on_press_fn != nullptr)
                _on_press_fn();
            
            if (!state && _on_release_fn != nullptr)
                _on_release_fn();
        }
        
        if (state) {
            if (millis() > _last_tranzition + hold_wait) {
                if (millis() > _last_hold_fn_call + hold_interval) {
                    if (_on_hold_fn != nullptr)
                        _on_hold_fn();
                    _last_hold_fn_call = millis();
                }
            }
        }
        
        _last_state = state;
    }
    
    static void loop_all() {
        for (int i=0; i < ButtonInput::_buttons_num; i++) {
            ButtonInput::_buttons[i]->loop();
        }
    }
    
    bool last_state() {
        return _last_state;
    }
    
    unsigned long last_tranzition() {
        return _last_tranzition;
    }
    
    static unsigned long most_recent_tranzition() {
        unsigned long mrt = 0;
        for (int i=0; i < ButtonInput::_buttons_num; i++) {
            if (ButtonInput::_buttons[i]->last_tranzition() > mrt)
                mrt = ButtonInput::_buttons[i]->last_tranzition();
        }
        return mrt;
    }
    
    unsigned long last_event() {
        if (_last_hold_fn_call > _last_tranzition)
            return _last_hold_fn_call;
        else
            return _last_tranzition;
    }
    
    static unsigned long most_recent_event() {
        unsigned long mre = 0;
        for (int i=0; i < ButtonInput::_buttons_num; i++) {
            if (ButtonInput::_buttons[i]->last_event() > mre)
                mre = ButtonInput::_buttons[i]->last_event();
        }
        return mre;
    }
};
int ButtonInput::_buttons_num=0;
ButtonInput* ButtonInput::_buttons[BUTTON_INPUT_NUM];

#endif