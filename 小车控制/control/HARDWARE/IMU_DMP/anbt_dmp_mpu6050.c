#include "sys.h"
#include "anbt_dmp_mpu6050.h"
#include "anbt_dmp_driver.h"
// #include "stmflash.h"
#include "delay.h"
#include "mpu_i2c.h"


// /////////////////******************************//////////// ///// ////////   
#define q30  1073741824.0f
float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
unsigned long sensor_timestamp;
short int MPU6050_gyro[3], MPU6050_accel[3], sensors;
unsigned char more;
long quat[4];							
float Yaw=0.00;
float Roll,Pitch;
 
 u8 AnBT_DMP_MPU6050_Init(void)	
{
	MPU_I2C_Init_IO();						//I2C初始化
	
	RCC->APB2ENR|=1<<4;     //使能PORTC时钟 
	RCC->APB2ENR|=1<<0;     //开启辅助时钟		  
	GPIOC->CRH&=0XF0FFFFFF; //PC14设置成输入	  
	GPIOC->CRH|=0X08000000;
	//Ex_NVIC_Config(GPIO_C,14,FTIR); 			//下降沿触发
	//MY_NVIC_Init(0,0,EXTI15_10_IRQn,2);    //抢占2，子优先级2，组2

	AnBT_DMP_MPU6050_DEV_CFG();	
    
	if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL));
	if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL)); 
	if(!mpu_set_sample_rate(DEFAULT_MPU_HZ)); 
	if(!dmp_load_motion_driver_firmware());
	if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation))); 
	if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
		DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | 
		DMP_FEATURE_SEND_CAL_GYRO |DMP_FEATURE_GYRO_CAL));
	if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ));
	run_self_test(0);
	if(!mpu_set_dmp_state(1)); 
	return 0;

}

void AnBT_DMP_Delay_ms(unsigned int dex)
{
	delay_ms(dex);
}
void MPU6050_Pose()
{
  dmp_read_fifo(MPU6050_gyro, MPU6050_accel, quat, &sensor_timestamp, &sensors,&more);
	if (sensors & INV_WXYZ_QUAT )
	{
		q0=quat[0] / q30;
		q1=quat[1] / q30;
		q2=quat[2] / q30;
		q3=quat[3] / q30;
		Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; 
		Roll =  -atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; 
		Yaw = 	atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;		
	// printf("pitch: %.2f    roll:%.2f		yaw:%.2f\r\n",Pitch,Roll,Yaw);		//普通串口输	?
   }	
 }	
/// ///// /// /// /////****** **** ***** ***** ***//////// /////// /////// //// //// ///
const struct gyro_reg_s reg= {
0x75,  //who_am_i
0x19,  //rate_div
0x1A,  //lpf
0x0C,  //prod_id
0x6A,  //user_ctrl
0x23,  //fifo_en
0x1B,  //gyro_cfg
0x1C,  //accel_cfg
0x1F,  // motion_thr
0x20,  // motion_dur
0x72,  // fifo_count_h
0x74,  // fifo_r_w
0x43,  // raw_gyro
0x3B,  // raw_accel
0x41,  // temp
0x38,  // int_enable
0x39,  //  dmp_int_status
0x3A,  //  int_status
0x6B,  // pwr_mgmt_1
0x6C,  // pwr_mgmt_2
0x37,  // int_pin_cfg
0x6F,  // mem_r_w
0x06,  // accel_offs
0x24,  // i2c_mst
0x6D,  // bank_sel
0x6E,  // mem_start_addr
0x70   // prgm_start_h
};
const struct hw_s hw={
0x68,	//addr
1024,	//max_fifo
118,	//num_reg
340,	//temp_sens
-521,	//temp_offset
256	 	//bank_size
};
const struct test_s test={
32768/250,	//gyro_sens
32768/16,		//	accel_sens
0,				 	//	reg_rate_div
1,					//	reg_lpf
0,				 	//	reg_gyro_fsr
0x18,				//	reg_accel_fsr
50,					//	wait_ms
5,					//	packet_thresh
10.0f,			//	min_dps
105.0f,			//	max_dps
0.14f,			//	max_gyro_var
0.3f,		   	//	min_g
0.95f,		  //	max_g
0.14f		   	//	max_accel_var
};
static struct gyro_state_s st={
  &reg,
  &hw,
  {0},
  &test
};
//
int mpu_set_gyro_fsr(unsigned short fsr)
{
    unsigned char data;

    if (!(st.chip_cfg.sensors))
        return -1;

    switch (fsr) {
    case 250:
        data = INV_FSR_250DPS << 3;
        break;
    case 500:
        data = INV_FSR_500DPS << 3;
        break;
    case 1000:
        data = INV_FSR_1000DPS << 3;
        break;
    case 2000:
        data = INV_FSR_2000DPS << 3;
        break;
    default:
        return -1;
    }

    if (st.chip_cfg.gyro_fsr == (data >> 3))
        return 0;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->gyro_cfg, 1, &data))
        return -1;
    st.chip_cfg.gyro_fsr = data >> 3;
    return 0;
}
int mpu_set_accel_fsr(unsigned char fsr)
{
    unsigned char data;

    if (!(st.chip_cfg.sensors))
        return -1;

    switch (fsr) {
    case 2:
        data = INV_FSR_2G << 3;
        break;
    case 4:
        data = INV_FSR_4G << 3;
        break;
    case 8:
        data = INV_FSR_8G << 3;
        break;
    case 16:
        data = INV_FSR_16G << 3;
        break;
    default:
        return -1;
    }
    if (st.chip_cfg.accel_fsr == (data >> 3))
        return 0;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->accel_cfg, 1, &data))
        return -1;
    st.chip_cfg.accel_fsr = data >> 3;
    return 0;
}
int mpu_set_lpf(unsigned short lpf)
{
    unsigned char data;

    if (!(st.chip_cfg.sensors))
        return -1;

    if (lpf >= 188)
        data = INV_FILTER_188HZ;
    else if (lpf >= 98)
        data = INV_FILTER_98HZ;
    else if (lpf >= 42)
        data = INV_FILTER_42HZ;
    else if (lpf >= 20)
        data = INV_FILTER_20HZ;
    else if (lpf >= 10)
        data = INV_FILTER_10HZ;
    else
        data = INV_FILTER_5HZ;

    if (st.chip_cfg.lpf == data)
        return 0;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->lpf, 1, &data))
        return -1;
    st.chip_cfg.lpf = data;
    return 0;
}
//
int mpu_set_int_latched(unsigned char enable)
{
    unsigned char tmp;
    if (st.chip_cfg.latched_int == enable)
        return 0;

    if (enable)
        tmp = BIT_LATCH_EN | BIT_ANY_RD_CLR;
    else
        tmp = 0;
    if (st.chip_cfg.bypass_mode)
        tmp |= BIT_BYPASS_EN;
    if (st.chip_cfg.active_low_int)
        tmp |= BIT_ACTL;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->int_pin_cfg, 1, &tmp))
        return -1;
    st.chip_cfg.latched_int = enable;
    return 0;
}

