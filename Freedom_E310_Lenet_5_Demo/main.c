#include <stdio.h>
#include <metal/machine.h>
#include <metal/led.h>
#include <string.h>
#include "main.h"

#define DEBUG 1U

#define SDA 12
#define SCL 13
 
#define LONG_DELAY  25 //25 // Equals ~400KHz
#define SHORT_DELAY 5 //5

#define OV2640_I2C_ADDR 0x30 // 7 bit
#define OV2640_PIDH 0x0A
#define OV2640_PIDL 0x0B

#define OV2640_SPI_FRMCTRL 0x01
#define OV2640_SPI_MODE 0x02

#define OV2640_SPI_FIFO 0x04
#define OV2640_SPI_VERSION 0x40

#define SKIP_BYTES 8

const uint8_t OV2640_QVGA[][2] =
{
	{0xff, 0x0}, 
	{0x2c, 0xff}, 
	{0x2e, 0xdf}, 
	{0xff, 0x1}, 
	{0x3c, 0x32}, 
	{0x11, 0x0}, 
	{0x9, 0x2}, 
	{0x4, 0x28}, //0xa8
	{0x13, 0xe5}, 
	{0x14, 0x48}, 
	{0x2c, 0xc}, 
	{0x33, 0x78}, 
	{0x3a, 0x33}, 
	{0x3b, 0xfb}, 
	{0x3e, 0x0}, 
	{0x43, 0x11}, 
	{0x16, 0x10}, 
	{0x39, 0x2}, 
	{0x35, 0x88}, 

	{0x22, 0xa}, 
	{0x37, 0x40}, 
	{0x23, 0x0}, 
	{0x34, 0xa0}, 
	{0x6, 0x2}, 
	{0x6, 0x88}, 
	{0x7, 0xc0}, 
	{0xd, 0xb7}, 
	{0xe, 0x1}, 
	{0x4c, 0x0}, 
	{0x4a, 0x81}, 
	{0x21, 0x99}, 
	{0x24, 0x40}, 
	{0x25, 0x38}, 
	{0x26, 0x82}, 
	{0x5c, 0x0}, 
	{0x63, 0x0}, 
	{0x46, 0x22}, 
	{0xc, 0x3a}, 
	{0x5d, 0x55}, 
	{0x5e, 0x7d}, 
	{0x5f, 0x7d}, 
	{0x60, 0x55}, 
	{0x61, 0x70}, 
	{0x62, 0x80}, 
	{0x7c, 0x5}, 
	{0x20, 0x80}, 
	{0x28, 0x30}, 
	{0x6c, 0x0}, 
	{0x6d, 0x80}, 
	{0x6e, 0x0}, 
	{0x70, 0x2}, 
	{0x71, 0x94}, 
	{0x73, 0xc1}, 
	{0x3d, 0x34}, 
	{0x12, 0x4}, 
	{0x5a, 0x57}, 
	{0x4f, 0xbb}, 
	{0x50, 0x9c}, 
	{0xff, 0x0}, 
	{0xe5, 0x7f}, 
	{0xf9, 0xc0}, 
	{0x41, 0x24}, 
	{0xe0, 0x14}, 
	{0x76, 0xff}, 
	{0x33, 0xa0}, 
	{0x42, 0x20}, 
	{0x43, 0x18}, 
	{0x4c, 0x0}, 
	{0x87, 0xd0}, 
	{0x88, 0x3f}, 
	{0xd7, 0x3}, 
	{0xd9, 0x10}, 
	{0xd3, 0x82}, 
	{0xc8, 0x8}, 
	{0xc9, 0x80}, 
	{0x7c, 0x0}, 
	{0x7d, 0x0}, 
	{0x7c, 0x3}, 
	{0x7d, 0x48}, 
	{0x7d, 0x48}, 
	{0x7c, 0x8}, 
	{0x7d, 0x20}, 
	{0x7d, 0x10}, 
	{0x7d, 0xe}, 
	{0x90, 0x0}, 
	{0x91, 0xe}, 
	{0x91, 0x1a}, 
	{0x91, 0x31}, 
	{0x91, 0x5a}, 
	{0x91, 0x69}, 
	{0x91, 0x75}, 
	{0x91, 0x7e}, 
	{0x91, 0x88}, 
	{0x91, 0x8f}, 
	{0x91, 0x96}, 
	{0x91, 0xa3}, 
	{0x91, 0xaf}, 
	{0x91, 0xc4}, 
	{0x91, 0xd7}, 
	{0x91, 0xe8}, 
	{0x91, 0x20}, 
	{0x92, 0x0}, 

	{0x93, 0x6}, 
	{0x93, 0xe3}, 
	{0x93, 0x3}, 
	{0x93, 0x3}, 
	{0x93, 0x0}, 
	{0x93, 0x2}, 
	{0x93, 0x0}, 
	{0x93, 0x0}, 
	{0x93, 0x0}, 
	{0x93, 0x0}, 
	{0x93, 0x0}, 
	{0x93, 0x0}, 
	{0x93, 0x0}, 
	{0x96, 0x0}, 
	{0x97, 0x8}, 
	{0x97, 0x19}, 
	{0x97, 0x2}, 
	{0x97, 0xc}, 
	{0x97, 0x24}, 
	{0x97, 0x30}, 
	{0x97, 0x28}, 
	{0x97, 0x26}, 
	{0x97, 0x2}, 
	{0x97, 0x98}, 
	{0x97, 0x80}, 
	{0x97, 0x0}, 
	{0x97, 0x0}, 
	{0xa4, 0x0}, 
	{0xa8, 0x0}, 
	{0xc5, 0x11}, 
	{0xc6, 0x51}, 
	{0xbf, 0x80}, 
	{0xc7, 0x10}, 
	{0xb6, 0x66}, 
	{0xb8, 0xa5}, 
	{0xb7, 0x64}, 
	{0xb9, 0x7c}, 
	{0xb3, 0xaf}, 
	{0xb4, 0x97}, 
	{0xb5, 0xff}, 
	{0xb0, 0xc5}, 
	{0xb1, 0x94}, 
	{0xb2, 0xf}, 
	{0xc4, 0x5c}, 
	{0xa6, 0x0}, 
	{0xa7, 0x20}, 
	{0xa7, 0xd8}, 
	{0xa7, 0x1b}, 
	{0xa7, 0x31}, 
	{0xa7, 0x0}, 
	{0xa7, 0x18}, 
	{0xa7, 0x20}, 
	{0xa7, 0xd8}, 
	{0xa7, 0x19}, 
	{0xa7, 0x31}, 
	{0xa7, 0x0}, 
	{0xa7, 0x18}, 
	{0xa7, 0x20}, 
	{0xa7, 0xd8}, 
	{0xa7, 0x19}, 
	{0xa7, 0x31}, 
	{0xa7, 0x0}, 
	{0xa7, 0x18}, 
	{0x7f, 0x0}, 
	{0xe5, 0x1f}, 
	{0xe1, 0x77}, 
	{0xdd, 0x7f}, 
	{0xc2, 0xe}, 
	
	{0xff, 0x0}, 
	{0xe0, 0x4}, 
	{0xc0, 0xc8}, 
	{0xc1, 0x96}, 
	{0x86, 0x3d}, 
	{0x51, 0x90}, 
	{0x52, 0x2c}, 
	{0x53, 0x0}, 
	{0x54, 0x0}, 
	{0x55, 0x88}, 
	{0x57, 0x0}, 
	
	{0x50, 0x92}, 
	{0x5a, 0x10}, //0x50 
	{0x5b, 0x10}, //0x3c
	{0x5c, 0x0}, 
	{0xd3, 0x4}, 
	{0xe0, 0x0}, 
	
	{0xff, 0x0}, 
	{0x5, 0x0}, 
	
	{0xda, 0x40}, // 0x8 for RGB565
	{0xd7, 0x3}, 
	{0xe0, 0x0}, 
	
	{0x5, 0x0}, 
	
	{0xff,0xff}
};

