SysTick - Ticks increment

IRQ interrupt priority:

	- Preemption Priority 0
		4 - CAN TX complete interrupt

	- Preemption Priority 1
		1 - USART receive interrupt
		4 - CAN RX interrupt
	
DMA Channel:

	- DMA 1
		Stream 3 - Channel 4 - USART3 TX
		Stream 4 - Channel 4 - USART4 TX
		Stream 6 - Channel 4 - USART2 TX
		Stream 7 - Channel 4 - UART5 TX
	- DMA 2
		Stream 7 - Channel 4 - USART1 TX
		
Timer:

	-Timer 6 (Basic timer)
		-Handling timer event