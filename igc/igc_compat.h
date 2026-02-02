/* SPDX-License-Identifier: GPL-2.0 */
/* Kernel compatibility header for igc driver */

#ifndef _IGC_COMPAT_H_
#define _IGC_COMPAT_H_

#include <linux/version.h>

/*
 * Kernel version compatibility macros for igc driver
 * Supports Linux 3.10+ kernels
 */

/* skb_frag_t / skb_frag_struct compatibility
 * In older kernels, skb_frag_t is typedef of struct skb_frag_struct
 * In newer kernels (5.4+), skb_frag_t is typedef of struct bio_vec
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0)
/* Use skb_frag_t directly, it's compatible with skb_frag_struct */
#define igc_skb_frag_t skb_frag_t
#else
/* In newer kernels, just use skb_frag_t */
#define igc_skb_frag_t skb_frag_t
#endif

/* netdev_xmit_more / skb->xmit_more compatibility
 * skb->xmit_more was removed in kernel 5.2, replaced with netdev_xmit_more()
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 2, 0)
#define igc_xmit_more(skb) ((skb)->xmit_more)
#else
#define igc_xmit_more(skb) netdev_xmit_more()
#endif

/* eth_get_headlen compatibility
 * Added struct net_device *dev parameter in kernel 5.0
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)
#define igc_eth_get_headlen(netdev, data, len) eth_get_headlen(data, len)
#else
#define igc_eth_get_headlen(netdev, data, len) eth_get_headlen(netdev, data, len)
#endif

/* netif_napi_add compatibility
 * weight parameter removed in kernel 6.1
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 1, 0)
#define igc_netif_napi_add(dev, napi, poll, weight) \
    netif_napi_add(dev, napi, poll, weight)
#else
#define igc_netif_napi_add(dev, napi, poll, weight) \
    netif_napi_add(dev, napi, poll)
#endif

/* pci_enable_pcie_error_reporting compatibility
 * Removed in kernel 6.6
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 6, 0)
#define igc_pci_enable_pcie_error_reporting(pdev) \
    pci_enable_pcie_error_reporting(pdev)
#define igc_pci_disable_pcie_error_reporting(pdev) \
    pci_disable_pcie_error_reporting(pdev)
#else
#define igc_pci_enable_pcie_error_reporting(pdev) do { } while (0)
#define igc_pci_disable_pcie_error_reporting(pdev) do { } while (0)
#endif

/* dev_addr compatibility
 * netdev->dev_addr became const in kernel 5.17
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 17, 0)
#define igc_set_dev_addr(netdev, addr) \
    memcpy((netdev)->dev_addr, (addr), (netdev)->addr_len)
#else
#define igc_set_dev_addr(netdev, addr) \
    eth_hw_addr_set(netdev, addr)
#endif

/* netif_trans_update / trans_start compatibility
 * netif_trans_update added in kernel 4.7, trans_start deprecated
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 7, 0)
#define igc_netif_trans_update(netdev) \
    ((netdev)->trans_start = jiffies)
#else
#define igc_netif_trans_update(netdev) \
    netif_trans_update(netdev)
#endif

/* timer_setup / setup_timer compatibility
 * timer_setup added in kernel 4.15
 * For old kernels, we use setup_timer with unsigned long callback
 * For new kernels, we use timer_setup with from_timer macro
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 15, 0)
#define IGC_USE_LEGACY_TIMER 1
/* from_timer compatibility for older kernels */
#define from_timer(var, callback_timer, timer_fieldname) \
    container_of(callback_timer, typeof(*var), timer_fieldname)
#else
#define IGC_USE_LEGACY_TIMER 0
#endif

/* napi_complete_done / napi_complete compatibility
 * napi_complete_done returns bool since kernel 4.10
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 10, 0)
#define igc_napi_complete_done(napi, work_done) napi_complete(napi)
#else
#define igc_napi_complete_done(napi, work_done) napi_complete_done(napi, work_done)
#endif

/* mmiowb compatibility
 * mmiowb removed in kernel 5.2 (now implicit in spin_unlock)
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 2, 0)
#define mmiowb() do { } while (0)
#endif

/* dma_map_page_attrs / dma_map_page compatibility */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 8, 0)
#define dma_map_page_attrs(dev, page, offset, size, dir, attrs) \
    dma_map_page(dev, page, offset, size, dir)
#define dma_unmap_page_attrs(dev, addr, size, dir, attrs) \
    dma_unmap_page(dev, addr, size, dir)
#endif

/* page_frag_cache_drain compatibility */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 10, 0)
#define __page_frag_cache_drain(page, count) __free_page(page)
#endif

#endif /* _IGC_COMPAT_H_ */