// data_t arr[32] __attribute__ ((section ("customdata"))) = { 4 };

void print_twoD(twoD_t *tmp, uint32_t channel, float scale)
{
#if (DEBUG)
	printf("Size: %uX%uX%u\n", tmp->channel, tmp->r, tmp->c);
	for (uint32_t i = 0; i < tmp->r; i++)
	{
		for (uint32_t j = 0; j < tmp->c; j++)
		{
			printf("%03d ", (int32_t)(tmp->data[channel * tmp->c * tmp->r + i * tmp->c + j]*scale));
		}
		printf("\n");
	}
#endif
}

// Choose stride>c=r to reuse existing buffer again.
void maxPooling(twoD_t *inout, uint32_t r, uint32_t c, uint32_t stride)
{
	data_t max = 0;

	for (uint32_t i = 0; i < inout->r;  i+=stride)
	{
		for (uint32_t j = 0; j < inout->c;  j+=stride)
		{
			max = 0;
			for (uint32_t ii = 0; ii < r;  ii++)
			{
				for (uint32_t jj = 0; jj < c;  jj++)
				{
					if (inout->data[(i+ii) * inout->c + (j+jj)] > max)
					{
						max = inout->data[(i+ii) * inout->c + (j+jj)];
					}
				}
			}
			inout->data[i/stride * inout->r/stride + j/stride] = max;
		}
	}
}

