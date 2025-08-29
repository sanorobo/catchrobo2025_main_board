#include <cstdio>

#include <halx/core.hpp>
#include <halx/driver/c6x0.hpp>
#include <halx/peripheral.hpp>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart6;

extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;

__attribute__((section(".ram_d2"))) static uint8_t ram_d2[8192];

extern "C" void main_thread(void *) {
  using namespace halx::peripheral;
  using namespace halx::driver;

  huart1.Init.BaudRate = 115200;
  huart2.Init.BaudRate = 115200;
  huart3.Init.BaudRate = 115200;
  huart4.Init.BaudRate = 115200;
  huart5.Init.BaudRate = 115200;
  huart6.Init.BaudRate = 115200;

  std::span ram_d2_span{ram_d2};
  Uart<&huart1, UartTxDma, UartRxDma> uart1{ram_d2_span.subspan<512 * 0, 512>(), ram_d2_span.subspan<512 * 1, 512>()};
  Uart<&huart2, UartTxDma, UartRxDma> uart2{ram_d2_span.subspan<512 * 2, 512>(), ram_d2_span.subspan<512 * 3, 512>()};
  Uart<&huart3, UartTxDma, UartRxDma> uart3{ram_d2_span.subspan<512 * 4, 512>(), ram_d2_span.subspan<512 * 5, 512>()};
  Uart<&huart4, UartTxDma, UartRxDma> uart4{ram_d2_span.subspan<512 * 6, 512>(), ram_d2_span.subspan<512 * 7, 512>()};
  Uart<&huart5, UartTxDma, UartRxDma> uart5{ram_d2_span.subspan<512 * 8, 512>(), ram_d2_span.subspan<512 * 9, 512>()};
  Uart<&huart6, UartTxDma, UartRxDma> uart6{ram_d2_span.subspan<512 * 10, 512>(), ram_d2_span.subspan<512 * 11, 512>()};

  Can<&hfdcan1> can1;
  Can<&hfdcan2> can2;

  enable_stdout(uart2);

  // これより上はbaud rate以外触らないほうがいいと思う

  C6x0Manager c6x0_manager{can1};
  C6x0 c6x0{c6x0_manager, C6x0Type::C610, C6x0Id::ID_1};

  can1.start();

  while (true) {
    c6x0_manager.update();

    printf("%d\r\n", c6x0.get_rpm());

    c6x0.set_current_ref(400.0f);

    c6x0_manager.transmit();

    halx::core::delay(10);
  }
}