static int set_int_enable(unsigned char enable)
{
    unsigned char tmp;

    if (st.chip_cfg.dmp_on) {
        if (enable)
            tmp = BIT_DMP_INT_EN;
        else
            tmp = 0x00;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->int_enable, 1, &tmp))
            return -1;
        st.chip_cfg.int_enable = tmp;
    } else {
        if (!st.chip_cfg.sensors)
            return -1;
        if (enable && st.chip_cfg.int_enable)
            return 0;
        if (enable)
            tmp = BIT_DATA_RDY_EN;
        else
            tmp = 0x00;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->int_enable, 1, &tmp))
            return -1;
        st.chip_cfg.int_enable = tmp;
    }
    return 0;
}
//
int mpu_reset_fifo(void)
{
    unsigned char data;

    if (!(st.chip_cfg.sensors))
        return -1;

    data = 0;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->int_enable, 1, &data))
        return -1;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->fifo_en, 1, &data))
        return -1;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->user_ctrl, 1, &data))
        return -1;

    if (st.chip_cfg.dmp_on) {
        data = BIT_FIFO_RST | BIT_DMP_RST;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->user_ctrl, 1, &data))
            return -1;
        AnBT_DMP_Delay_ms(50);
        data = BIT_DMP_EN | BIT_FIFO_EN;
        if (st.chip_cfg.sensors & INV_XYZ_COMPASS)
            data |= BIT_AUX_IF_EN;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->user_ctrl, 1, &data))
            return -1;
        if (st.chip_cfg.int_enable)
            data = BIT_DMP_INT_EN;
        else
            data = 0;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->int_enable, 1, &data))
            return -1;
        data = 0;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->fifo_en, 1, &data))
            return -1;
    } else {
        data = BIT_FIFO_RST;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->user_ctrl, 1, &data))
            return -1;
        if (st.chip_cfg.bypass_mode || !(st.chip_cfg.sensors & INV_XYZ_COMPASS))
            data = BIT_FIFO_EN;
        else
            data = BIT_FIFO_EN | BIT_AUX_IF_EN;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->user_ctrl, 1, &data))
            return -1;
        AnBT_DMP_Delay_ms(50);
        if (st.chip_cfg.int_enable)
            data = BIT_DATA_RDY_EN;
        else
            data = 0;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->int_enable, 1, &data))
            return -1;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->fifo_en, 1, &st.chip_cfg.fifo_enable))
            return -1;
    }
    return 0;
}
/**
 *  @brief      Select which sensors are pushed to FIFO.
 *  @e sensors can contain a combination of the following flags:
 *  \n INV_X_GYRO, INV_Y_GYRO, INV_Z_GYRO
 *  \n INV_XYZ_GYRO
 *  \n INV_XYZ_ACCEL
 *  @param[in]  sensors Mask of sensors to push to FIFO.
 *  @return     0 if successful.
 */
