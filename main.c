#include "cyhal.h"
#include "cybsp.h"
#include "mtb_ssd1306.h"
#include "GUI.h"
#include "cy_retarget_io.h"

#define PIN_ADC (P10_0)
#define ACQUISITION_TIME_NS (1000u)
#define MICRO_TO_MILLI_CONV_RATIO (1100u)
int32_t voltaje;
float32_t temperatura;

int main(void)
{
	cy_rslt_t result;
	    cyhal_i2c_t i2c_obj;
	    cyhal_adc_t adc_obj;
	    	cyhal_adc_channel_t adc_chan_0_obj;


    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    /*BSP init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);

    }

    /* Initialize retarget-io to use the debug UART port */
         	result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
         	CY_ASSERT(result == CY_RSLT_SUCCESS);

         	/* ADC conversion result. */
         	     	/* Initialize ADC. The ADC block which can connect to pin 10[6] is selected */
         	     	result = cyhal_adc_init(&adc_obj, PIN_ADC, NULL);

         	     	// ADC configuration structure
         	     	const cyhal_adc_config_t ADCconfig ={
         	     		.continuous_scanning = false,
         	     		.resolution = 12,
         	     		.average_count = 1,
         	     		.average_mode_flags = 0,
         	     		.ext_vref_mv = 0,
         	     		.vneg = CYHAL_ADC_VNEG_VREF,
         	     		.vref = CYHAL_ADC_REF_VDDA,
         	     		.ext_vref = NC,
         	     		.is_bypassed = false,
         	     		.bypass_pin = NC
         	     	};

         	     	// Configure to use VDD as Vref
         	     	result = cyhal_adc_configure(&adc_obj, &ADCconfig);

         	     	/* Initialize ADC channel, allocate channel number 0 to pin 10[6] as this is the first channel initialized */
         	     	const cyhal_adc_channel_config_t channel_config = { .enable_averaging = false, .min_acquisition_ns=ACQUISITION_TIME_NS , .enabled = true };
         	     	result = cyhal_adc_channel_init_diff(&adc_chan_0_obj, &adc_obj, PIN_ADC, CYHAL_ADC_VNEG, &channel_config);


    /* Initialize the I2C to use with the OLED display */
       result = cyhal_i2c_init(&i2c_obj, CYBSP_I2C_SDA, CYBSP_I2C_SCL, NULL);

    /* Initialize the User LED */
    result = cyhal_gpio_init(P9_4, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
    result = cyhal_gpio_init(P9_7, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
	/* GPIO init failed. Stop program execution */
	if (result != CY_RSLT_SUCCESS)
	{
		CY_ASSERT(0);




	}

	/* Initialize the user button */
	result = cyhal_gpio_init(P6_2, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);
		/* GPIO init failed. Stop program execution */
	result = cyhal_gpio_init(P6_5, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);
	/* GPIO init failed. Stop program execution */

	  /* Initialize OLED display */
	    result = mtb_ssd1306_init_i2c(&i2c_obj);

	if (result != CY_RSLT_SUCCESS)
	{
		CY_ASSERT(0);
	}
    __enable_irq();

    GUI_Init();
    GUI_DispStringAt("Temperatura:", 0, 10);
    GUI_DispStringAt("PROYECT TD2 PORTAL.M", 0, 50);

    for (;;)
    {

    	    	/* Read the ADC conversion result for corresponding ADC channel. Repeat as necessary. */
    	    	    	    			voltaje = cyhal_adc_read_uv(&adc_chan_0_obj)/MICRO_TO_MILLI_CONV_RATIO;
    	    	    	    			temperatura= (voltaje/30.00);
    	    	    	    			GUI_GotoXY(75, 10);
    	    	    	    			GUI_DispFloatMin(temperatura,1);
    	    	    	    			GUI_DispString("C\n");
    	    	    	    			printf("Temperatura en mV:%4ldmV\r\n",(long int)voltaje);
    	    	    	    			printf("TEMPERATURA EN GRADOS CENTIGRADOS:%0.1fºC\r\n", (float)temperatura);
    	    	    	    			cyhal_system_delay_ms(500);

    	  //Button is active low
    	    	 if(!cyhal_gpio_read(P6_2)){
    	    	 cyhal_gpio_write(P9_7, CYBSP_LED_STATE_ON);
    	    	 GUI_DispStringAt("MOV IZQUIERDA", 10, 30);

    	    	}
    	    	else{
    	    	   cyhal_gpio_write(P9_7, CYBSP_LED_STATE_OFF);

    	    	}
    	        if(!cyhal_gpio_read(P6_5)){
    	        cyhal_gpio_write(P9_4, CYBSP_LED_STATE_ON);
    	        GUI_DispStringAt("MOV DERECHA", 10, 30);

    	        }
    	    	else{
    	        cyhal_gpio_write(P9_4, CYBSP_LED_STATE_OFF);

    	    	    	    			          }
    	}
}




/* [] END OF FILE */
