#include <WS_2812_Ring.h>

WS_2812_Ring::WS_2812_Ring(int pin, int count) : ring(count, pin)
{
    control("led ring - 6 levels");
    
    listen = "AIR_AQI_RANGE";    

    addToLoop(this);
    addToSetup(this);
    
};

void WS_2812_Ring::setup(){    
    ring.begin();
    ring.setBrightness(255);
    ring.Interval = 100;
    // Loop rainbow until first reading
    ring.RainbowCycle(3);
    ring.RainbowCycleUpdate();   
}

void WS_2812_Ring::loop()
{
    static int lastUpdate = 0;

    if ((millis() - lastUpdate) < 100) {
        yield();
        return;
    }

    lastUpdate = millis();

    static int last_range     = -1;
    static uint32_t old_color = ring.Color(255,255,255);

    if (!listen.length()) {
        if (debug) {
            Serial
                << "LED RING | Listening not set " << endl
                << "LED RING | Use ring.listen = \"range name\" to set it." << endl;
        }
        return;
    }

    if (debug) {
        Serial
            << "LED RING | Listening set to: " << listen << endl;
    }

    int current_range = (int) senseValues[listen].value;
    int wifi = (int)controlValues["wifi"];

    if (wifi==0)
    {
        ring.Interval = 0;
        // ring.RainbowCycle(3, FORWARD);       
        ring.Scanner(ring.Color(255,0,0), 55);

        int start = millis();
        while((millis()-start) < 5000){
           yield();            
           ring.Update(); 
        }
    }
    
    if (last_range != current_range) {
        int colors[6][3] = {
            {    1, 200, 255 }, // blue
            {   60, 255,   1 }, // green
            {  230, 120,   1 }, // yellow
            {  255,  40,   1 }, // orange
            {  255,   1, 170 }, // purple
            {  255,   1,   1 } // red
        };

        if (current_range >= (int)sizeof(colors))
            current_range = sizeof(colors) - 1;

        uint32_t new_color = ring.Color(colors[current_range][0], colors[current_range][1], colors[current_range][2]);

        int steps = 255;
        for (int i = 0; i < steps; i++) {
            uint8_t red   = ((ring.Red(old_color) * (steps - i)) + (ring.Red(new_color) * i)) / steps;
            uint8_t green = ((ring.Green(old_color) * (steps - i)) + (ring.Green(new_color) * i)) / steps;
            uint8_t blue  = ((ring.Blue(old_color) * (steps - i)) + (ring.Blue(new_color) * i)) / steps;

            ring.ColorSet(ring.Color(red, green, blue));
            ring.show();
            delay(2);
        }
        old_color = new_color;
    }
} // WS_2812_Ring::loop