int mpu_configure_fifo(unsigned char sensors)
{
    unsigned char prev;
    int result = 0;

    sensors &= ~INV_XYZ_COMPASS;
    if (st.chip_cfg.dmp_on)
        return 0;
    else {
        if (!(st.chip_cfg.sensors))
            return -1;
        prev = st.chip_cfg.fifo_enable;
        st.chip_cfg.fifo_enable = sensors & st.chip_cfg.sensors;
        if (st.chip_cfg.fifo_enable != sensors)
            result = -1;
        else
            result = 0;
        if (sensors || st.chip_cfg.lp_accel_mode)
            set_int_enable(1);
        else
            set_int_enable(0);
        if (sensors) {
            if (mpu_reset_fifo()) {
                st.chip_cfg.fifo_enable = prev;
                return -1;
            }
        }
    }

    return result;
}
//
int mpu_lp_accel_mode(unsigned char rate)
{
    unsigned char tmp[2];

    if (rate > 40)
        return -1;

    if (!rate) {
        mpu_set_int_latched(0);
        tmp[0] = 0;
        tmp[1] = BIT_STBY_XYZG;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->pwr_mgmt_1, 2, tmp))
            return -1;
        st.chip_cfg.lp_accel_mode = 0;
        return 0;
    }
    mpu_set_int_latched(1);

    tmp[0] = BIT_LPA_CYCLE;
    if (rate == 1) {
        tmp[1] = INV_LPA_1_25HZ;
        mpu_set_lpf(5);
    } else if (rate <= 5) {
        tmp[1] = INV_LPA_5HZ;
        mpu_set_lpf(5);
    } else if (rate <= 20) {
        tmp[1] = INV_LPA_20HZ;
        mpu_set_lpf(10);
    } else {
        tmp[1] = INV_LPA_40HZ;
        mpu_set_lpf(20);
    }
    tmp[1] = (tmp[1] << 6) | BIT_STBY_XYZG;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->pwr_mgmt_1, 2, tmp))
        return -1;

    st.chip_cfg.sensors = INV_XYZ_ACCEL;
    st.chip_cfg.clk_src = 0;
    st.chip_cfg.lp_accel_mode = 1;
    mpu_configure_fifo(0);

    return 0;
}
int mpu_set_sample_rate(unsigned short rate)
{
    unsigned char data;

    if (!(st.chip_cfg.sensors))
        return -1;

    if (st.chip_cfg.dmp_on)
        return -1;
    else {
        if (st.chip_cfg.lp_accel_mode) {
            if (rate && (rate <= 40)) {
                /* Just stay in low-power accel mode. */
                mpu_lp_accel_mode(rate);
                return 0;
            }
            /* Requested rate exceeds the allowed frequencies in LP accel mode,
             * switch back to full-power mode.
             */
            mpu_lp_accel_mode(0);
        }
        if (rate < 4)
            rate = 4;
        else if (rate > 1000)
            rate = 1000;

        data = 1000 / rate - 1;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->rate_div, 1, &data))
            return -1;

        st.chip_cfg.sample_rate = 1000 / (1 + data);


        /* Automatically set LPF to 1/2 sampling rate. */
        mpu_set_lpf(st.chip_cfg.sample_rate >> 1);
        return 0;
    }
}
int mpu_set_bypass(unsigned char bypass_on)
{
    unsigned char tmp;

    if (st.chip_cfg.bypass_mode == bypass_on)
        return 0;

    if (bypass_on) {
        if (AnBT_DMP_I2C_Read(st.hw->addr, st.reg->user_ctrl, 1, &tmp))
            return -1;
        tmp &= ~BIT_AUX_IF_EN;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->user_ctrl, 1, &tmp))
            return -1;
        AnBT_DMP_Delay_ms(3);
        tmp = BIT_BYPASS_EN;
        if (st.chip_cfg.active_low_int)
            tmp |= BIT_ACTL;
        if (st.chip_cfg.latched_int)
            tmp |= BIT_LATCH_EN | BIT_ANY_RD_CLR;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->int_pin_cfg, 1, &tmp))
            return -1;
    } else {
        /* Enable I2C master mode if compass is being used. */
        if (AnBT_DMP_I2C_Read(st.hw->addr, st.reg->user_ctrl, 1, &tmp))
            return -1;
        if (st.chip_cfg.sensors & INV_XYZ_COMPASS)
            tmp |= BIT_AUX_IF_EN;
        else
            tmp &= ~BIT_AUX_IF_EN;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->user_ctrl, 1, &tmp))
            return -1;
        AnBT_DMP_Delay_ms(3);
        if (st.chip_cfg.active_low_int)
            tmp = BIT_ACTL;
        else
            tmp = 0;
        if (st.chip_cfg.latched_int)
            tmp |= BIT_LATCH_EN | BIT_ANY_RD_CLR;
        if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->int_pin_cfg, 1, &tmp))
            return -1;
    }
    st.chip_cfg.bypass_mode = bypass_on;
    return 0;
}

