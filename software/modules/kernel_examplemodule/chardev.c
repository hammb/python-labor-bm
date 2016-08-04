/*
 *  chardev.c: Creates a read-only char device that says how many times
 *  you've read from the dev file
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>


static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

/* 
 * Global variables are declared as static, so are global within the file. 
 */
static char hello_world[] = "Hello World\n";

static dev_t hello_dev_number;
static struct cdev *driver_object;
static struct class *hello_class;
static struct device *hello_dev;

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = device_read
};

/*
 * This function is called when the module is loaded
 */
static int __init mod_init(void)
{
	if(alloc_chrdev_region(&hello_dev_number,0,1,"Hello")<0)
		return -EIO;

	driver_object = cdev_alloc();
	if(driver_object==NULL)
		goto free_device_number;

	driver_object->owner = THIS_MODULE;
	driver_object->ops = &fops;
	if(cdev_add(driver_object,hello_dev_number,1))
		goto free_cdev;

	//Eintrag im Sysfs
	hello_class = class_create(THIS_MODULE, "Hello");
	if(IS_ERR(hello_class))
		goto free_cdev;

	hello_dev = device_create(hello_class,NULL,hello_dev_number,NULL,"%s","hello");
	dev_info(hello_dev, "mod_init called\n");
	return 0;

free_cdev:
	kobject_put(&driver_object->kobj);

free_device_number:
	unregister_chrdev_region(hello_dev_number,1);

	return -EIO;
}

/*
 * This function is called when the module is unloaded
 */
static void __exit mod_exit(void)
{
	dev_info(hello_dev, "mod exit called\n");

	//Loeschen des Sysfs Eintrags
	device_destroy(hello_class, hello_dev_number);
	class_destroy(hello_class);

	cdev_del(driver_object);
	unregister_chrdev_region(hello_dev_number, 1);
}

module_init(mod_init);
module_exit(mod_exit);
MODULE_LICENSE("GPL");

/*
 * Methods
 */

/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char *user,		/* buffer to fill with data !USERSPACE!*/
			   size_t length,	/* length of the buffer     */
			   loff_t * offset)
{
	unsigned long not_copied, to_copy;

	to_copy = min(length, strlen(hello_world)+1);
	not_copied = copy_to_user(user, hello_world, to_copy);
	return to_copy-not_copied;
}

/*  
 * Called when a process writes to dev file: echo "hi" > /dev/hello 
 */
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
	return -EINVAL;
}
