#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xb3753869, "module_layout" },
	{ 0xc38c6801, "device_destroy" },
	{ 0xb356c301, "class_destroy" },
	{ 0xc79de84d, "device_create" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xceeb51d6, "__free_pages" },
	{ 0x8d62ea07, "__class_create" },
	{ 0xce2398b8, "alloc_pages_current" },
	{ 0xe6278ff5, "__register_chrdev" },
	{ 0x5f924ddd, "kill_fasync" },
	{ 0x3673a548, "remap_pfn_range" },
	{ 0x7cd8d75e, "page_offset_base" },
	{ 0x97651e6c, "vmemmap_base" },
	{ 0x30126750, "fasync_helper" },
	{ 0xc5850110, "printk" },
	{ 0x1b44c663, "current_task" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "6335129D9CF9BD32FE25A43");