void dot(twoD_t *input, const twoD_t *weights, twoD_t *output, data_t (*activation)(data_t))
{
	if (weights->r != input->r * input->c * input->channel)
	{
		printf("size mismatch\n");
	}

	output->r = weights->c;
	output->c = 1;

	data_t sum;

	for (uint32_t i = 0; i < weights->c;  i++)
	{
		sum = 0;
		for (uint32_t j = 0; j < weights->r;  j++)
		{
			sum += input->data[j] * weights->data[i * weights->r + j];
		}

		if (weights->bias)
		{
			sum += weights->bias[i];
		}

		if (activation)
		{
			sum = activation(sum);
		}
        
        output->data[i] = sum;
	}
}

data_t reLU(data_t a)
{
	return a > 0 ? a : 0;
}

// only works when stride = r = c
void conv2D(twoD_t *input, const twoD_t *kernel, twoD_t *output, data_t (*activation)(data_t), uint32_t stride, uint32_t r, uint32_t c)
{
	uint32_t aux_r = input->r - kernel->r + 1;
	uint32_t aux_c = input->c - kernel->c + 1;
	data_t aux_sum;
	data_t max_pool;

	output->r = aux_r / stride;
	output->c = aux_c / stride;

	for (uint32_t out_ch = 0; out_ch < output->channel; out_ch++)
	{
		for (uint32_t i = 0; i < aux_r;  i+=stride)
		{
			for (uint32_t j = 0; j < aux_c;  j+=stride)
			{
				max_pool = 0; /* it is safe because post ReLU values are being compared */
				for (uint32_t i_pool = 0; i_pool < r;  i_pool++)
				{
					for (uint32_t j_pool = 0; j_pool < c;  j_pool++)
					{
						aux_sum = 0;
						for (uint32_t ii = 0; ii < kernel->r;  ii++)
						{
							for (uint32_t jj = 0; jj < kernel->c;  jj++)
							{
								for (uint32_t in_ch = 0; in_ch < input->channel;  in_ch++)
								{
									aux_sum += input->data[in_ch * input->r * input->c + (i+i_pool+ii) * input->c + (j+j_pool+jj)] * kernel->data[out_ch * kernel->in_channel * kernel->r * kernel->c + in_ch * kernel->r * kernel->c + ii * kernel->c + jj];
								}
							}
						}
						if (kernel->bias)
						{
							aux_sum += kernel->bias[out_ch];
						}

						if (activation)
						{
							aux_sum = activation(aux_sum);
						}

						if (aux_sum > max_pool)
						{
							max_pool = aux_sum;
						}
					}
				}
				output->data[out_ch * output->r * output->c  + i/stride * output->c + j/stride] = max_pool;
			}
		}
	}
}

