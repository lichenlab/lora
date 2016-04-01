#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x405fe887, "module_layout" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x3ec6c705, "kmalloc_caches" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xf9a482f9, "msleep" },
	{ 0x17a142df, "__copy_from_user" },
	{ 0x6980fe91, "param_get_int" },
	{ 0x98082893, "__copy_to_user" },
	{ 0x3b810932, "dev_set_drvdata" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xd5c45942, "device_destroy" },
	{ 0xb40be847, "__register_chrdev" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x62b72b0d, "mutex_unlock" },
	{ 0xff964b25, "param_set_int" },
	{ 0xf4cd17e0, "nonseekable_open" },
	{ 0xf6288e02, "__init_waitqueue_head" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x5f754e5a, "memset" },
	{ 0xde75b689, "set_irq_type" },
	{ 0x65d6d0f0, "gpio_direction_input" },
	{ 0xdc798d37, "__mutex_init" },
	{ 0xea147363, "printk" },
	{ 0xec6a4d04, "wait_for_completion_interruptible" },
	{ 0x7f80133c, "driver_unregister" },
	{ 0xf2ed731, "fasync_helper" },
	{ 0xa8f59416, "gpio_direction_output" },
	{ 0xe16b893b, "mutex_lock" },
	{ 0x564a6735, "device_create" },
	{ 0xfda85a7d, "request_threaded_irq" },
	{ 0x2196324, "__aeabi_idiv" },
	{ 0xb06dfa70, "spi_sync" },
	{ 0x6b1e3028, "kmem_cache_alloc" },
	{ 0x11f447ce, "__gpio_to_irq" },
	{ 0x108e8985, "param_get_uint" },
	{ 0xfe990052, "gpio_free" },
	{ 0x37a0cba, "kfree" },
	{ 0xa2e7208b, "spi_write_then_read" },
	{ 0x9d669763, "memcpy" },
	{ 0x3285cc48, "param_set_uint" },
	{ 0x42b26667, "class_destroy" },
	{ 0x60f71cfa, "complete" },
	{ 0xfa8e56ab, "spi_register_driver" },
	{ 0x582a74ba, "__class_create" },
	{ 0x122af8d0, "dev_get_drvdata" },
	{ 0xf20dabd8, "free_irq" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

