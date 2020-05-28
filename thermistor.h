//Thermistor
//Description: Thermistors are a variable resistor, that when hooked up in a voltage divider configuration, we can use the analog pin to read the voltage and convert that into a tempurature.
//analog: The actual read value from the ADC
//inCelcius: If you want the returned units to be C/F
//analogBits: The number of bits in the ADC
//resistor: The resistance in ohms of the other resistor in the divider circuit
double thermistor_analogToTemp(int analog, bool inCelcius = true, int analogBits = 12, int resistor = 100000);
