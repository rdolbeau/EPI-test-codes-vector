#ifndef VEHAVE_CONTROL_H
#define VEHAVE_CONTROL_H

#define VEHAVE_CSR_DEBUG_NUM "3106"
#define VEHAVE_CSR_DEBUG(reg)                                                  \
  do {                                                                         \
    __asm__ __volatile__("csrr " #reg ", " VEHAVE_CSR_DEBUG_NUM : : :);        \
  } while (0)

#define vehave_disable_debug() VEHAVE_CSR_DEBUG(x0)
#define vehave_enable_debug() VEHAVE_CSR_DEBUG(x1)

#define vehave_disable_tracing() VEHAVE_CSR_DEBUG(x2)
#define vehave_enable_tracing() VEHAVE_CSR_DEBUG(x3)

#define vehave_start_mark() VEHAVE_CSR_DEBUG(x4)
#define vehave_stop_mark() VEHAVE_CSR_DEBUG(x5)

#endif
