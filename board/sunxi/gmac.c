#include <netdev.h>
#include <miiphy.h>
#include <asm/io.h>
#include <asm/arch/clock.h>

void eth_init_board(void)
{
#ifdef CONFIG_MACH_SUN9I
	/*
	 * sun9i puts the GMAC transmit clock control in a register of its own
	 * in the AHB1 region rather than in the CCU. The value written is the
	 * same as everywhere else - only the address differs.
	 */
	void *gmac_clk_cfg = (void *)SUNXI_GMAC_TX_CLK_CFG;
#else
	struct sunxi_ccm_reg *const ccm =
		(struct sunxi_ccm_reg *)SUNXI_CCM_BASE;
	void *gmac_clk_cfg = &ccm->gmac_clk_cfg;
#endif

	/* Set MII clock */
#ifdef CONFIG_RGMII
	setbits_le32(gmac_clk_cfg, CCM_GMAC_CTRL_TX_CLK_SRC_INT_RGMII |
		CCM_GMAC_CTRL_GPIT_RGMII);
	setbits_le32(gmac_clk_cfg,
		     CCM_GMAC_CTRL_TX_CLK_DELAY(CONFIG_GMAC_TX_DELAY));
#else
	setbits_le32(gmac_clk_cfg, CCM_GMAC_CTRL_TX_CLK_SRC_MII |
		CCM_GMAC_CTRL_GPIT_MII);
#endif
}
