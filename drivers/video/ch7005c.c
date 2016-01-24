#include <linux/miscdevice.h>  
#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/signal.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <asm/io.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/uaccess.h>

#include <linux/videodev.h>
#include <linux/video_encoder.h>

MODULE_DESCRIPTION(" LCD vs VGA");
MODULE_AUTHOR("Dave Perks");
MODULE_LICENSE("GPL");

#define module_driver(__driver, __register, __unregister, ...) \
static int __init __driver##_init(void) \
{ \
         return __register(&(__driver) , ##__VA_ARGS__); \
} \
 module_init(__driver##_init); \
static void __exit __driver##_exit(void) \
{ \
        __unregister(&(__driver) , ##__VA_ARGS__); \
} \
module_exit(__driver##_exit);

#define module_i2c_driver(__i2c_driver) \
         module_driver(__i2c_driver, i2c_add_driver, \
                         i2c_del_driver)
#if 0
#if 0 
static struct platform_driver s3c2412fb_driver = {
        .probe          = s3c2412fb_probe,
        .remove         = s3c2410fb_remove,
        .suspend        = s3c2410fb_suspend,
        .resume         = s3c2410fb_resume,
        .driver         = {
                .name   = "s3c2412-lcd",
                .owner  = THIS_MODULE,
        },
};


static struct i2c_board_info ch7005c_info = {
	I2C_BOARD_INFO("ch7005c", 0xEA),      
	//.platform_data = &ch7005c,
};

static struct i2c_client *ch7005c_client;

static int ch7005_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
        printk(KERN_ERR "[LIUYANG] [%s]LCD  %s\n", __func__);
        return 0;
}


static int ch7005_remove(struct i2c_client *client)
{

        printk(KERN_ERR "[LIUYANG] [%s]LCD  %s\n", __func__);
        return 0;
}
#endif

#endif


/*
struct miscdevice  {
          int minor;
          const char *name;
          const struct file_operations *fops;
          struct list_head list;
          struct device *parent;
          struct device *this_device;
          const struct attribute_group **groups;
          const char *nodename;
          umode_t mode;
};

*/
static long ch7005_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)  
{
	 printk(KERN_ERR "[LIUYANG] [s3c2410fb.c]LCD  %s\n", __func__);
	return 0;
}

static long ch7005_read(struct file *filp, unsigned int cmd, unsigned long arg)  
{
	 printk(KERN_ERR "[LIUYANG] [s3c2410fb.c]LCD  %s\n", __func__);
	return 0;
}

static long ch7005_write(struct file *filp, unsigned int cmd, unsigned long arg)  
{
	 printk(KERN_ERR "[LIUYANG] [s3c2410fb.c]LCD  %s\n", __func__);
	return 0;
}

static struct file_operations dev_fops = {  
    .owner          = THIS_MODULE,  
    .unlocked_ioctl = ch7005_ioctl,  
    .read 	    = &ch7005_read,
    .write 	    = &ch7005_write,
};  

#ifndef MISC_DYNAMIC_MINOR 
#define MISC_DYNAMIC_MINOR 255
#endif
static struct miscdevice misc_ch7005 = {
	.minor = MISC_DYNAMIC_MINOR, 
	.name = "misc_ch7005",
	.fops = &dev_fops,
};

static int ch7005c_attach_adapter(struct i2c_adapter *adapter)  
{	
	//int misc_register(struct miscdevice * misc); 
	misc_register(&misc_ch7005);
 	 printk(KERN_ERR "[LIUYANG] [s3c2410fb.c]LCD  %s\n", __func__);
	 //return i2c_probe(adapter, &addr_data, &ch7005_detect_client);  
	return 0;
} 

/*
extern int i2c_master_send(struct i2c_client *,const char* ,int);
extern int i2c_master_recv(struct i2c_client *,char* ,int);  


*/
 //i2c_add_driver(&i2c_driver_ch7005c);
#if 0
def TEST_CH7005C
static int ch7005c_dev_init(void)
{
#if 0

	struct i2c_adapter *i2c_adap;    

        printk(KERN_ERR "[LIUYANG] [s3c2410fb.c]LCD  %s\n", __func__);

	i2c_adap = i2c_get_adapter(0);   
	ch7005c_client = i2c_new_device(i2c_adap, &ch7005c_info);  
	//ch7005c_client 
	i2c_put_adapter(i2c_adap);
#endif
	printk(KERN_ERR "[LIUYANG] [s3c2410fb.c]LCD  %s\n", __func__);
	g_ch7005c_obj = NULL;
	return i2c_add_driver(&i2c_driver_ch7005c);

	return 0;	
}
static void ch7005c_dev_exit(void)
{
    printk(KERN_ERR "[LIUYANG] [s3c2410fb.c]LCD  %s\n", __func__);
	i2c_del_driver(&i2c_driver_ch7005c);
	
}

//module_init(s3c2410fb_init);
late_initcall(ch7005c_dev_init);
module_exit(ch7005c_dev_exit);
#endif

 struct ch7005c_obj{		
	 struct i2c_client client;		  
	 int users; // how many users using the driver	  
 };  
  
 struct ch7005c_obj* g_ch7005c_obj;  
 static unsigned short normal_i2c[] = {  
		 0xEA ,   
		 I2C_CLIENT_END  
 };  
 I2C_CLIENT_INSMOD; 

 static int ch7005_detect_client(struct i2c_adapter *adapter,int address,int kind)  
 {	
	 struct ch7005c_obj *pObj;	
  	 char value0[] = {0x04, 0x20};
  	 char value1[] = {0x07, 0x8c};
  	 char value2[] = {0x0e, 0x1b};
  	 char value3[] = {0x0d, 0x03};
	 int err = 0;  
	 int ret = 0;
  
	 printk(KERN_INFO "[LIUYANG] %s: %x ...\n", __func__, address);  
  
	 if( g_ch7005c_obj != NULL	) {  
		 //already allocated,inc user count, and return the allocated handle  
		 g_ch7005c_obj->users++;  
		 return 0;	
	 }	
  
	 /* alloc obj */ 
	 pObj = kmalloc(sizeof(struct ch7005c_obj), GFP_KERNEL);  
	 if (pObj==0){	
		 return -ENOMEM;  
	 }	
	 memset(pObj, 0, sizeof(struct ch7005c_obj));  
	 pObj->client.addr	  = address;  
	 pObj->client.adapter = adapter;  
	// pObj->client.driver  = &i2c_driver_ch7005c;  
	 pObj->client.flags   = 0;	//I2C_CLIENT_ALLOW_USE;  
	 pObj->users++;  
  
	 /* attach i2c client to sys i2c clients list */ 
	 if((err = i2c_attach_client(&pObj->client))){	
		 printk( KERN_ERR "I2C: ERROR: i2c_attach_client fail! address=%x\n",address);	
		 return err;  
	 }	
  
	 // store the pObj	
	 g_ch7005c_obj = pObj;	
	 ret = i2c_master_send(&pObj->client, (char *)value0, sizeof(value0)) ;
	 printk(KERN_ERR "[LIUYANG]  %s, value0  ret = %d\n", __func__, ret);
	 msleep(50);

	 ret = i2c_master_send(&pObj->client, (char *)value1, sizeof(value0)) ;
	 printk(KERN_ERR "[LIUYANG]  %s, value1  ret = %d\n", __func__, ret);
	 msleep(50);

	 ret = i2c_master_send(&pObj->client, (char *)value2, sizeof(value0)) ;
	 printk(KERN_ERR "[LIUYANG]  %s, value2  ret = %d\n", __func__, ret);
	 msleep(50);

	 ret = i2c_master_send(&pObj->client, (char *)value3, sizeof(value0)) ;
	 printk(KERN_ERR "[LIUYANG]  %s, value3  ret = %d\n", __func__, ret);
	 msleep(50);

	 printk( KERN_ERR "[LIUYANG ]I2C: i2c_attach_client ok! address=%x\n",address);  
  
	 return 0;	
 } 

 static int ch7005c_detach_client(struct i2c_client *client)  
 {	
	 int err;  

	 printk(KERN_ERR "[LIUYANG] [s3c2410fb.c]LCD  %s\n", __func__);
	 if( ! client->adapter ){  
		 return -ENODEV;  
	 }	
  
	 if( (err = i2c_detach_client(client)) ) {	
		 printk( KERN_ERR "Client deregistration failed (address=%x), client not detached.\n", client->addr);  
		 return err;  
	 }	
  
	 client->adapter = NULL;  
  
	 if( g_ch7005c_obj ){  
		 kfree(g_ch7005c_obj);	
	 }	
  
	 return 0;	
 } 

static struct i2c_driver i2c_driver_ch7005c = {  
	.driver = {  
	    .name = "ch7005c",      /* name */
	},   
	.attach_adapter = &ch7005c_attach_adapter,
	.detach_client  = &ch7005c_detach_client,
	.command        = NULL,  
};
module_i2c_driver(i2c_driver_ch7005c);
MODULE_AUTHOR("LIUYANG" );
MODULE_DESCRIPTION("Framebuffer driver for the s3c2410");
MODULE_LICENSE("GPL");
