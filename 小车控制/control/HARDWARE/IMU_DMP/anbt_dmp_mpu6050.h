#ifndef _DMP_MPU6050_H_
#define _DMP_MPU6050_H_

#include <string.h>
#include "sys.h"
#include "anbt_i2c.h"
#include "anbt_dmp_driver.h"
#include "anbt_dmp_fun.h"
//#include "anbt_communication.h"
 //long quat[4];
//short gyro[3], accel[3], sensors;
//unsigned char more;
//unsigned long sensor_timestamp;
//#define   AnBT_Uart1_Send_String	AnBT_Uart1_Send_Num
#define q30  1073741824.0f
extern float Pitch,Roll,Yaw;
extern  short int MPU6050_gyro[3], MPU6050_accel[3], sensors;

void MPU6050_Pose(void);
#define BIT_I2C_MST_VDDIO   (0x80)
#define BIT_FIFO_EN         (0x40)
#define BIT_DMP_EN          (0x80)
#define BIT_FIFO_RST        (0x04)
#define BIT_DMP_RST         (0x08)
#define BIT_FIFO_OVERFLOW   (0x10)
#define BIT_DATA_RDY_EN     (0x01)
#define BIT_DMP_INT_EN      (0x02)
#define BIT_MOT_INT_EN      (0x40)
#define BITS_FSR            (0x18)
#define BITS_LPF            (0x07)
#define BITS_HPF            (0x07)
#define BITS_CLK            (0x07)
#define BIT_FIFO_SIZE_1024  (0x40)
#define BIT_FIFO_SIZE_2048  (0x80)
#define BIT_FIFO_SIZE_4096  (0xC0)
#define BIT_RESET           (0x80)
#define BIT_SLEEP           (0x40)
#define BIT_S0_DELAY_EN     (0x01)
#define BIT_S2_DELAY_EN     (0x04)
#define BITS_SLAVE_LENGTH   (0x0F)
#define BIT_SLAVE_BYTE_SW   (0x40)
#define BIT_SLAVE_GROUP     (0x10)
#define BIT_SLAVE_EN        (0x80)
#define BIT_I2C_READ        (0x80)
#define BITS_I2C_MASTER_DLY (0x1F)
#define BIT_AUX_IF_EN       (0x20)
#define BIT_ACTL            (0x80)
#define BIT_LATCH_EN        (0x20)
#define BIT_ANY_RD_CLR      (0x10)
#define BIT_BYPASS_EN       (0x02)
#define BITS_WOM_EN         (0xC0)
#define BIT_LPA_CYCLE       (0x20)
#define BIT_STBY_XA         (0x20)
#define BIT_STBY_YA         (0x10)
#define BIT_STBY_ZA         (0x08)
#define BIT_STBY_XG         (0x04)
#define BIT_STBY_YG         (0x02)
#define BIT_STBY_ZG         (0x01)
#define BIT_STBY_XYZA       (BIT_STBY_XA | BIT_STBY_YA | BIT_STBY_ZA)
#define BIT_STBY_XYZG       (BIT_STBY_XG | BIT_STBY_YG | BIT_STBY_ZG)
//
#define INV_X_GYRO      (0x40)
#define INV_Y_GYRO      (0x20)
#define INV_Z_GYRO      (0x10)
#define INV_XYZ_GYRO    (INV_X_GYRO | INV_Y_GYRO | INV_Z_GYRO)
#define INV_XYZ_ACCEL   (0x08)
#define INV_XYZ_COMPASS (0x01)
//
#define DMP_INT_GESTURE     (0x01)
#define DMP_INT_CONTINUOUS  (0x02)
#define DMP_FEATURE_TAP             (0x001)
#define DMP_FEATURE_ANDROID_ORIENT  (0x002)
#define DMP_FEATURE_LP_QUAT         (0x004)
#define DMP_FEATURE_PEDOMETER       (0x008)
#define DMP_FEATURE_6X_LP_QUAT      (0x010)
#define DMP_FEATURE_GYRO_CAL        (0x020)
#define DMP_FEATURE_SEND_RAW_ACCEL  (0x040)
#define DMP_FEATURE_SEND_RAW_GYRO   (0x080)
#define DMP_FEATURE_SEND_CAL_GYRO   (0x100)
#define INV_WXYZ_QUAT       		(0x100)
//
#define PRODUCT_WHOAMI				(0x68)
#define DEFAULT_MPU_HZ  			(200)
// #define MAX_PACKET_LENGTH 			(12)
//
struct gyro_reg_s {
    unsigned char who_am_i;
    unsigned char rate_div;
    unsigned char lpf;
    unsigned char prod_id;
    unsigned char user_ctrl;
    unsigned char fifo_en;
    unsigned char gyro_cfg;
    unsigned char accel_cfg;
    unsigned char motion_thr;
    unsigned char motion_dur;
    unsigned char fifo_count_h;
    unsigned char fifo_r_w;
    unsigned char raw_gyro;
    unsigned char raw_accel;
    unsigned char temp;
    unsigned char int_enable;
    unsigned char dmp_int_status;
    unsigned char int_status;
    unsigned char pwr_mgmt_1;
    unsigned char pwr_mgmt_2;
    unsigned char int_pin_cfg;
    unsigned char mem_r_w;
    unsigned char accel_offs;
    unsigned char i2c_mst;
    unsigned char bank_sel;
    unsigned char mem_start_addr;
    unsigned char prgm_start_h;
};
struct hw_s {
    unsigned char addr;
    unsigned short max_fifo;
    unsigned char num_reg;
    unsigned short temp_sens;
    short temp_offset;
    unsigned short bank_size;
};
struct test_s {
    unsigned long gyro_sens;
    unsigned long accel_sens;
    unsigned char reg_rate_div;
    unsigned char reg_lpf;
    unsigned char reg_gyro_fsr;
    unsigned char reg_accel_fsr;
    unsigned short wait_ms;
    unsigned char packet_thresh;
    float min_dps;
    float max_dps;
    float max_gyro_var;
    float min_g;
    float max_g;
    float max_accel_var;
};
struct motion_int_cache_s {
    unsigned short gyro_fsr;
    unsigned char accel_fsr;
    unsigned short lpf;
    unsigned short sample_rate;
    unsigned char sensors_on;
    unsigned char fifo_sensors;
    unsigned char dmp_on;
};
struct chip_cfg_s {
    unsigned char gyro_fsr;
    unsigned char accel_fsr;
    unsigned char sensors;
    unsigned char lpf;
    unsigned char clk_src;
    unsigned short sample_rate;
    unsigned char fifo_enable;
    unsigned char int_enable;
    unsigned char bypass_mode;
    unsigned char accel_half;
    unsigned char lp_accel_mode;
    unsigned char int_motion_only;
    struct motion_int_cache_s cache;
    unsigned char active_low_int;
    unsigned char latched_int;
    unsigned char dmp_on;
    unsigned char dmp_loaded;
    unsigned short dmp_sample_rate;
};
struct gyro_state_s {
    const struct gyro_reg_s *reg;
    const struct hw_s *hw;
    struct chip_cfg_s chip_cfg;
    const struct test_s *test;
};
//