uint32_t get_class(const twoD_t *output)
{
	uint32_t max_inx = 0;
	for (uint32_t i = 0; i < output->r; i++)
	{
		if (output->data[i] > output->data[max_inx])
		{
			max_inx = i;
		}
	}
	return max_inx;
}

void basic_delay(uint32_t amount)
{
	for(volatile uint32_t i = 0; i < amount; i++)
		__asm__("nop");
}

void i2c_init(struct metal_gpio *gpio_device)
{
	metal_gpio_disable_output(gpio_device, SDA);
	metal_gpio_enable_input(gpio_device, SDA);
	metal_gpio_disable_output(gpio_device, SCL);
	metal_gpio_enable_input(gpio_device, SCL);
}

void i2c_start(struct metal_gpio *gpio_device)
{
	metal_gpio_disable_input(gpio_device, SCL);
	metal_gpio_disable_input(gpio_device, SDA);

	metal_gpio_set_pin(gpio_device, SDA, 0);
	metal_gpio_enable_output(gpio_device, SDA);
	basic_delay(SHORT_DELAY);
	metal_gpio_set_pin(gpio_device, SCL, 0);
	metal_gpio_enable_output(gpio_device, SCL);
}

_Bool i2c_busy(struct metal_gpio *gpio_device)
{
	metal_gpio_disable_output(gpio_device, SCL);
	metal_gpio_enable_input(gpio_device, SCL);
	metal_gpio_disable_output(gpio_device, SDA);
	metal_gpio_enable_input(gpio_device, SDA);
	return !(metal_gpio_get_input_pin(gpio_device, SDA) && metal_gpio_get_input_pin(gpio_device, SCL));
}

void i2c_stop(struct metal_gpio *gpio_device)
{
	metal_gpio_disable_output(gpio_device, SCL);
	metal_gpio_enable_input(gpio_device, SCL);
	basic_delay(SHORT_DELAY);
	metal_gpio_disable_output(gpio_device, SDA);
	metal_gpio_enable_input(gpio_device, SDA);
}

