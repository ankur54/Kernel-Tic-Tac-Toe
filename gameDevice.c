#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>

#define BUFF_SIZE 1024
#define game "tictactoe"

dev_t dev = 0;
struct class *dev_class;
struct cdev test_cdev;
static uint8_t *kernel_buffer;
static uint8_t board[10];
static int player = 0;
static int moves = 0;
static int over = 0;
int status, idx;



// To hold he file operations performed on this device

static int test_open (struct inode * pinode, struct file * pfile) {
    if((kernel_buffer = kmalloc(BUFF_SIZE , GFP_KERNEL)) == 0){
        printk(KERN_INFO "Cannot allocate memory in kernel\n");
        return -1;
    }
    printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
    return 0;
}

static ssize_t test_read (struct file * pfile, char __user * buffer, size_t length, loff_t * offset) {
    printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
    printk(KERN_ALERT "Kernel read: %s\n", kernel_buffer);
    copy_to_user(buffer, kernel_buffer, length);
    return 0;
}

static ssize_t test_write (struct file * pfile, const char __user * buffer, size_t length, loff_t * offset) {
    printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
    copy_from_user(kernel_buffer, buffer, length);
    printk(KERN_NOTICE "Kernel: %s", kernel_buffer);
    
    if (kernel_buffer[0] == '0' && (kernel_buffer[1] == '0' || kernel_buffer[1] == '1' || kernel_buffer[1] == '2' || kernel_buffer[1] == '3')) {
        if (kernel_buffer[1] == '0') {
            memset(board, '*', sizeof(board));
            board[9] = '\0';
            over = 0;
            moves = 0;
            if (kernel_buffer[2] == 'X' || kernel_buffer[2] == 'O') 
                strcpy(kernel_buffer, "INVFMT");
            else if (kernel_buffer[2] == ' ' && (kernel_buffer[3] == 'X' || kernel_buffer[3] == 'O')){
                if (kernel_buffer[3] == 'X')
                    player = 1;
                else 
                    player = 2;
                
                strcpy(kernel_buffer, "OK\0");
            }
            else strcpy(kernel_buffer, "UNKCMD");
        }
        else if (kernel_buffer[1] == '1') {
            memcpy(kernel_buffer, board, sizeof(board));
        }

        else if (!over) {
            if (kernel_buffer[1] == '2') {
                if (kernel_buffer[2] >= '0' && kernel_buffer[2] <= '9') 
                    strcpy(kernel_buffer, "INVFMT");

                else if (kernel_buffer[2] == ' ' && (kernel_buffer[3] >= '0' && kernel_buffer[3] <= '9')) {
                    if (kernel_buffer[2] >= '0' && kernel_buffer[2] <= '9') 
                        strcpy(kernel_buffer, "INVFMT");

                    else if (kernel_buffer[4] == ' ' && (kernel_buffer[5] >= '0' && kernel_buffer[5] <= '9')) {
                        if ((moves + player)%2 == 1) {
                            if ((kernel_buffer[3] >= '0' && kernel_buffer[3] <= '2') &&
                                (kernel_buffer[5] >= '0' && kernel_buffer[5] <= '2')) {
                                idx = (kernel_buffer[3] - '0') * 3 + (kernel_buffer[5] - '0');

                                printk(KERN_ALERT "idx: %d", idx);
                                printk(KERN_ALERT "moves: %d", moves);
                                printk(KERN_ALERT "player: %d", player);

                                if (idx < 9) {
                                    // check if the position is empty:
                                    if (board[idx] == '*') {
                                        moves++;

                                        if (player == 1) 
                                            board[idx] = 'X';
                                        else 
                                            board[idx] = 'O';
                                        
                                        // check if the player has won or not
                                        if (player == 1) {
                                            if ((board[0] == board[1] && board[1] == board[2] && board[2] == 'X') || (board[6] == board[7] && board[7] == board[8] && board[8] == 'X') || (board[0] == board[3] && board[3] == board[6] && board[6] == 'X') ||
                                                (board[2] == board[5] && board[5] == board[8] && board[8] == 'X') || (board[0] == board[4] && board[4] == board[8] && board[8] == 'X') || (board[2] == board[4] && board[4] == board[6] && board[6] == 'X')) {
                                                    status = 1;
                                            }
                                            else status = 0;
                                        }
                                        else {
                                            if ((board[0] == board[1] && board[1] == board[2] && board[2] == 'O') || (board[6] == board[7] && board[7] == board[8] && board[8] == 'O') || (board[0] == board[3] && board[3] == board[6] && board[6] == 'O') ||
                                                (board[2] == board[5] && board[5] == board[8] && board[8] == 'O') || (board[0] == board[4] && board[4] == board[8] && board[8] == 'O') || (board[2] == board[4] && board[4] == board[6] && board[6] == 'O')) {
                                                    status = 1;
                                            }
                                            else status = 0;
                                        }

                                        if (status == 1) {
                                            strcpy(kernel_buffer, "WIN");
                                            over = 1;
                                        }
                                        else strcpy(kernel_buffer, "OK");
                                    }
                                    else strcpy(kernel_buffer, "ILLMOV");
                                }
                                else strcpy(kernel_buffer, "ILLMOV");
                            }
                            else strcpy(kernel_buffer, "ILLMOV");
                        }
                        else strcpy(kernel_buffer, "OOT");
                    }
                    else strcpy(kernel_buffer, "UNKCMD");                    
                }
                else strcpy(kernel_buffer, "UNKCMD");
            }
            else if (kernel_buffer[1] == '3') {
                int i;
                if ((moves + player)%2 == 0) {
                    moves++;

                    printk(KERN_ALERT "moves: %d", moves);
                    printk(KERN_ALERT "player: %d", player);

                    for (i = 0; i < 9; i++) {
                        if (board[i] == '*') {
                            if (player == 1)
                                board[i] = 'O';
                            else
                                board[i] = 'X';
                            break;
                        }
                    }

                    if (i == 9) 
                        over = 1;

                    // check if the player has won or not
                    // if (player == 1) {
                    //     if ((board[0] == board[1] && board[1] == board[2] && board[2] = 'X') || (board[6] == board[7] && board[7] == board[8] && board[8] = 'X') || (board[0] == board[3] && board[3] == board[6] && board[6] = 'X') ||
                    //         (board[2] == board[5] && board[5] == board[8] && board[8] = 'X') || (board[0] == board[4] && board[4] == board[8] && board[8] = 'X') || (board[2] == board[4] && board[4] == board[6] && board[6] = 'X')) {
                    //             status = 1;
                    //     }
                    //     else status = 0;
                    // }
                    // else {
                    //     if ((board[0] == board[1] && board[1] == board[2] && board[2] = 'O') || (board[6] == board[7] && board[7] == board[8] && board[8] = 'O') || (board[0] == board[3] && board[3] == board[6] && board[6] = 'O') ||
                    //         (board[2] == board[5] && board[5] == board[8] && board[8] = 'O') || (board[0] == board[4] && board[4] == board[8] && board[8] = 'O') || (board[2] == board[4] && board[4] == board[6] && board[6] = 'O')) {
                    //             status = 1;
                    //     }
                    //     else status = 0;
                    // }

                    // if (status == 1) {
                    //     strcpy(kernel_buffer, "WIN");
                    //     over = 1;
                    // }
                    strcpy(kernel_buffer, "OK");
                }
                else strcpy(kernel_buffer, "OOT");
            }
            else strcpy(kernel_buffer, "UNKCMD");
        } 
        else strcpy(kernel_buffer, "NOGAME");
    }
    else if (kernel_buffer[1] == ' ') 
        strcpy(kernel_buffer, "INVFMT");
    else strcpy(kernel_buffer, "UNKCMD");
    
    printk(KERN_NOTICE "KERNEL: %s\n", kernel_buffer);
    printk(KERN_NOTICE "PLAYER: %d\n", player);
    printk(KERN_NOTICE "BOARD: %s", board);
    return length;
}


