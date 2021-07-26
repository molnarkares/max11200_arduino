#include <Arduino.h>
#include <typeinfo>
#include <SPI.h>

class Max11200 {

  protected:
    SPIClass &spi;
    uint8_t cs;
    uint8_t din;
    uint8_t spi_init;
    uint8_t ctrl2_reg;
    uint8_t ctrl1_reg;
    // uint8_t ctrl3_reg;

    //CMD bits
    static const uint8_t start = 1<<7;
    static const uint8_t mode = 1<<6;
    static const uint8_t cal1 = 1<<5;
    static const uint8_t cal0 = 1<<4;
    static const uint8_t impd = 1<<3;
    
    //CTRL3 bits
    static const uint8_t nosysg = 1<<4;
    static const uint8_t nosyso = 1<<3;
    static const uint8_t noscg = 1<<2;
    static const uint8_t nosco = 1<<1;

    //REGISTER addresses
    static const uint8_t stat1 = 0;    
    static const uint8_t ctrl1 = 1;
    static const uint8_t ctrl2 = 2;
    static const uint8_t ctrl3 = 3;
    static const uint8_t data = 4;
    static const uint8_t soc = 5;
    static const uint8_t sgc = 6;
    static const uint8_t scoc = 7;
    static const uint8_t scgc = 8;
  public:
    void cmd(uint8_t d);
    void w8(uint8_t address, uint8_t d);
    void w24(uint8_t address, uint32_t d);
    uint8_t r8(uint8_t address);
    uint32_t r24(uint8_t address);


  // public:
    union rate {
      union cont{
        enum class hz50 {rate50=0b100, rate100=0b101, rate200=0b110, rate400=0b111};    
        enum class hz60 {rate60=0b100, rate120=0b101, rate240=0b110, rate480=0b111};    
      };
      union single {
        enum class hz50 { rate0p83=0b000, rate2p08=0b001, rate4p17=0b010, rate8p33=0b011,
                          rate12p5=0b100, rate25=0b101, rate50=0b110, rate100=0b111};    
        enum class hz60 { rate1=0b000, rate2p5=0b001, rate5=0b010, rate10=0b011,
                          rate15=0b100, rate30=0b101, rate60=0b110, rate120=0b111};    
      };
    };
    enum class dPin {gpio1=0, gpio2=1, gpio3=2, gpio4=3};
    enum class dMode {input, output};
    enum class dVal {low=0, high=1};

    //CTRL1 bits
    static const uint8_t linef = 1<<7;
    static const uint8_t ub = 1<<6;
    static const uint8_t extclk = 1<<5;
    static const uint8_t refbuf = 1<<4;
    static const uint8_t sigbuf = 1<<3;
    static const uint8_t format = 1<<2;
    static const uint8_t scycle  = 1<<1;

    Max11200(SPIClass &_spi, const uint8_t _cs, const uint8_t _din, bool _spi_init = true);
    void begin();
    void end();
    void sleep();
    bool busy();
    bool dataReady();
    bool waitForReady(unsigned long timeout);
    void config(uint8_t cfg);
    uint8_t status();
    uint32_t analogRead();      
    int32_t analogReadSigned();
    uint32_t analogReadUnsigned();
    void pinMode(dPin pin, dMode mode);
    void digitalWrite(dPin pin, dVal value) ;
    dVal digitalRead (dPin pin);
    void calibrateSelf();
    void calibrateOffset();
    void calibrateGain();
    void setRate(uint8_t r);
    void setRate(rate::cont::hz50 r);
    void setRate(rate::cont::hz60 r);
    void setRate(rate::single::hz50 r);
    void setRate(rate::single::hz60 r);

};

class Max11210 : public Max11200 {
    public:
        using Max11200::Max11200;
        enum class gain{g1 = 0, 
                        g2 = 1,
                        g4 = 2,
                        g8 = 3,
                        g16 = 4};
        void setgain(gain _g);
};