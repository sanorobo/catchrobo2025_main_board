#include <cstdio>

#include <halx/core.hpp>
#include <halx/peripheral.hpp>

#include <halx/driver/c6x0.hpp>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart6;

extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;

extern "C" void main_thread(void *) {
  using namespace halx::peripheral;
  using namespace halx::driver;

  Uart<&huart1> uart1;
  Uart<&huart2> uart2;
  Uart<&huart3> uart3;
  Uart<&huart4> uart4;
  Uart<&huart5> uart5;
  Uart<&huart6> uart6;

  Can<&hfdcan1> fdcan1;
  Can<&hfdcan2> fdcan2;

  enable_stdout(uart2);

  C6x0Manager c6x0_manager{fdcan1};
  C6x0 c6x0{c6x0_manager, C6x0Type::C610, C6x0Id::ID_1};

  fdcan1.start();

  while (true) {
    c6x0_manager.update();

    printf("%d\r\n", c6x0.get_rpm());

    c6x0.set_current_ref(400.0f);

    c6x0_manager.transmit();

    halx::core::delay(10);
  }
}
