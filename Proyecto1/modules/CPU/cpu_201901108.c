#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/sysinfo.h>
#include <linux/sched/task.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Módulo para la obtención de datos del uso de CPU");
MODULE_AUTHOR("Daniel Jiménez");
MODULE_VERSION("1.0");

static int calcularPorcentajeCPU(void)
{
    struct file *archivo;
    char lectura[256];

    int usuario, nice, idle, iowait, irq, softirq, steal, guest, guest_nice;
    int total, porcentaje;

    archivo = filp_open("/proc/stat", O_RDONLY, 0);
    if (archivo == NULL)
    {
        printk(KERN_INFO "Error al abrir el archivo\n");
        return -1;
    }

    memset(lectura, 0, 256);
    kernel_read(archivo, lectura, 256, &archivo->f_pos);

    sscanf(lectura, "cpu %d %d %d %d %d %d %d %d %d", &usuario, &nice, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);

    total = usuario + nice + idle + iowait + irq + softirq + steal + guest + guest_nice;

    porcentaje = (total - idle) * 100 / total;

    filp_close(archivo, NULL);

    return porcentaje;
}

static int escribir_archivo(struct seq_file *archivo, void *v)
{
    int porcentaje = calcularPorcentajeCPU();
    if (porcentaje == -1)
    {
        seq_printf(archivo, "{\"error\": \"Error al leer el archivo\"}\n");
    }
    else
    {
        seq_printf(archivo, "{\n");
        seq_printf(archivo, "  \"percentage_used\": %d,\n", porcentaje);
        seq_printf(archivo, "  \"tasks\": [\n");

        struct task_struct *task;
        int ram;
        bool first_task = true;

        for_each_process(task)
        {
            if (!first_task)
            {
                seq_printf(archivo, ",\n");
            }
            seq_printf(archivo, "    {\n");
            seq_printf(archivo, "      \"pid\": %d,\n", task->pid);
            seq_printf(archivo, "      \"name\": \"%s\",\n", task->comm);
            seq_printf(archivo, "      \"state\": %d,\n", task->__state);
            seq_printf(archivo, "      \"user\": %d,\n", task->cred->uid.val);
            if (task->mm)
            {
                ram = (get_mm_rss(task->mm) << PAGE_SHIFT) / (1024 * 1024);
                seq_printf(archivo, "      \"ram\": %d,\n", ram);
            }
            else
            {
                seq_printf(archivo, "      \"ram\": null,\n");
            }
            seq_printf(archivo, "      \"father\": %d\n", task->parent->pid);
            seq_printf(archivo, "    }");
            first_task = false;
        }

        seq_printf(archivo, "\n  ]\n");
        seq_printf(archivo, "}\n");
    }
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
    proc_create("cpu", 0, NULL, &operaciones);
    printk(KERN_INFO "Creado el archivo /proc/cpu\n - Clase 4\n");
    return 0;
}

static void _delete(void)
{
    remove_proc_entry("cpu", NULL);
    printk(KERN_INFO "Eliminado el archivo /proc/cpu\n - Clase 4\n");
}

module_init(_insert);
module_exit(_delete);
