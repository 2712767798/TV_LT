#include "CST816.h"
#include "math.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "driver/gpio.h"

static const char *TAG = "i2c-simple-example";

#define I2C_MASTER_SCL_IO 8			/*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO 19		/*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM 0			/*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
									// I2C主I2C端口号，可用的I2C外设接口数量将取决于芯片
#define I2C_MASTER_FREQ_HZ 400000	/*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS 1000

esp_err_t CST816_Init(void)
{
	int i2c_master_port = I2C_MASTER_NUM;

	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = I2C_MASTER_SDA_IO,
		.scl_io_num = I2C_MASTER_SCL_IO,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = I2C_MASTER_FREQ_HZ,
	};

	i2c_param_config(i2c_master_port, &conf);
	gpio_config_t cfg = {
		.pin_bit_mask = 3,
		.mode = GPIO_MODE_INPUT,
		.pull_up_en = false,
		.pull_down_en = false,
		.intr_type = GPIO_INTR_DISABLE,
	};
	gpio_config(&cfg);
	i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
	CST816_Get_ChipID();
	//  CST816_Set_AutoRST(1);
	return ESP_OK;
}

void CST816_IIC_WriteREG(unsigned char reg, unsigned char date)
{
	// CST816_IIC_SendByte(0x2A);
	// CST816_IIC_SendByte(reg);
	// CST816_IIC_SendByte(date);
	uint8_t write_buf[2] = {0};
	write_buf[0] = reg;
	write_buf[1] = date;
	i2c_master_write_to_device(I2C_MASTER_NUM, CST816_ADDR, &write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
	// ESP_LOGI("wri", "is %d\r\n", sizeof(write_buf));
}

unsigned char CST816_Get_ChipID()
{
	uint8_t sbuff = ChipID;
	i2c_master_write_read_device(I2C_MASTER_NUM, CST816_ADDR, &sbuff, 1, &sbuff, 1, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
	ESP_LOGI("id", "is %d\r\n", sbuff);
	return sbuff;
}

void CST816_Get_XY(int16_t *x, int16_t *y)
{
	unsigned char temp[4], csxy = 0x03;
	int16_t x2, y2;

	i2c_master_write_read_device(I2C_MASTER_NUM, CST816_ADDR, &csxy, 1, temp, 4, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);

	y2 = (unsigned int)((temp[0] & 0x0F) << 8) | temp[1]; //(temp[0]&0X0F)<<4|
	x2 = (unsigned int)((temp[2] & 0x0F) << 8) | temp[3]; //(temp[2]&0X0F)<<4|
	// ESP_LOGI("xy", "x2 is %d\r\n", x2);
	// ESP_LOGI("xy", "y2 is %d\r\n", y2);
	if (1 /* x2 < 240 && y2 < 280 */)
	{
		*x = x2;
		*y = 240 - y2;
	}
	// ESP_LOGI("xy", "x is %d\r\n", *x);
}

bool CST816_Get_Sta()
{
	unsigned char sta, temp[2] = {0x02, 0x02};
	i2c_master_write_read_device(I2C_MASTER_NUM, CST816_ADDR, &temp, 1, temp, 1, I2C_MASTER_TIMEOUT_MS / portTICK_RATE_MS);
	sta = temp[0];
	if (sta != 255 && sta != 0)
	{
		// ESP_LOGI("iic", "iic %d\r\n", sta);
		return true;
	}
	else
	{
		// ESP_LOGI("iic", "iic %d\r\n", sta);
		return false;
	}
	//	return CST816_IIC_ReadREG(FingerNum);
	// if (!gpio_get_level(3))
	// {

	// 	return true;
	// }
	// else
	// {
	// 	// ESP_LOGI("gpio", "gpio %d\r\n", gpio_get_level(3));
	// 	return false;
	// }
}

void CST816_Set_MotionMask(unsigned char Motion)
{
	CST816_IIC_WriteREG(MotionMask, Motion);
}

void CST816_Set_IrqCtrl(unsigned char IRQCtrl)
{
	CST816_IIC_WriteREG(IrqCrl, IRQCtrl);
}

void CST816_Set_AutoSleep(unsigned char en) //是否使能自动进入低功耗模式
{
	if (en == FALSE)
	{
		CST816_IIC_WriteREG(DisAutoSleep, FALSE);
	}
	else
	{
		CST816_IIC_WriteREG(DisAutoSleep, ENABLE);
		CST816_IIC_WriteREG(AutoSleepTime, en);
	}
}
void CST816_Set_AutoRST(unsigned char time) //设置多长时间无触摸自动复位，默认5s,写0则禁用功能
{
	CST816_IIC_WriteREG(AutoReset, time);
}
void CST816_Set_LongPressRST(unsigned char time) //设置长按多长时间复位，默认为10s，写0禁用功能
{
	CST816_IIC_WriteREG(LongPressTime, time);
}
