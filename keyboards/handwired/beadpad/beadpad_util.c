

//utility ==================================================================================================================

uint8_t increment_wrap(uint8_t value, uint8_t count) {
    return (value + 1) % count;
}

uint8_t decrement_wrap(uint8_t value, uint8_t count) {
    uprintf("dec %u, %u", value, count);
    return (value <= 0) ? count - 1  : value--;
}

uint8_t increment_step(uint8_t value, uint8_t step, uint8_t max) {
     return (value + step) < max ? (value + step) : max;
}

uint8_t decrement_step(uint8_t value, uint8_t step) {
    return (value - step) <= 0 ? 0 : (value - step);
}

uint8_t increment(uint8_t value, uint8_t max) {
     return increment_step(value, 1, max);
}

uint8_t decrement(uint8_t value) {
    return decrement_step(value, 1);
}


