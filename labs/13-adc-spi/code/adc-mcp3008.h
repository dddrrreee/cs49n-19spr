
#ifndef __ADC_MCP3008_H__
#define __ADC_MCP3008_H__

// note: you can encode binary constants as 0b....  (e.g., to say 3 do 0b11)
enum {
    CH0 = ???,
    CH1 = ???,
    CH2 = ???,
    CH3 = ???,
    CH4 = ???,
    CH5 = ???,
    CH6 = ???,
    CH7 = ???,
};

// call first!
void adc_mcp3008_init(void);

// returns a single 10 bit quantity.
unsigned adc_mcp3008_read(int ch);


// can call these directly, but probably just use the mcp3008_read()
void adc_mcp3008_wr_cmd(uint8_t tx[], int ch);
unsigned adc_mcp3008_rd_cmd(uint8_t rx[]);


#endif