int mpu_set_sensors(unsigned char sensors)
{
    unsigned char data;

    if (sensors & INV_XYZ_GYRO)
        data = INV_CLK_PLL;
    else if (sensors)
        data = 0;
    else
        data = BIT_SLEEP;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->pwr_mgmt_1, 1, &data)) {
        st.chip_cfg.sensors = 0;
        return -1;
    }
    st.chip_cfg.clk_src = data & ~BIT_SLEEP;

    data = 0;
    if (!(sensors & INV_X_GYRO))
        data |= BIT_STBY_XG;
    if (!(sensors & INV_Y_GYRO))
        data |= BIT_STBY_YG;
    if (!(sensors & INV_Z_GYRO))
        data |= BIT_STBY_ZG;
    if (!(sensors & INV_XYZ_ACCEL))
        data |= BIT_STBY_XYZA;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->pwr_mgmt_2, 1, &data)) {
        st.chip_cfg.sensors = 0;
        return -1;
    }

    if (sensors && (sensors != INV_XYZ_ACCEL))
        /* Latched interrupts only used in LP accel mode. */
        mpu_set_int_latched(0);


    st.chip_cfg.sensors = sensors;
    st.chip_cfg.lp_accel_mode = 0;
    AnBT_DMP_Delay_ms(50);
    return 0;
}
//
int mpu_set_dmp_state(unsigned char enable)
{
    unsigned char tmp;
    if (st.chip_cfg.dmp_on == enable)
        return 0;

    if (enable) {
        if (!st.chip_cfg.dmp_loaded)
            return -1;
        /* Disable data ready interrupt. */
        set_int_enable(0);
        /* Disable bypass mode. */
        mpu_set_bypass(0);
        /* Keep constant sample rate, FIFO rate controlled by DMP. */
        mpu_set_sample_rate(st.chip_cfg.dmp_sample_rate);
        /* Remove FIFO elements. */
        tmp = 0;
        AnBT_DMP_I2C_Write(st.hw->addr, 0x23, 1, &tmp);
        st.chip_cfg.dmp_on = 1;
        /* Enable DMP interrupt. */
        set_int_enable(1);
        mpu_reset_fifo();
    } else {
        /* Disable DMP interrupt. */
        set_int_enable(0);
        /* Restore FIFO settings. */
        tmp = st.chip_cfg.fifo_enable;
        AnBT_DMP_I2C_Write(st.hw->addr, 0x23, 1, &tmp);
        st.chip_cfg.dmp_on = 0;
        mpu_reset_fifo();
    }
    return 0;
}
//
int mpu_get_gyro_fsr(unsigned short *fsr)
{
    switch (st.chip_cfg.gyro_fsr) {
    case INV_FSR_250DPS:
        fsr[0] = 250;
        break;
    case INV_FSR_500DPS:
        fsr[0] = 500;
        break;
    case INV_FSR_1000DPS:
        fsr[0] = 1000;
        break;
    case INV_FSR_2000DPS:
        fsr[0] = 2000;
        break;
    default:
        fsr[0] = 0;
        break;
    }
    return 0;
}
int mpu_get_accel_fsr(unsigned char *fsr)
{
    switch (st.chip_cfg.accel_fsr) {
    case INV_FSR_2G:
        fsr[0] = 2;
        break;
    case INV_FSR_4G:
        fsr[0] = 4;
        break;
    case INV_FSR_8G:
        fsr[0] = 8;
        break;
    case INV_FSR_16G:
        fsr[0] = 16;
        break;
    default:
        return -1;
    }
    if (st.chip_cfg.accel_half)
        fsr[0] <<= 1;
    return 0;
}
int mpu_get_lpf(unsigned short *lpf)
{
    switch (st.chip_cfg.lpf) {
    case INV_FILTER_188HZ:
        lpf[0] = 188;
        break;
    case INV_FILTER_98HZ:
        lpf[0] = 98;
        break;
    case INV_FILTER_42HZ:
        lpf[0] = 42;
        break;
    case INV_FILTER_20HZ:
        lpf[0] = 20;
        break;
    case INV_FILTER_10HZ:
        lpf[0] = 10;
        break;
    case INV_FILTER_5HZ:
        lpf[0] = 5;
        break;
    case INV_FILTER_256HZ_NOLPF2:
    case INV_FILTER_2100HZ_NOLPF:
    default:
        lpf[0] = 0;
        break;
    }
    return 0;
}
int mpu_get_sample_rate(unsigned short *rate)
{
    if (st.chip_cfg.dmp_on)
        return -1;
    else
        rate[0] = st.chip_cfg.sample_rate;
    return 0;
}
int mpu_get_fifo_config(unsigned char *sensors)
{
    sensors[0] = st.chip_cfg.fifo_enable;
    return 0;
}
static int get_st_biases(long *gyro, long *accel, unsigned char hw_test)
{
    unsigned char data[MAX_PACKET_LENGTH];
    unsigned char packet_count, ii;
    unsigned short fifo_count;

    data[0] = 0x01;
    data[1] = 0;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->pwr_mgmt_1, 2, data))
        return -1;
    AnBT_DMP_Delay_ms(200);
    data[0] = 0;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->int_enable, 1, data))
        return -1;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->fifo_en, 1, data))
        return -1;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->pwr_mgmt_1, 1, data))
        return -1;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->i2c_mst, 1, data))
        return -1;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->user_ctrl, 1, data))
        return -1;
    data[0] = BIT_FIFO_RST | BIT_DMP_RST;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->user_ctrl, 1, data))
        return -1;
    AnBT_DMP_Delay_ms(15);
    data[0] = st.test->reg_lpf;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->lpf, 1, data))
        return -1;
    data[0] = st.test->reg_rate_div;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->rate_div, 1, data))
        return -1;
    if (hw_test)
        data[0] = st.test->reg_gyro_fsr | 0xE0;
    else
        data[0] = st.test->reg_gyro_fsr;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->gyro_cfg, 1, data))
        return -1;

    if (hw_test)
        data[0] = st.test->reg_accel_fsr | 0xE0;
    else
        data[0] = test.reg_accel_fsr;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->accel_cfg, 1, data))
        return -1;
    if (hw_test)
        AnBT_DMP_Delay_ms(200);

    /* Fill FIFO for test.wait_ms milliseconds. */
    data[0] = BIT_FIFO_EN;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->user_ctrl, 1, data))
        return -1;

    data[0] = INV_XYZ_GYRO | INV_XYZ_ACCEL;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->fifo_en, 1, data))
        return -1;
    AnBT_DMP_Delay_ms(test.wait_ms);
    data[0] = 0;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->fifo_en, 1, data))
        return -1;

    if (AnBT_DMP_I2C_Read(st.hw->addr, st.reg->fifo_count_h, 2, data))
        return -1;

    fifo_count = (data[0] << 8) | data[1];
    packet_count = fifo_count / MAX_PACKET_LENGTH;
    gyro[0] = gyro[1] = gyro[2] = 0;
    accel[0] = accel[1] = accel[2] = 0;

    for (ii = 0; ii < packet_count; ii++) {
        short accel_cur[3], gyro_cur[3];
        if (AnBT_DMP_I2C_Read(st.hw->addr, st.reg->fifo_r_w, MAX_PACKET_LENGTH, data))
            return -1;
        accel_cur[0] = ((short)data[0] << 8) | data[1];
        accel_cur[1] = ((short)data[2] << 8) | data[3];
        accel_cur[2] = ((short)data[4] << 8) | data[5];
        accel[0] += (long)accel_cur[0];
        accel[1] += (long)accel_cur[1];
        accel[2] += (long)accel_cur[2];
        gyro_cur[0] = (((short)data[6] << 8) | data[7]);
        gyro_cur[1] = (((short)data[8] << 8) | data[9]);
        gyro_cur[2] = (((short)data[10] << 8) | data[11]);
        gyro[0] += (long)gyro_cur[0];
        gyro[1] += (long)gyro_cur[1];
        gyro[2] += (long)gyro_cur[2];
    }

    gyro[0] = (long)(((long long)gyro[0]<<16) / test.gyro_sens / packet_count);
    gyro[1] = (long)(((long long)gyro[1]<<16) / test.gyro_sens / packet_count);
    gyro[2] = (long)(((long long)gyro[2]<<16) / test.gyro_sens / packet_count);
    accel[0] = (long)(((long long)accel[0]<<16) / test.accel_sens /
        packet_count);
    accel[1] = (long)(((long long)accel[1]<<16) / test.accel_sens /
        packet_count);
    accel[2] = (long)(((long long)accel[2]<<16) / test.accel_sens /
        packet_count);
    /* Don't remove gravity! */
    if (accel[2] > 0L)
        accel[2] -= 65536L;
    else
        accel[2] += 65536L;

    return 0;
}
static int get_accel_prod_shift(float *st_shift)
{
    unsigned char tmp[4], shift_code[3], ii;

    if (AnBT_DMP_I2C_Read(st.hw->addr, 0x0D, 4, tmp))
        return 0x07;

    shift_code[0] = ((tmp[0] & 0xE0) >> 3) | ((tmp[3] & 0x30) >> 4);
    shift_code[1] = ((tmp[1] & 0xE0) >> 3) | ((tmp[3] & 0x0C) >> 2);
    shift_code[2] = ((tmp[2] & 0xE0) >> 3) | (tmp[3] & 0x03);
    for (ii = 0; ii < 3; ii++) {
        if (!shift_code[ii]) {
            st_shift[ii] = 0.f;
            continue;
        }
        /* Equivalent to..
         * st_shift[ii] = 0.34f * powf(0.92f/0.34f, (shift_code[ii]-1) / 30.f)
         */
        st_shift[ii] = 0.34f;
        while (--shift_code[ii])
            st_shift[ii] *= 1.034f;
    }
    return 0;
}
int mpu_get_gyro_sens(float *sens)
{
    switch (st.chip_cfg.gyro_fsr) {
    case INV_FSR_250DPS:
        sens[0] = 131.f;
        break;
    case INV_FSR_500DPS:
        sens[0] = 65.5f;
        break;
    case INV_FSR_1000DPS:
        sens[0] = 32.8f;
        break;
    case INV_FSR_2000DPS:
        sens[0] = 16.4f;
        break;
    default:
        return -1;
    }
    return 0;
}
int mpu_get_accel_sens(unsigned short *sens)
{
    switch (st.chip_cfg.accel_fsr) {
    case INV_FSR_2G:
        sens[0] = 16384;
        break;
    case INV_FSR_4G:
        sens[0] = 8092;
        break;
    case INV_FSR_8G:
        sens[0] = 4096;
        break;
    case INV_FSR_16G:
        sens[0] = 2048;
        break;
    default:
        return -1;
    }
    if (st.chip_cfg.accel_half)
        sens[0] >>= 1;
    return 0;
}
//
static int accel_self_test(long *bias_regular, long *bias_st)
{
    int jj, result = 0;
    float st_shift[3], st_shift_cust, st_shift_var;

    get_accel_prod_shift(st_shift);
    for(jj = 0; jj < 3; jj++) {
        st_shift_cust = labs(bias_regular[jj] - bias_st[jj]) / 65536.f;
        if (st_shift[jj]) {
            st_shift_var = st_shift_cust / st_shift[jj] - 1.f;
            if (fabs(st_shift_var) > test.max_accel_var)
                result |= 1 << jj;
        } else if ((st_shift_cust < test.min_g) ||
            (st_shift_cust > test.max_g))
            result |= 1 << jj;
    }

    return result;
}
static int gyro_self_test(long *bias_regular, long *bias_st)
{
    int jj, result = 0;
    unsigned char tmp[3];
    float st_shift, st_shift_cust, st_shift_var;

    if (AnBT_DMP_I2C_Read(st.hw->addr, 0x0D, 3, tmp))
        return 0x07;

    tmp[0] &= 0x1F;
    tmp[1] &= 0x1F;
    tmp[2] &= 0x1F;

    for (jj = 0; jj < 3; jj++) {
        st_shift_cust = labs(bias_regular[jj] - bias_st[jj]) / 65536.f;
        if (tmp[jj]) {
            st_shift = 3275.f / test.gyro_sens;
            while (--tmp[jj])
                st_shift *= 1.046f;
            st_shift_var = st_shift_cust / st_shift - 1.f;
            if (fabs(st_shift_var) > test.max_gyro_var)
                result |= 1 << jj;
        } else if ((st_shift_cust < test.min_dps) ||
            (st_shift_cust > test.max_dps))
            result |= 1 << jj;
    }
    return result;
}
int mpu_run_self_test(long *gyro, long *accel)
{
    const unsigned char tries = 2;
    long gyro_st[3], accel_st[3];
    unsigned char accel_result, gyro_result;
    int ii;
    int result;
    unsigned char accel_fsr, fifo_sensors, sensors_on;
    unsigned short gyro_fsr, sample_rate, lpf;
    unsigned char dmp_was_on;

    if (st.chip_cfg.dmp_on) {
        mpu_set_dmp_state(0);
        dmp_was_on = 1;
    } else
        dmp_was_on = 0;

    /* Get initial settings. */
    mpu_get_gyro_fsr(&gyro_fsr);
    mpu_get_accel_fsr(&accel_fsr);
    mpu_get_lpf(&lpf);
    mpu_get_sample_rate(&sample_rate);
    sensors_on = st.chip_cfg.sensors;
    mpu_get_fifo_config(&fifo_sensors);

    /* For older chips, the self-test will be different. */
    for (ii = 0; ii < tries; ii++)
        if (!get_st_biases(gyro, accel, 0))
            break;
    if (ii == tries) {
        /* If we reach this point, we most likely encountered an I2C error.
         * We'll just report an error for all three sensors.
         */
        result = 0;
        goto restore;
    }
    for (ii = 0; ii < tries; ii++)
        if (!get_st_biases(gyro_st, accel_st, 1))
            break;
    if (ii == tries) {
        /* Again, probably an I2C error. */
        result = 0;
        goto restore;
    }
    accel_result = accel_self_test(accel, accel_st);
    gyro_result = gyro_self_test(gyro, gyro_st);

    result = 0;
    if (!gyro_result)
        result |= 0x01;
    if (!accel_result)
        result |= 0x02;

restore:

    /* Set to invalid values to ensure no I2C writes are skipped. */
    st.chip_cfg.gyro_fsr = 0xFF;
    st.chip_cfg.accel_fsr = 0xFF;
    st.chip_cfg.lpf = 0xFF;
    st.chip_cfg.sample_rate = 0xFFFF;
    st.chip_cfg.sensors = 0xFF;
    st.chip_cfg.fifo_enable = 0xFF;
    st.chip_cfg.clk_src = INV_CLK_PLL;
    mpu_set_gyro_fsr(gyro_fsr);
    mpu_set_accel_fsr(accel_fsr);
    mpu_set_lpf(lpf);
    mpu_set_sample_rate(sample_rate);
    mpu_set_sensors(sensors_on);
    mpu_configure_fifo(fifo_sensors);

    if (dmp_was_on) mpu_set_dmp_state(1);
        
    return result;
}
void run_self_test(char cal)
{
  int result;
	float sens;
	unsigned short accel_sens;
  long gyro_test[3]={0,0,0}, accel_test[3]={0,0,0};
	//
	if(cal==1)
	{	
		result=	  mpu_run_self_test(gyro_test, accel_test) ;
		if( result == 0x3 )
		{
// 			Flash_Save_GyroOffset(gyro_test);//保存校准数值
// 			Flash_Save_AccOffset(accel_test); //保存校准数值
		}
	}
// 	Flash_Read_GyroOffset(gyro_test);//从Flash读出校准数值
// 	Flash_Read_AccOffset(accel_test); //从Flash读出校准数值

	mpu_get_gyro_sens(&sens);
	gyro_test[0] = (long)(gyro_test[0] * sens);
	gyro_test[1] = (long)(gyro_test[1] * sens);
	gyro_test[2] = (long)(gyro_test[2] * sens);
	dmp_set_gyro_bias(gyro_test);
	mpu_get_accel_sens(&accel_sens);
	accel_test[0] *= accel_sens;
	accel_test[1] *= accel_sens;
	accel_test[2] *= accel_sens;
	dmp_set_accel_bias(accel_test);
	/* Test passed. We can trust the gyro data here, so let's push it down
	 * to the DMP.
	 */
	

}
//
int mpu_write_mem(unsigned short mem_addr, unsigned short length, unsigned char *data)
{
    unsigned char tmp[2];

    if (!data)
        return -1;
    if (!st.chip_cfg.sensors)
        return -1;

    tmp[0] = (unsigned char)(mem_addr >> 8);
    tmp[1] = (unsigned char)(mem_addr & 0xFF);

    /* Check bank boundaries. */
    if (tmp[1] + length > st.hw->bank_size)
        return -1;

    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->bank_sel, 2, tmp))
        return -1;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->mem_r_w, length, data))
        return -1;
    return 0;
}
int mpu_read_mem(unsigned short mem_addr, unsigned short length,  unsigned char *data)
{
    unsigned char tmp[2];

    if (!data)
        return -1;
    if (!st.chip_cfg.sensors)
        return -1;

    tmp[0] = (unsigned char)(mem_addr >> 8);
    tmp[1] = (unsigned char)(mem_addr & 0xFF);

    /* Check bank boundaries. */
    if (tmp[1] + length > st.hw->bank_size)
        return -1;

    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->bank_sel, 2, tmp))
        return -1;
    if (AnBT_DMP_I2C_Read(st.hw->addr, st.reg->mem_r_w, length, data))
        return -1;
    return 0;
}
int mpu_read_fifo_stream(unsigned short length, unsigned char *data, unsigned char *more)
{
    unsigned char tmp[2];
    unsigned short fifo_count;
    if (!st.chip_cfg.dmp_on)
        return -1;
    if (!st.chip_cfg.sensors)
        return -1;

    if (AnBT_DMP_I2C_Read(st.hw->addr, st.reg->fifo_count_h, 2, tmp))
        return -1;
    fifo_count = (tmp[0] << 8) | tmp[1];
    if (fifo_count < length) {
        more[0] = 0;
        return -1;
    }
    if (fifo_count > (st.hw->max_fifo >> 1)) {
        /* FIFO is 50% full, better check overflow bit. */
        if (AnBT_DMP_I2C_Read(st.hw->addr, st.reg->int_status, 1, tmp))
            return -1;
        if (tmp[0] & BIT_FIFO_OVERFLOW) {
            mpu_reset_fifo();
            return -2;
        }
    }

    if (AnBT_DMP_I2C_Read(st.hw->addr, st.reg->fifo_r_w, length, data))
        return -1;
    more[0] = fifo_count / length - 1;
    return 0;
}
int mpu_load_firmware(unsigned short length, const unsigned char *firmware,unsigned short start_addr, unsigned short sample_rate)
{
    unsigned short ii;
    unsigned short this_write;
    /* Must divide evenly into st.hw->bank_size to avoid bank crossings. */
#define LOAD_CHUNK  (16)
    unsigned char cur[LOAD_CHUNK], tmp[2];

    if (st.chip_cfg.dmp_loaded)
        /* DMP should only be loaded once. */
        return -1;

    if (!firmware)
        return -1;
    for (ii = 0; ii < length; ii += this_write) {
        this_write = min(LOAD_CHUNK, length - ii);
        if (mpu_write_mem(ii, this_write, (unsigned char*)&firmware[ii]))
            return -1;
        if (mpu_read_mem(ii, this_write, cur))
            return -1;
        if (memcmp(firmware+ii, cur, this_write))
            return -2;
    }

    /* Set program start address. */
    tmp[0] = start_addr >> 8;
    tmp[1] = start_addr & 0xFF;
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->prgm_start_h, 2, tmp))
        return -1;

    st.chip_cfg.dmp_loaded = 1;
    st.chip_cfg.dmp_sample_rate = sample_rate;
    return 0;
}

