#include "bsp.h"
#include "plic_driver.h"

#if BSP_USE_UART0 || BSP_USE_UART1
#include "uart.h"
#endif

#if BSP_USE_IIC0
#include "iic.h"
#endif

#if BSP_USE_GPIO
#include "gpio.h"
#endif

#if BSP_USE_SPI1
#include "spi.h"
#endif

#if BSP_USE_ICEBLK
#include "iceblk.h"
#endif

// to communicate with the debugger in spike
volatile uint64_t tohost __attribute__((aligned(64)));
volatile uint64_t fromhost __attribute__((aligned(64)));

plic_instance_t Plic;

/**
 *  Prepare haredware to run the demo.
 */
void prvSetupHardware(void)
{
    // Resets PLIC, threshold 0, nothing enabled
    PLIC_init(&Plic, PLIC_BASE_ADDR, PLIC_NUM_SOURCES, PLIC_NUM_PRIORITIES);

// Set priorities & initialize peripherals
#if BSP_USE_UART0
    PLIC_set_priority(&Plic, PLIC_SOURCE_UART0, PLIC_PRIORITY_UART0);
    uart0_init();
#endif

#if BSP_USE_UART1
    PLIC_set_priority(&Plic, PLIC_SOURCE_UART1, PLIC_PRIORITY_UART1);
    uart1_init();
#endif

#if BSP_USE_ETHERNET
    configASSERT(BSP_USE_DMA);
    PLIC_set_priority(&Plic, PLIC_SOURCE_ETH, PLIC_PRIORITY_ETH);
    PLIC_set_priority(&Plic, PLIC_SOURCE_DMA_MM2S, PLIC_PRIORITY_DMA_MM2S);
    PLIC_set_priority(&Plic, PLIC_SOURCE_DMA_S2MM, PLIC_PRIORITY_DMA_S2MM);
#endif

#if BSP_USE_ICENET
    PLIC_set_priority(&Plic, PLIC_SOURCE_ICEETH_RX, PLIC_PRIORITY_ETH);
    PLIC_set_priority(&Plic, PLIC_SOURCE_ICEETH_TX, PLIC_PRIORITY_ETH);
#endif

#if BSP_USE_ICEBLK
    PLIC_set_priority(&Plic, PLIC_SOURCE_ICEBLK, PLIC_PRIORITY_ICEBLK);
    iceblk_init();
#endif

#if BSP_USE_IIC0
    PLIC_set_priority(&Plic, PLIC_SOURCE_IIC0, PLIC_PRIORITY_IIC0);
    iic0_init();
#endif

#if BSP_USE_SPI0
#error "BSP_USE_SPI0 should never be set! The onboard flash already uses this device"
#endif

#if BSP_USE_SPI1
    PLIC_set_priority(&Plic, PLIC_SOURCE_SPI1, PLIC_PRIORITY_SPI1);
    spi1_init();
#endif

#if BSP_USE_GPIO
    gpio_init();
#endif
}

/**
 * Define an external interrupt handler
 * cause = 0x8000000b == Machine external interrupt
 */
void external_interrupt_handler(HANDLER_DATATYPE cause)
{
    configASSERT(cause == MCAUSE_EXTERNAL_INTERRUPT);

    plic_source source_id = PLIC_claim_interrupt(&Plic);

    if ((source_id >= 1) && (source_id < PLIC_NUM_INTERRUPTS))
    {
        Plic.HandlerTable[source_id].Handler(Plic.HandlerTable[source_id].CallBackRef);
    }

    // clear interrupt
    PLIC_complete_interrupt(&Plic, source_id);
}

#ifdef BIN_SOURCE_LMCO
    /**
     * Prints mcause, mepc, and mstatus register values
     * and requests other tasks to shut down
     */
    #include <stdio.h>

    #ifndef NETBOOT
        /* From besspinMisc.c */
        extern void exitBesspin (uint8_t exitCode);
    #endif

    void exception_handler(HANDLER_DATATYPE mcause, HANDLER_DATATYPE mepc, HANDLER_DATATYPE mstatus)
    {
        printf("LMCO SSITH CPU Pipeline exception\n");
        printf("mcause: 0x%lX\n", (long unsigned int) mcause);
        printf("mepc: 0x%lX\n", (long unsigned int) mepc);
        printf("mstatus: 0x%lX\n", (long unsigned int) mstatus);

        #ifndef NETBOOT
            /* Signal other tasks to terminate cleanly */
            exitBesspin(1);
        #endif

        for (;;) ;;
    }
#endif /* BIN_SOURCE_LMCO */
