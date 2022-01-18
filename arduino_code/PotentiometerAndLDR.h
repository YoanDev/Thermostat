#define POT A0 
#define LDR A1 

class Potentiometer
{
  private:
    uint8_t id;
  public:
    int pot_val();
};

int Potentiometer::pot_val()
{
  return (analogRead(POT)/32)+4; // between 4 and 36
}

//---------------------------------------//

class LDR_c
{
  private:
    uint8_t id;
    
  public:
    uint16_t ldr_val();
};

uint16_t LDR_c::ldr_val()
{
  return analogRead(LDR);
}
