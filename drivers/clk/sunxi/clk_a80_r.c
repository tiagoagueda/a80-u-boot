// SPDX-License-Identifier: (GPL-2.0+ OR MIT)
/*
 * Clock gates and resets for the sun9i (A80) CPUS domain.
 *
 * These are not a CCU.  sun9i describes this domain with the older sunxi
 * bindings, where each node owns a single 32-bit PRCM register and index N
 * is bit N:
 *
 *	apbs_gates  0x08001428  "allwinner,sun9i-a80-apbs-gates-clk"
 *	apbs_rst    0x080014b0  "allwinner,sun6i-a31-clock-reset"
 *
 * The RSB is index 3 in both.  It is the only consumer U-Boot has here, but
 * a provider has to exist for it: sun8i_rsb_probe() treats its clock and
 * reset lookups as best-effort and carries on to sun8i_rsb_init() either
 * way, so with no driver it writes to a block that is still gated and held
 * in reset, and the AHB access hangs the SoC.
 */

#include <clk-uclass.h>
#include <dm.h>
#include <clk/sunxi.h>
#include <linux/bitops.h>

/* Indices and names from the apbs_gates node's clock-indices/-output-names. */
static const struct ccu_clk_gate a80_apbs_gates[] = {
	[0]	= GATE(0x0, BIT(0)),	/* apbs_pio */
	[1]	= GATE(0x0, BIT(1)),	/* apbs_ir */
	[2]	= GATE(0x0, BIT(2)),	/* apbs_timer */
	[3]	= GATE(0x0, BIT(3)),	/* apbs_rsb */
	[4]	= GATE(0x0, BIT(4)),	/* apbs_uart */
	[5]	= GATE(0x0, BIT(5)),	/* apbs_1wire */
	[6]	= GATE(0x0, BIT(6)),	/* apbs_i2c0 */
	[7]	= GATE(0x0, BIT(7)),	/* apbs_i2c1 */
	[12]	= GATE(0x0, BIT(12)),	/* apbs_ps2_0 */
	[13]	= GATE(0x0, BIT(13)),	/* apbs_ps2_1 */
	[16]	= GATE(0x0, BIT(16)),	/* apbs_dma */
	[17]	= GATE(0x0, BIT(17)),	/* apbs_i2s0 */
	[18]	= GATE(0x0, BIT(18)),	/* apbs_i2s1 */
	[20]	= GATE(0x0, BIT(20)),	/* apbs_twd */
};

const struct ccu_desc a80_apbs_gates_desc = {
	.gates = a80_apbs_gates,
	.num_gates = ARRAY_SIZE(a80_apbs_gates),
};

/*
 * The reset register mirrors the gate register's bit assignment.  Only the
 * resettable peripherals appear; pio, dma and twd have no reset bit.
 */
static const struct ccu_reset a80_apbs_resets[] = {
	[1]	= RESET(0x0, BIT(1)),	/* ir */
	[2]	= RESET(0x0, BIT(2)),	/* timer */
	[3]	= RESET(0x0, BIT(3)),	/* rsb */
	[4]	= RESET(0x0, BIT(4)),	/* uart */
	[5]	= RESET(0x0, BIT(5)),	/* 1wire */
	[6]	= RESET(0x0, BIT(6)),	/* i2c0 */
	[7]	= RESET(0x0, BIT(7)),	/* i2c1 */
};

const struct ccu_desc a80_apbs_rst_desc = {
	.resets = a80_apbs_resets,
	.num_resets = ARRAY_SIZE(a80_apbs_resets),
};
