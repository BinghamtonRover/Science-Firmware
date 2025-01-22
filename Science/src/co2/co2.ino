#include "Ezo_i2c.h"                          // Include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include "sequencer2.h"                      // Imports a 2-function sequencer. Controls the I2C read/write timing without blocking other operations    
#include "Ezo_i2c_util.h"                     // Brings in common print statements
#include "Adafruit_LiquidCrystal.h"           // Include the Adafruit I2C LCD backpack library

Ezo_board CO2 = Ezo_board(105, "CO2");        // Create a CO2 circuit object with address 105 and name "CO2"
Adafruit_LiquidCrystal CO2_lcd(0);            // Sets the I2C address of the LCD to 0 and names it "CO2_lcd"

float co2_concentration;                      // Declare a global variable to store the CO2 reading

void step1();                                 // Declare the functions used in the sequencer
void step2();                                 // Step 1 sends a read command, step 2 receives the reading

Sequencer2 Seq(&step1, 1000, &step2, 0);      // Calls the steps in sequence and sets the time between them

void setup() {
  Serial.begin(9600);                         // Enables the serial port and sets the baud rate to 9600
  Seq.reset();                                // Initialize the sequencer
  CO2_lcd.begin(16, 2);                       // Initialize the 16x2 LCD
  CO2_lcd.setBacklight(HIGH);                 // Turn on the backlight of the LCD
  CO2_lcd.setCursor(1, 0);                    // Set cursor to column 1, row 0
  CO2_lcd.print("Carbon Dioxide");            // Display "Carbon Dioxide" on line 1
  CO2_lcd.setCursor(10, 1);                   // Set cursor to column 10, row 1
  CO2_lcd.print("ppm");                       // Display "ppm" on the LCD
}

void loop() {
  Seq.run();                                  // Run the sequencer to get the CO2 readings
}

void step1() {
  CO2.send_read_cmd();                        // Send a read command using the Ezo_i2c library
}

void step2() {
  receive_and_print_reading(CO2);             // Get the reading from the CO2 sensor
  co2_concentration = CO2.get_last_received_reading();  // Store the reading in the variable
  Serial.print("CO2 Concentration: ");        // Print the reading for debugging
  Serial.println(co2_concentration);          // Show the reading on the serial monitor
  lcd_print();                                // Send the CO2 reading to the LCD
}

void lcd_print() {                            // Prints the CO2 reading to the LCD
  CO2_lcd.setCursor(3, 1);                    // Move the cursor to column 3, row 1
  CO2_lcd.print("     ");                     // Clear previous CO2 readings on the display
  CO2_lcd.setCursor(3, 1);                    // Move the cursor to column 3, row 1
  CO2_lcd.print(co2_concentration, 0);        // Display the CO2 reading on the LCD
}
