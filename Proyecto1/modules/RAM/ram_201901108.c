#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/seq_file.h>
#include <linux/mm.h>
#include <linux/sysinfo.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Módulo para leer información del uso de la RAM");
MODULE_AUTHOR("Daniel Jiménez");
MODULE_VERSION("1.0");

static int escribir_archivo(struct seq_file *archivo, void *v)
{
    struct sysinfo info;
    long total_ram, free_ram, used_ram, percentage_used;
    si_meminfo(&info);
    total_ram = (info.totalram * info.mem_unit) / (1024 * 1024);
    free_ram = (info.freeram * info.mem_unit) / (1024 * 1024);
    used_ram = total_ram - free_ram;
    percentage_used = (used_ram * 100) / total_ram;
    seq_printf(archivo, "{\n");
    seq_printf(archivo, "\"total_ram\": %ld,\n", total_ram);
    seq_printf(archivo, "\"free_ram\": %ld,\n", free_ram);
    seq_printf(archivo, "\"used_ram\": %ld,\n", used_ram);
    seq_printf(archivo, "\"percentage_used\": %ld\n", percentage_used);
    seq_printf(archivo, "}\n");
    return 0;
}

static int al_abrir(struct inode *inode, struct file *file)
{
    return single_open(file, escribir_archivo, NULL);
}

static struct proc_ops operaciones = {
    .proc_open = al_abrir,
    .proc_read = seq_read};

static int _insert(void)
{
    proc_create("ram", 0, NULL, &operaciones);
    printk(KERN_INFO "Creado el archivo /proc/ram\n - Clase 4\n");
    return 0;
}

static void _delete(void)
{
    remove_proc_entry("ram", NULL);
    printk(KERN_INFO "Eliminado el archivo /proc/ram\n - Clase 4\n");
}

module_init(_insert);
module_exit(_delete);