static int test_release (struct inode * pinode, struct file * pfile) {
    kfree(kernel_buffer);
    printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
    return 0;
}

struct file_operations test_file_operation = {
    .owner   = THIS_MODULE,
    .open    = test_open,
    .read    = test_read,
    .write   = test_write,
    .release = test_release
};


static int __init test_init(void) {
    printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);

    /* Allocating major number */
    if (alloc_chrdev_region(&dev, 0, 1, game) < 0) {
        printk(KERN_ALERT "Cannot allocate major number\n");
        return -1;
    }
    printk(KERN_INFO "Major number = %d, Minor number = %d\n", MAJOR(dev), MINOR(dev));

    /* Creating cdev structure */
    cdev_init(&test_cdev, &test_file_operation);

    /* Adding character device to the system */
    if (cdev_add(&test_cdev, dev, 1) < 0) {
        // error message
        printk(KERN_INFO "Cannot add the device to the system\n");
        goto r_class;
    }

    /* Creating structure class */
    if ((dev_class = class_create(THIS_MODULE, game)) == NULL) {
        //error meesage
        printk(KERN_INFO "Cannot create the struct class\n");
        goto r_class;
    }

    /* Creating device (file) */
    if (device_create(dev_class, NULL, dev, NULL, game) == NULL) {
        //error message
        printk(KERN_INFO "Cannot create the Device\n");
        goto r_device;
    }
    printk(KERN_INFO "Inserting device driver done...!!!");

    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}


static void __exit test_exit(void) {
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&test_cdev);
    printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
    unregister_chrdev_region(dev, 1);
}


module_init(test_init);
module_exit(test_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("MY PROGRAMMING CAREER");
MODULE_DESCRIPTION("WORLD CLASS PROGRAM. PLEASE HIRE ME IN GOOGLE");
MODULE_VERSION("2.0");