void i2c_send_bytes_rs(struct metal_gpio *gpio_device, uint8_t* data, uint8_t size, _Bool rs, uint8_t* data_r, uint8_t size_r)
{
  uint8_t address_r = data[0] | 0x01;

  for(uint8_t ii=0; ii<size; ii++)
  {
	metal_gpio_disable_input(gpio_device, SDA);
	metal_gpio_enable_output(gpio_device, SDA);
    for(uint8_t i=0; i<9; i++)
    {
	  basic_delay(SHORT_DELAY);

      if(i != 8)
      {
        if(data[ii] & 0x80)
			metal_gpio_set_pin(gpio_device, SDA, 1);
        else
			metal_gpio_set_pin(gpio_device, SDA, 0);
      }
      else
      {
		metal_gpio_disable_output(gpio_device, SDA);
		metal_gpio_enable_input(gpio_device, SDA);
        if(metal_gpio_get_input_pin(gpio_device, SDA))
        {
            printf("NO ACK\n");
        }

		metal_gpio_disable_input(gpio_device, SDA);
		metal_gpio_enable_output(gpio_device, SDA);
        if(rs == 0)
        {
          metal_gpio_set_pin(gpio_device, SDA, 0);
        }
        else
        {
          metal_gpio_set_pin(gpio_device, SDA, 1);
        }

      }
      basic_delay(LONG_DELAY-SHORT_DELAY);

      metal_gpio_set_pin(gpio_device, SCL, 1);

      basic_delay(LONG_DELAY);

      metal_gpio_set_pin(gpio_device, SCL, 0);

      data[ii] <<= 1;
    }
  }

  if(rs == 0)
    return;

  metal_gpio_set_pin(gpio_device, SCL, 1);

  basic_delay(SHORT_DELAY);

  metal_gpio_set_pin(gpio_device, SDA, 0);

  basic_delay(SHORT_DELAY);

  metal_gpio_set_pin(gpio_device, SCL, 0);

  for(uint8_t i=0; i<9; i++)
  {
    basic_delay(SHORT_DELAY);
    if(i != 8)
    {
      if(address_r & 0x80)
        metal_gpio_set_pin(gpio_device, SDA, 1);
      else
        metal_gpio_set_pin(gpio_device, SDA, 0);
    }
    else
    {
	  metal_gpio_disable_output(gpio_device, SDA);
	  metal_gpio_enable_input(gpio_device, SDA);
      if(metal_gpio_get_input_pin(gpio_device, SDA))
      {
          printf("NO ACK\n");
      }
    }
    basic_delay(LONG_DELAY-SHORT_DELAY);

    metal_gpio_set_pin(gpio_device, SCL, 1);

    basic_delay(LONG_DELAY);

    metal_gpio_set_pin(gpio_device, SCL, 0);

    address_r <<= 1;
  }

  // Receive Data
  for(uint8_t ii=0; ii<size_r; ii++)
  {
    for(uint8_t i=0; i<9; i++)
    {
      metal_gpio_set_pin(gpio_device, SCL, 1);

      if(i!=8)
        data_r[ii] <<= 1;

      basic_delay(SHORT_DELAY);

      if(i!=8)
      {
        if(metal_gpio_get_input_pin(gpio_device, SDA))
        {
          data_r[ii] |= 0x1;
        }
      }
      else if(ii == (size_r-1))
      {
		metal_gpio_disable_input(gpio_device, SDA);
		metal_gpio_enable_output(gpio_device, SDA);
        metal_gpio_set_pin(gpio_device, SDA, 0);
      } else {

      }

      basic_delay(LONG_DELAY-SHORT_DELAY);

      metal_gpio_set_pin(gpio_device, SCL, 0);

      if((i==7) && (ii != (size_r-1))){
		metal_gpio_disable_input(gpio_device, SDA);
		metal_gpio_enable_output(gpio_device, SDA);
        metal_gpio_set_pin(gpio_device, SDA, 0);
      }
      basic_delay(LONG_DELAY);

    }

    if(ii != (size_r-1)){
		  metal_gpio_disable_output(gpio_device, SDA);
		  metal_gpio_enable_input(gpio_device, SDA);
          basic_delay(SHORT_DELAY);
    }
  }
}

/* Address is 7 bit. */
uint8_t i2c_read_byte(struct metal_gpio *gpio_device, uint8_t address, uint8_t reg_addr)
{
  uint8_t temp[2];
  uint8_t data;
  temp[0]=address<<1;
  temp[1]=reg_addr;
  while(i2c_busy(gpio_device));
  i2c_start(gpio_device);
  i2c_send_bytes_rs(gpio_device, temp, sizeof(temp), 1, &data, 1);
  i2c_stop(gpio_device);
  return data;
}


void i2c_write_byte(struct metal_gpio *gpio_device, uint8_t address, uint8_t reg_addr, uint8_t val)
{
  uint8_t temp[3];
  uint8_t data;
  temp[0]=address<<1;
  temp[1]=reg_addr;
  temp[2]=val;
  while(i2c_busy(gpio_device));
  i2c_start(gpio_device);
  i2c_send_bytes_rs(gpio_device, temp, sizeof(temp), 0, &data, 1);
  i2c_stop(gpio_device);
}

uint32_t read_fifo_len(struct metal_spi *spi, struct metal_spi_config *conf)
{
	uint32_t len = 0;
	char tx_buf[2] = {0x42, 0x00};
	char rx_buf[2];
	metal_spi_transfer(spi, conf, 2, tx_buf, rx_buf);
	len |= rx_buf[1];

	tx_buf[0] = 0x43;
	metal_spi_transfer(spi, conf, 2, tx_buf, rx_buf);
	len |= rx_buf[1] << 8;

	tx_buf[0] = 0x44;
	metal_spi_transfer(spi, conf, 2, tx_buf, rx_buf);
	len |= rx_buf[1] << 16;

	return len;
}