static  unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;
	//
    if (row[0] > 0)    b = 0;
    else if (row[0] < 0)  b = 4;
    else if (row[1] > 0)   b = 1;
    else if (row[1] < 0)   b = 5;
    else if (row[2] > 0)       b = 2;
    else if (row[2] < 0)   b = 6;
    else         b = 7;      // error
    return b;
}

static  unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;
    return scalar;
}

//
u8 AnBT_DMP_MPU6050_DEV_CFG(void)		
{
    unsigned char anbt_dmp_data[6], anbt_dmp_rev;
    //检验ＷＨＯ　ＡＭ　Ｉ
    if (AnBT_DMP_I2C_Read(st.hw->addr, st.reg->who_am_i, 1, &(anbt_dmp_data[0])))
        return 1;   
       
    if (anbt_dmp_data[0]!=PRODUCT_WHOAMI) 
    {
        //AnBT_Uart1_Send_String("WHO AM I error",14); 
        //AnBT_Uart1_Send_Num(anbt_dmp_data[0]);
        return 2;    
    }
    
    anbt_dmp_data[0] = 0x80;	//etootle: BIT_RESET, Reset device			重设器件
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->pwr_mgmt_1, 1, &(anbt_dmp_data[0])))
			return 3;   
    AnBT_DMP_Delay_ms(100);
    anbt_dmp_data[0] = 0x00; //etootle: Wake up chip			唤醒
    if (AnBT_DMP_I2C_Write(st.hw->addr, st.reg->pwr_mgmt_1, 1, &(anbt_dmp_data[0])))
			return 4;
    if (AnBT_DMP_I2C_Read(st.hw->addr, st.reg->accel_offs, 6, anbt_dmp_data))
			return 5;	//etootle:Check product revision  		检查产品修正
		
    anbt_dmp_rev = ((anbt_dmp_data[5] & 0x01) << 2) | ((anbt_dmp_data[3] & 0x01) << 1) | (anbt_dmp_data[1] & 0x01);
    if (anbt_dmp_rev)  //Congrats, these parts are better
		{
		        if (anbt_dmp_rev == 1) st.chip_cfg.accel_half = 1;   
        else    if (anbt_dmp_rev == 2) st.chip_cfg.accel_half = 0;   
        else 
			{
			//AnBT_Uart1_Send_String("Unsupported version=1.0",23);  
            return 6;
			}
    } 
		else 
		{
			if (AnBT_DMP_I2C_Read(st.hw->addr, st.reg->prod_id, 1, &(anbt_dmp_data[0]))) return 7;      
            anbt_dmp_rev = anbt_dmp_data[0] & 0x0F;
            if (!anbt_dmp_rev) 
			{
				//AnBT_Uart1_Send_String("Product ID error",16); 		//产品编号错误
				return 5;    
            } 
			else if (anbt_dmp_rev == 4) 
			{
				//AnBT_Uart1_Send_String("Half Sensitivity Part",21); 
                st.chip_cfg.accel_half = 1;
            } 
			else
			{
				st.chip_cfg.accel_half = 0;
			}
    }
    
    // Set to invalid values to ensure no I2C writes are skipped. 
    st.chip_cfg.sensors = 0xFF;
    st.chip_cfg.gyro_fsr = 0xFF;
    st.chip_cfg.accel_fsr = 0xFF;
    st.chip_cfg.lpf = 0xFF;
    st.chip_cfg.sample_rate = 0xFFFF;
    st.chip_cfg.fifo_enable = 0xFF;
    st.chip_cfg.bypass_mode = 0xFF;

    // mpu_set_sensors always preserves this setting. 
    st.chip_cfg.clk_src = INV_CLK_PLL;
    // Handled in next call to mpu_set_bypass. 
    st.chip_cfg.active_low_int = 1;
    st.chip_cfg.latched_int = 0;
    st.chip_cfg.int_motion_only = 0;
    st.chip_cfg.lp_accel_mode = 0;
    memset(&st.chip_cfg.cache, 0, sizeof(st.chip_cfg.cache));
    st.chip_cfg.dmp_on = 0;
    st.chip_cfg.dmp_loaded = 0;
    st.chip_cfg.dmp_sample_rate = 0;

    if (mpu_set_gyro_fsr(2000)) return 0x10;
    if (mpu_set_accel_fsr(2)) return 0x11;   
    if (mpu_set_lpf(42)) return 0x12; 
    if (mpu_set_sample_rate(50)) return 0x13;
    if (mpu_configure_fifo(0)) return 0x14;
        
    // Already disabled by setup_compass. 
    if (mpu_set_bypass(0)) return 0x15;					//不同点
		
    mpu_set_sensors(0);
    return 0;
}

//把数转化为角速度(角度每秒)
float number_to_dps(s16 gyro_number)
{
	float gyro_temp;
	gyro_temp=(float)(gyro_number);
	gyro_temp=gyro_temp*0.0061f;
	return gyro_temp;
}