//
enum accel_fsr_e {
    INV_FSR_2G = 0,
    INV_FSR_4G,
    INV_FSR_8G,
    INV_FSR_16G,
    NUM_ACCEL_FSR
};
enum clock_sel_e {
    INV_CLK_INTERNAL = 0,
    INV_CLK_PLL,
    NUM_CLK
};
enum gyro_fsr_e {
    INV_FSR_250DPS = 0,
    INV_FSR_500DPS,
    INV_FSR_1000DPS,
    INV_FSR_2000DPS,
    NUM_GYRO_FSR
};
enum lpf_e {
    INV_FILTER_256HZ_NOLPF2 = 0,
    INV_FILTER_188HZ,
    INV_FILTER_98HZ,
    INV_FILTER_42HZ,
    INV_FILTER_20HZ,
    INV_FILTER_10HZ,
    INV_FILTER_5HZ,
    INV_FILTER_2100HZ_NOLPF,
    NUM_FILTER
};
enum lp_accel_rate_e {

    INV_LPA_1_25HZ,
    INV_LPA_5HZ,
    INV_LPA_20HZ,
    INV_LPA_40HZ
};
//
static signed char gyro_orientation[9] = {-1, 0, 0, 0,-1, 0, 0, 0, 1};
//
// int mpu_set_gyro_fsr(unsigned short fsr);		//设置陀螺仪检测范围
int mpu_set_accel_fsr(unsigned char fsr);		//设置加速度检测范围
int mpu_set_lpf(unsigned short lpf);			//设置低通滤波
//
int mpu_set_int_latched(unsigned char enable);	//使能中断锁存
static int set_int_enable(unsigned char enable);
//
int mpu_reset_fifo(void);
int mpu_configure_fifo(unsigned char sensors);	//**选择传感器导通FIFO
//
int mpu_lp_accel_mode(unsigned char rate);		//加速度低功耗模式
int mpu_set_sample_rate(unsigned short rate);	//**设置采样频率
int mpu_set_bypass(unsigned char bypass_on);	//设置设备旁路模式 Bypass
int mpu_set_sensors(unsigned char sensors);		//**关闭或打开特定的传感器
//
int mpu_get_accel_sens(unsigned short *sens);	//得到加速度比例因子的敏感度
int mpu_get_accel_fsr(unsigned char *fsr);		//得到加速度检测范围

void run_self_test(char cal);
int mpu_set_dmp_state(unsigned char enable);

float number_to_dps(s16 gyro_number);

#endif