int main() {
	long cpu_clock = metal_clock_get_rate_hz(&__metal_dt_clock_4.clock);
	long new_cpu_clock = metal_clock_set_rate_hz(&__metal_dt_clock_4.clock, 360000000L);
	printf("New clk: %ld\n", new_cpu_clock);

	twoD_t input, output, output2, output3, output4, output5;

	// Maximum memory usage is 2*max(32*32, 6*14*14, 16*5*5, 120, 84, 10)*sizeof(data_t), ping pong buffer is being used.
	// Maximum memory usage is 2*max(1024, 1176, 400, 120, 84, 10)*sizeof(data_t), ping pong buffer is being used.

	data_t buffer1[1024] = {0};
	data_t buffer2[1176] = {0};

	input.r = input.c = 32;
	input.channel = 1;
	input.data = buffer1;
	input.bias = NULL;

	output.r = output.c = (32-5+1)/2;
	output.data = buffer2;
	output.bias = NULL;

	output2.r = output2.c = (14-5+1)/2;
	output2.data = buffer1;
	output2.bias = NULL;

	output3.r = 120;
	output3.c = 1;
	output3.data = buffer2;
	output3.bias = NULL;

	output4.r = 84;
	output4.c = 1;
	output4.data = buffer1;
	output4.bias = NULL;

	output5.r = 10;
	output5.c = 1;
	output5.data = buffer2;
	output5.bias = NULL;

	struct metal_gpio *gpio_device = metal_gpio_get_device(0);
	i2c_init(gpio_device);
	if ((i2c_read_byte(gpio_device, OV2640_I2C_ADDR, OV2640_PIDH) == 0x26) &&
			(i2c_read_byte(gpio_device, OV2640_I2C_ADDR, OV2640_PIDL) == 0x42))
	{
		printf("OV2640 OK!\n");
	}

	i2c_write_byte(gpio_device, OV2640_I2C_ADDR, 0xff, 0x01);
	i2c_write_byte(gpio_device, OV2640_I2C_ADDR, 0x12, 0x80);
	uint32_t config_row = sizeof(OV2640_QVGA) / sizeof(OV2640_QVGA[0]);
	for (uint32_t i = 0; i < config_row; i++)
	{
		i2c_write_byte(gpio_device, OV2640_I2C_ADDR, OV2640_QVGA[i][0], OV2640_QVGA[i][1]);
		//printf("i2c read: [%x]=%x --- %x\n", OV2640_QVGA[i][0], OV2640_QVGA[i][1], i2c_read_byte(gpio_device, OV2640_I2C_ADDR, OV2640_QVGA[i][0]));
		basic_delay(LONG_DELAY);
	}

	struct metal_spi *spi = metal_spi_get_device(1);

	if(spi == NULL) {
		printf("Failed to get spi1 device \n");
		return 1;
	}

	metal_spi_init(spi, 500000);

	/* CPOL = 0, CPHA = 1, MSB-first, CS active low */
	struct metal_spi_config config = {
		.protocol = METAL_SPI_SINGLE,
		.polarity = 0,
		.phase = 0,
		.little_endian = 0,
		.cs_active_high = 0,
		.csid = 0,
//		.cmd_num = 1,
//		.addr_num = 1,
//		.dummy_num = 1,
	};
	char tx_buf[2] = {OV2640_SPI_VERSION, 0x00};
	char rx_buf[2];
	metal_spi_transfer(spi, &config, 2, tx_buf, rx_buf);
	if (rx_buf[1] == 0x73 )
	printf("Arducam OK! %x-%x\n", rx_buf[0], rx_buf[1]);

	tx_buf[0] = 0x80 | OV2640_SPI_MODE;
	tx_buf[1] = 0x00;
	metal_spi_transfer(spi, &config, 2, tx_buf, rx_buf);

	tx_buf[0] = 0x80 | OV2640_SPI_FRMCTRL;
	tx_buf[1] = 0x00;
	// Capture 1 frame
	metal_spi_transfer(spi, &config, 2, tx_buf, rx_buf);
	for(;;)
	{

	// Fifo clear
	tx_buf[0] = 0x80 | OV2640_SPI_FIFO;
	tx_buf[1] = 0x01;
	metal_spi_transfer(spi, &config, 2, tx_buf, rx_buf);

	// Fifo clear
	tx_buf[0] = 0x80 | OV2640_SPI_FIFO;
	tx_buf[1] = 0x01;
	metal_spi_transfer(spi, &config, 2, tx_buf, rx_buf);


	basic_delay(LONG_DELAY);

	// Start fifo capture
	tx_buf[0] = 0x80 | OV2640_SPI_FIFO;
	tx_buf[1] = 0x02;
	metal_spi_transfer(spi, &config, 2, tx_buf, rx_buf);

	tx_buf[0] = 0x41;
	while(1)
	{
		metal_spi_transfer(spi, &config, 2, tx_buf, rx_buf);
		if (rx_buf[1] & 0x08)
		{
			printf("Capture done: 0x%x-0x%x\n", rx_buf[0], rx_buf[1]);
			break;
		}
		basic_delay(SHORT_DELAY);
	}

	uint32_t fifo_size = read_fifo_len(spi, &config);
	printf("Fifo_len: %d\n", fifo_size);
	
	tx_buf[0] = 0x3D;

	//metal_tty_putc_raw('a');
	//metal_tty_putc_raw('b');
	
	uint8_t offset = SKIP_BYTES;
	while(offset--)
	{
		fifo_size--;
		metal_spi_transfer(spi, &config, 2, tx_buf, rx_buf);
		basic_delay(LONG_DELAY);
	}

	uint32_t buffer1_inx = 0; 
	for (uint8_t r = 0; r < 64; r++)
	{
		for (uint8_t c = 0; c < 64; c++)
		{
			metal_spi_transfer(spi, &config, 2, tx_buf, rx_buf);
			basic_delay(LONG_DELAY);
			//metal_tty_putc_raw(rx_buf[1]);
			if (c%2==1 && r%2==1){	
				buffer1[buffer1_inx] = rx_buf[1];
				if (buffer1[buffer1_inx] < 100.0)
				{
					buffer1[buffer1_inx] = 0.0;
				}
				buffer1[buffer1_inx++] = buffer1[buffer1_inx]/256.0;
				
			}
			fifo_size--;
		}
	}

	if(fifo_size == (8 - SKIP_BYTES))
	{
		printf("Transfer done!\n");
	}
		// Test input
		// memcpy(buffer1, test, sizeof(test));

		print_twoD(&input, 0, 256.0);

		input.channel = 1; /* Single channel input 32x32 matrix */
		output.channel = w_0_weight_2d.channel; /* 6 input channels */
		conv2D(&input, &w_0_weight_2d, &output, &reLU, 2, 2, 2);
		// print_twoD(&output, 1);

		output2.channel = w_3_weight_2d.channel; /* 6 input channels are maped to 16 channels */
		conv2D(&output, &w_3_weight_2d, &output2, &reLU, 2, 2, 2);
		// print_twoD(&output2, 0);

		output3.channel = w_7_weight_2d.channel; /* Flat single channel output for fully connected layer */
		dot(&output2, &w_7_weight_2d, &output3, &reLU);
		//print_twoD(&output3, 0);

		output4.channel = w_9_weight_2d.channel;
		dot(&output3, &w_9_weight_2d, &output4, &reLU);
		//print_twoD(&output4, 0);

		output5.channel = w_11_weight_2d.channel;
		dot(&output4, &w_11_weight_2d, &output5, NULL);

		printf("CLASS SCORES: ");
		print_twoD(&output5, 0, 100.0);
	
		printf("PREDICTION: %d\n", get_class(&output5));
	}
}
