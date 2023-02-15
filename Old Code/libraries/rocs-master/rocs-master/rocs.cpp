#include "rocs.hpp"

#include <Wire.h>

namespace rocs {

static uint8_t default_read_handler(uint8_t reg) {
  return 0;
}

static void default_write_handler(uint8_t reg, uint8_t val) {
  
}

static RegisterReadHandler read_handler = default_read_handler;
static RegisterWriteHandler write_handler = default_write_handler;

static uint8_t register_selector = 0;

static char* user_name;
static uint8_t user_name_len;
static char* name_ptr = nullptr;

static void i2c_receive_handler(int num_bytes) {
  while (num_bytes > 0) {
    register_selector = Wire.read();
    num_bytes--;

    if (num_bytes >= 1) {
      uint8_t val = Wire.read();
      num_bytes--;

      write_handler(register_selector, val);
    }
  }
}

static void i2c_request_handler() {
  if (register_selector == 0) {
    if (name_ptr == nullptr) {
      name_ptr = user_name;
      Wire.write(user_name_len);
    } else {
      uint8_t c = *name_ptr;
      name_ptr++;

      if (*name_ptr == 0) name_ptr = nullptr;

      Wire.write(c);
    }
    
    return;  
  }
  
  Wire.write(read_handler(register_selector));
}

void init(uint8_t slave_addr, const char* name, uint8_t name_len) {
  user_name = name;
  user_name_len = name_len;
  
  Wire.begin(slave_addr);

  Wire.onReceive(i2c_receive_handler);
  Wire.onRequest(i2c_request_handler);
}

void set_read_handler(RegisterReadHandler handler) {
  read_handler = handler;
}

void set_write_handler(RegisterWriteHandler handler) {
  write_handler = handler;
}

}
