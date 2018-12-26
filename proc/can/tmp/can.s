	.cpu cortex-m4
	.eabi_attribute 27, 1
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 4
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"can.c"
	.text
	.section	.text.can_init,"ax",%progbits
	.align	1
	.global	can_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	can_init, %function
can_init:
	@ args = 0, pretend = 0, frame = 48
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, r5, r6, lr}
	ldr	r3, .L9
	ldrb	r0, [r3]	@ zero_extendqisi2
	sub	sp, sp, #48
	movs	r5, #0
	cmp	r0, #0
	beq	.L1
	strb	r5, [r3]
	ldr	r3, .L9+4
	ldr	r4, .L9+8
	strb	r5, [r3]
	ldr	r3, .L9+12
	ldr	r2, [r3, #88]
	orr	r2, r2, #33554432
	str	r2, [r3, #88]
	ldr	r2, [r3, #88]
	and	r2, r2, #33554432
	str	r2, [sp]
	ldr	r2, [sp]
	ldr	r2, [r3, #76]
	orr	r2, r2, #1
	str	r2, [r3, #76]
	ldr	r3, [r3, #76]
	and	r3, r3, #1
	str	r3, [sp, #4]
	mov	r2, #6144
	ldr	r3, [sp, #4]
	movs	r3, #18
	strd	r2, r3, [sp, #8]
	add	r1, sp, #8
	movs	r3, #9
	mov	r0, #1207959552
	str	r3, [sp, #24]
	strd	r5, r5, [sp, #16]
	bl	HAL_GPIO_Init
	mov	r2, r5
	movs	r1, #1
	movs	r0, #20
	bl	HAL_NVIC_SetPriority
	movs	r0, #20
	bl	HAL_NVIC_EnableIRQ
	ldr	r3, .L9+16
	str	r3, [r4]
	mov	r1, #196608
	mov	r3, #4194304
	strd	r1, r3, [r4, #16]
	ldr	r3, .L9+20
	ldr	r2, .L9+24
	ldr	r3, [r3]
	strb	r5, [r4, #24]
	movs	r6, #1
	mov	r0, r4
	udiv	r3, r3, r2
	strb	r5, [r4, #25]
	strb	r5, [r4, #26]
	strb	r6, [r4, #27]
	strb	r5, [r4, #28]
	strb	r5, [r4, #29]
	strd	r5, r5, [r4, #8]
	str	r3, [r4, #4]
	bl	HAL_CAN_Init
	cbz	r0, .L3
	ldr	r2, .L9+28
	ldr	r3, [r2]
	orr	r3, r3, #8388608
	orrs	r3, r3, r6
.L8:
	str	r3, [r2]
	bl	error_handle
	mov	r0, r6
.L1:
	add	sp, sp, #48
	@ sp needed
	pop	{r4, r5, r6, pc}
.L3:
	movs	r3, #5
	strd	r0, r0, [sp, #8]
	strd	r0, r0, [sp, #16]
	str	r0, [sp, #24]
	strd	r3, r0, [sp, #28]
	strd	r0, r6, [sp, #36]
	add	r1, sp, #8
	mov	r0, r4
	bl	HAL_CAN_ConfigFilter
	cbz	r0, .L4
	ldr	r2, .L9+28
	ldr	r3, [r2]
	orr	r3, r3, #8388608
	orr	r3, r3, #2
	b	.L8
.L4:
	mov	r0, r4
	bl	HAL_CAN_Start
	cbz	r0, .L5
	ldr	r2, .L9+28
	ldr	r3, [r2]
	orr	r3, r3, #8388608
	orr	r3, r3, #4
	b	.L8
.L5:
	movs	r1, #2
	mov	r0, r4
	bl	HAL_CAN_ActivateNotification
	cbz	r0, .L6
	ldr	r2, .L9+28
	ldr	r3, [r2]
	orr	r3, r3, #8388608
	orr	r3, r3, #8
	b	.L8
.L6:
	ldr	r3, .L9+32
	movw	r2, #938
	strd	r2, r0, [r3]
	movs	r2, #8
	strd	r0, r0, [r3, #8]
	str	r2, [r3, #16]
	strb	r0, [r3, #20]
	b	.L1
.L10:
	.align	2
.L9:
	.word	.LANCHOR0
	.word	.LANCHOR1
	.word	.LANCHOR2
	.word	1073876992
	.word	1073767424
	.word	SystemCoreClock
	.word	1000000
	.word	error
	.word	.LANCHOR3
	.size	can_init, .-can_init
	.section	.text.can_msg_write,"ax",%progbits
	.align	1
	.global	can_msg_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	can_msg_write, %function
can_msg_write:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L22
	ldrb	r3, [r3]	@ zero_extendqisi2
	push	{r0, r1, r4, lr}
	mov	r4, r0
	cbnz	r3, .L12
.L14:
	movs	r3, #0
.L13:
	ldrb	r2, [r4, r3]	@ zero_extendqisi2
	strb	r2, [sp, r3]
	adds	r3, r3, #1
	cmp	r3, #8
	bne	.L13
	ldr	r3, .L22+4
	ldr	r1, .L22+8
	ldr	r0, .L22+12
	mov	r2, sp
	bl	HAL_CAN_AddTxMessage
	cbz	r0, .L11
	ldr	r2, .L22+16
	ldr	r3, [r2]
	orr	r3, r3, #8388608
	orr	r3, r3, #16
	b	.L21
.L12:
	bl	can_init
	cmp	r0, #0
	beq	.L14
	ldr	r2, .L22+16
	ldr	r3, [r2]
	orr	r3, r3, #8388608
	orr	r3, r3, #64
.L21:
	str	r3, [r2]
	bl	error_handle
	movs	r0, #1
.L11:
	add	sp, sp, #8
	@ sp needed
	pop	{r4, pc}
.L23:
	.align	2
.L22:
	.word	.LANCHOR0
	.word	.LANCHOR4
	.word	.LANCHOR3
	.word	.LANCHOR2
	.word	error
	.size	can_msg_write, .-can_msg_write
	.section	.text.can_msg_read,"ax",%progbits
	.align	1
	.global	can_msg_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	can_msg_read, %function
can_msg_read:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	ldr	r3, .L37
	ldrb	r3, [r3]	@ zero_extendqisi2
	push	{r4, lr}
	mov	r4, r0
	cbz	r3, .L25
	bl	can_init
	cbz	r0, .L25
	ldr	r2, .L37+4
	ldr	r3, [r2]
	orr	r3, r3, #8388608
	orr	r3, r3, #64
	str	r3, [r2]
	bl	error_handle
.L36:
	movs	r0, #1
.L24:
	pop	{r4, pc}
.L25:
	ldr	r2, .L37+8
	ldrb	r1, [r2]	@ zero_extendqisi2
	cbnz	r1, .L28
	ldr	r2, .L37+4
	ldr	r3, [r2]
	orr	r3, r3, #8388608
	orr	r3, r3, #256
	str	r3, [r2]
	b	.L36
.L28:
	ldr	r0, .L37+12
	movs	r3, #0
.L27:
	ldrb	r1, [r3, r0]	@ zero_extendqisi2
	strb	r1, [r4, r3]
	adds	r3, r3, #1
	cmp	r3, #8
	bne	.L27
	movs	r0, #0
	strb	r0, [r2]
	b	.L24
.L38:
	.align	2
.L37:
	.word	.LANCHOR0
	.word	error
	.word	.LANCHOR1
	.word	.LANCHOR5
	.size	can_msg_read, .-can_msg_read
	.section	.text.CAN1_RX0_IRQHandler,"ax",%progbits
	.align	1
	.global	CAN1_RX0_IRQHandler
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	CAN1_RX0_IRQHandler, %function
CAN1_RX0_IRQHandler:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r0, .L40
	b	HAL_CAN_IRQHandler
.L41:
	.align	2
.L40:
	.word	.LANCHOR2
	.size	CAN1_RX0_IRQHandler, .-CAN1_RX0_IRQHandler
	.section	.text.HAL_CAN_RxFifo0MsgPendingCallback,"ax",%progbits
	.align	1
	.global	HAL_CAN_RxFifo0MsgPendingCallback
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	HAL_CAN_RxFifo0MsgPendingCallback, %function
HAL_CAN_RxFifo0MsgPendingCallback:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r3, lr}
	ldr	r2, .L46
	ldr	r3, .L46+4
	movs	r1, #0
	bl	HAL_CAN_GetRxMessage
	cbz	r0, .L43
	ldr	r2, .L46+8
	ldr	r3, [r2]
	orr	r3, r3, #8388608
	orr	r3, r3, #32
	str	r3, [r2]
	pop	{r3, lr}
	b	error_handle
.L43:
	ldr	r3, .L46+12
	ldrb	r2, [r3]	@ zero_extendqisi2
	cbz	r2, .L44
	ldr	r2, .L46+8
	ldr	r3, [r2]
	orr	r3, r3, #8388736
	str	r3, [r2]
.L42:
	pop	{r3, pc}
.L44:
	movs	r2, #1
	strb	r2, [r3]
	b	.L42
.L47:
	.align	2
.L46:
	.word	.LANCHOR6
	.word	.LANCHOR5
	.word	error
	.word	.LANCHOR1
	.size	HAL_CAN_RxFifo0MsgPendingCallback, .-HAL_CAN_RxFifo0MsgPendingCallback
	.section	.bss.can,"aw",%nobits
	.align	2
	.set	.LANCHOR2,. + 0
	.type	can, %object
	.size	can, 40
can:
	.space	40
	.section	.bss.can_msg_pending,"aw",%nobits
	.set	.LANCHOR1,. + 0
	.type	can_msg_pending, %object
	.size	can_msg_pending, 1
can_msg_pending:
	.space	1
	.section	.bss.can_rx_data,"aw",%nobits
	.set	.LANCHOR5,. + 0
	.type	can_rx_data, %object
	.size	can_rx_data, 8
can_rx_data:
	.space	8
	.section	.bss.can_rx_header,"aw",%nobits
	.align	2
	.set	.LANCHOR6,. + 0
	.type	can_rx_header, %object
	.size	can_rx_header, 28
can_rx_header:
	.space	28
	.section	.bss.can_tx_header,"aw",%nobits
	.align	2
	.set	.LANCHOR3,. + 0
	.type	can_tx_header, %object
	.size	can_tx_header, 24
can_tx_header:
	.space	24
	.section	.bss.can_tx_mailbox,"aw",%nobits
	.align	2
	.set	.LANCHOR4,. + 0
	.type	can_tx_mailbox, %object
	.size	can_tx_mailbox, 4
can_tx_mailbox:
	.space	4
	.section	.data.init_pending,"aw",%progbits
	.set	.LANCHOR0,. + 0
	.type	init_pending, %object
	.size	init_pending, 1
init_pending:
	.byte	1
	.ident	"GCC: (15:7-2018-q2-4) 7.3.1 20180622 (release) [ARM/embedded-7-branch revision 261907]"
