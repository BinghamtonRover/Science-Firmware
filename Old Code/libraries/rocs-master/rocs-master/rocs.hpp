#include <stdint.h>

namespace rocs {

typedef uint8_t (*RegisterReadHandler)(uint8_t reg);
typedef void (*RegisterWriteHandler)(uint8_t reg, uint8_t val);

void init(uint8_t slave_addr, const char* name, uint8_t name_len);
void set_read_handler(RegisterReadHandler handler);
void set_write_handler(RegisterWriteHandler handler);
  
}
