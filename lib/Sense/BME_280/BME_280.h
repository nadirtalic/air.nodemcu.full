#ifndef BME_280_H
#define BME_280_H

/***************************************************************************
*  BME280 humidity, temperature & pressure sensor
*  These sensors use I2C address either 0x76 or 0x77.
***************************************************************************/

#include "CityOS.h"

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

const float SEA_LEVEL_PRESSURE_HPA = 1013.25;
const int DELAY         = 2000;
const int STARTUP_DELAY = 500;

class BME_280 : public CityOS {
public:

    BME_280(int scl, int sda);
    BME_280(int scl, int sda, int i2c);
    ~BME_280();

    struct _in {
        // BME_280 Sensor
        float temperature;
        float humidity;
        float temperature_feel;
        float pressure;
        float altitude;
    } in;

    void interval();
    // FROM DHT
    float convertCtoF(float c);
    float convertFtoC(float f);
    float computeHeatIndex(float temperature, float percentHumidity, bool isFahrenheit);

private:
    Adafruit_BME280 * _bme;
    TwoWire * _wire;
    void setup(int scl, int sda, int i2c);
};

#endif /* ifndef BME_280_H */
