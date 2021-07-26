#include "max11200.h"

Max11200::Max11200(SPIClass &_spi, const uint8_t _cs, const uint8_t _din, bool _spi_init) : 
                  spi(_spi), 
                  cs(_cs),
                  din(_din),
                  spi_init(_spi_init) {
  ::pinMode(cs, OUTPUT);
  ::digitalWrite(cs, HIGH);  
  ctrl2_reg = 0;
}

void Max11200::cmd(uint8_t d) {
  
  d &= ~mode;
  d |= start;
  
  ::digitalWrite(cs, LOW);  

  spi.transfer(d);
 
  ::digitalWrite(cs, HIGH);

}


void Max11200::w8(uint8_t address, uint8_t d) {
  address &= 0x0f;
  address <<=1;
  address |= (start|mode);
  
  ::digitalWrite(cs, LOW);  

  spi.transfer(address);
  spi.transfer(d);
   
  ::digitalWrite(cs, HIGH);
}

void Max11200::w24(uint8_t address, uint32_t d) {
  address &= 0x0f;
  address <<=1;
  address |= (start|mode);
  
  ::digitalWrite(cs, LOW);  

  spi.transfer(address);
  spi.transfer(static_cast<uint8_t>(d>>16));
  spi.transfer(static_cast<uint8_t>(d>>8));
  spi.transfer(static_cast<uint8_t>(d));
   
  ::digitalWrite(cs, HIGH);
}


uint8_t Max11200::r8(uint8_t address) {
  address &= 0x0f;
  address <<=1;
  address |= (start|mode|0x01);
  
  ::digitalWrite(cs, LOW);  

  spi.transfer(address);
  uint8_t ret = spi.transfer(0);
 
  ::digitalWrite(cs, HIGH);
  return ret;
}

uint32_t Max11200::r24(uint8_t address) {
  address &= 0x0f;
  address <<=1;
  address |= (start|mode|0x01);
  
  ::digitalWrite(cs, LOW);  

  spi.transfer(address);
  uint32_t ret = spi.transfer(0);
  ret <<=8;
  ret |= spi.transfer(0);
  ret <<=8;
  ret |= spi.transfer(0);
 
  ::digitalWrite(cs, HIGH);
  return ret;
}


void Max11200::begin(){
  if(spi_init) {
    spi.begin();
  }
  calibrateSelf();
}

void Max11200::end(){
}


uint8_t Max11200::status() {
    return r8(stat1);
}

void Max11200::config(uint8_t cfg) {
    w8(ctrl1,cfg);
}

void Max11200::calibrateSelf(){
  cmd(cal0);
  // delay(210); //cca 200ms calibration
  delay(210); //cca 200ms calibration
  while(busy()) {
       delay(5);
  }
  uint8_t ctrl3_reg = r8(ctrl3); // capture actual state of ctrl3
  ctrl3_reg &= ~(noscg|nosco);
  w8(ctrl3, ctrl3_reg);
}



bool Max11200::waitForReady(unsigned long timeout){
  unsigned long tend = millis() + timeout;
  bool ret = false;
  ::digitalWrite(cs, LOW);
  delayMicroseconds(1);
  while(tend > millis()){
    if(!::digitalRead(din)) {
      ret = true;
      break;
    }
  }
  ::digitalWrite(cs, HIGH);
  return ret;
}

bool Max11200::dataReady() {
  ::digitalWrite(cs, LOW);
  delayMicroseconds(1);
  uint8_t nrdy = ::digitalRead(din);
  ::digitalWrite(cs, HIGH);
  return nrdy ? false : true;
}

bool Max11200::busy(){
  auto stat1_reg = status();
  return ((stat1_reg & 0x02) != 0); 
}

void Max11200::setRate(uint8_t r) {
  cmd(r);
}
void Max11200::setRate(rate::cont::hz50 r){
  cmd(static_cast<uint8_t>(r));
}
void Max11200::setRate(rate::cont::hz60 r){
  cmd(static_cast<uint8_t>(r));
}
void Max11200::setRate(rate::single::hz50 r){
  cmd(static_cast<uint8_t>(r));
}
void Max11200::setRate(rate::single::hz60 r){
  cmd(static_cast<uint8_t>(r));
}

void Max11200::sleep(){
  cmd(impd);
}

uint32_t Max11200::analogRead() {
  return r24(0x4);
}

uint32_t Max11200::analogReadUnsigned() {
  return r24(0x4);
}

int32_t Max11200::analogReadSigned() {
  uint32_t ret = r24(0x4);
  if((ret & 0x800000) != 0) {
    ret |= 0xff000000;
  }
  return static_cast<int32_t>(ret);
}


void Max11200::pinMode(dPin pin, dMode mode) {
  ctrl2_reg = r8(ctrl2); // capture actual state of input pins  
  switch(mode){
    case dMode::input:
      ctrl2_reg &= ~(1<<(static_cast<int>(pin)+4));      
      break;
    case dMode::output:
      ctrl2_reg |= (1<<(static_cast<int>(pin)+4));
      break;
    default:
      break;
  }
  w8(ctrl2,ctrl2_reg);
}

void Max11200::digitalWrite(dPin pin, dVal value){

  if(value == dVal::high){
    ctrl2_reg |= (1 << static_cast<int>(pin));
  }else {
    ctrl2_reg &= ~(1 << static_cast<int>(pin));
  }
  w8(ctrl2,ctrl2_reg);
}

Max11200::dVal Max11200::digitalRead (dPin pin) {
  ctrl2_reg = r8(ctrl2);

  dVal ret = ((ctrl2_reg & 1<<static_cast<int>(pin)) != 0) ? dVal::high : dVal::low;

  return ret;  

}

void Max11210::setgain(gain _g) {
    uint8_t ctrl3_reg = r8(ctrl3); // capture actual state of ctrl3
    ctrl3_reg &= ~0xe0;
    ctrl3_reg |= static_cast<int>(_g)<<5;
    w8(ctrl3,ctrl3_reg);
}