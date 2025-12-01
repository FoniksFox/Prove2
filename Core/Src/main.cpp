#include "main.h"
#include "ST-LIB.hpp"

int main(void) {
#ifdef SIM_ON
    SharedMemory::start();
#endif

    SCB_DisableDCache();

    // DigitalOutput led_on(PB0);
    STLIB::start();

    uint8_t data_buffer[256];
    auto mdma_channel = MDMA::inscribe(data_buffer);

    uint32_t source_data[256];
    uint32_t destination_data[256];
    uint32_t destination_data2[256];

    for (uint32_t i = 0; i < 256; ++i) {
        source_data[i] = i;
        destination_data[i] = 0;
        destination_data2[i] = 0;
    }

    auto source_node = MDMA::link_node_pool.construct(source_data, destination_data);
    auto dest_node = MDMA::link_node_pool.construct(destination_data, destination_data2);
    source_node->set_next(dest_node->get_node());

    auto promise = Promise::inscribe();
    MDMA::transfer_list(mdma_channel, source_node, promise);
    promise->then([](void*){
        while(1);
    });

    // Time::register_low_precision_alarm(100, [&]() { led_on.toggle(); 
    // });

    while (1) {
        STLIB::update();
    }
}

void Error_Handler(void) {
    ErrorHandler("HAL error handler triggered");
    while (1) {
    }
}
