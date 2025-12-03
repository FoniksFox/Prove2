#include "main.h"
#include "ST-LIB.hpp"
#include "HALAL/Models/Packets/MdmaPacket.hpp"


int main(void) {
#ifdef SIM_ON
    SharedMemory::start();
#endif

    

    // DigitalOutput led_on(PB0);
    STLIB::start();
    SCB_DisableDCache();

    uint8_t data_buffer[256];
    [[maybe_unused]] auto mdma_channel = MDMA::inscribe(data_buffer);

    // uint32_t source_data[256];
    // uint32_t destination_data[256];
    // uint32_t destination_data2[256];
    // for (uint32_t i = 0; i < 256; ++i) {
    //     source_data[i] = i;
    //     destination_data[i] = 0;
    //     destination_data2[i] = 0;
    // }
    // auto source_node = MDMA::link_node_pool.construct(source_data, destination_data);
    // auto dest_node = MDMA::link_node_pool.construct(destination_data, destination_data2);
    // source_node->set_next(dest_node->get_node());
    // auto promise = Promise::inscribe();
    // promise->then([](void*){
    //     while(1);
    // });
    // MDMA::transfer_list(mdma_channel, source_node, promise);

    uint32_t source_data1 = 0x12345678, source_data2 = 0x9ABCDEF0;
    uint32_t destination_data1[2] = {0, 0};
    auto packet = MdmaPacket(3, &source_data1, &source_data2);
    auto promise = Promise::inscribe();
    packet.build(promise, reinterpret_cast<uint8_t*>(destination_data1));
    promise->then([](void*){
        while(1);
    });

    packet.parse(promise, reinterpret_cast<uint8_t*>(destination_data1));




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
