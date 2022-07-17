#include "usb.h"
#include "xhci/xhci.h"
#include <common/kprint.h>
#include <driver/pci/pci.h>
#include <debug/bug.h>

#define MAX_USB_NUM 8 // pci总线上的usb设备的最大数量

// 在pci总线上寻找到的usb设备控制器的header
struct pci_device_structure_header_t *usb_pdevs[MAX_USB_NUM];
static int usb_pdevs_count = 0;

/**
 * @brief 初始化usb驱动程序
 *
 */
void usb_init()
{
    kinfo("Initializing usb driver...");
    // 获取所有usb-pci设备的列表
    pci_get_device_structure(USB_CLASS, USB_SUBCLASS, usb_pdevs, &usb_pdevs_count);

    if (WARN_ON(usb_pdevs_count == 0))
    {
        kwarn("There is no usb hardware in this computer!");
        return;
    }

    // 初始化每个usb控制器
    for (int i = 0; i < usb_pdevs_count; ++i)
    {
        switch (usb_pdevs[i]->ProgIF)
        {
        case USB_TYPE_UHCI:
        case USB_TYPE_OHCI:
        case USB_TYPE_EHCI:
        case USB_TYPE_UNSPEC:
        case USB_TYPE_DEVICE:
            kwarn("Unsupported usb host type: %#02x", usb_pdevs[i]->ProgIF);
            break;

        case USB_TYPE_XHCI:
            // 初始化对应的xhci控制器
            xhci_init(usb_pdevs[i]);
            break;

        default:
            kerror("Error value of usb_pdevs[%d]->ProgIF: %#02x", i, usb_pdevs[i]->ProgIF);
            return;
            break;
        }
    }
    kinfo("Successfully initialized all usb host controllers!");
}