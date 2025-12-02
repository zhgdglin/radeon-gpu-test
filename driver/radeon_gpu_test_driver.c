// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/delay.h>

#define GPU_MMIO_BASE 0xA0000000 // Example MMIO base address
#define MMIO_REGISTER_OFFSET 0x100 // Example offset for a specific MMIO register

static void __iomem *mmio_base;

static int __init radeon_gpu_test_init(void) {
    // Map the MMIO region
    mmio_base = ioremap(GPU_MMIO_BASE, PAGE_SIZE);
    if (!mmio_base) {
        pr_err("Failed to map MMIO region\n");
        return -ENOMEM;
    }

    // Perform memory tests
    uint32_t value;
    for (int i = 0; i < 10; i++) {
        value = readl(mmio_base + MMIO_REGISTER_OFFSET);
        pr_info("MMIO register value: %x\n", value);
        writel(value ^ 0xFFFFFFFF, mmio_base + MMIO_REGISTER_OFFSET);
        udelay(10);
    }

    return 0;
}

static void __exit radeon_gpu_test_exit(void) {
    // Unmap the MMIO region
    if (mmio_base) {
        iounmap(mmio_base);
    }
    pr_info("Radeon GPU Test Module unloaded\n");
}

module_init(radeon_gpu_test_init);
module_exit(radeon_gpu_test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zhgdglin");
MODULE_DESCRIPTION("AMD Radeon GPU Memory Testing via MMIO");
