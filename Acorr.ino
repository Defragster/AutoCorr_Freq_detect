#include "Piano_C4.h"
#include "Guitar_C5.h"
#include "C4.h"
#include "AltoSaxVib_C4.h"
// Sample Frequency in kHz
const float sample_freq = 22050;
// const float sample_freq = 44100;  // Changing this still finds a freq - it just gets mis-read!
unsigned char * rawData[4] =
{
  ArawData, CrawData, GrawData, PrawData
};

char rawData_sz[][20] =
{
  "AltoSaxVib_C4", "C4", "Guitar_C5", "Piano_C4"
};

uint16_t rawData_len[4] =
{
  sizeof(ArawData), sizeof(CrawData), sizeof(GrawData), sizeof(PrawData)
};

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\n HI _QUAD_");
}

void loop()
{
  // put your main code here, to run repeatedly:
  static uint16_t jj = 0;
  for (; jj < 4; jj++)
  {
    Serial.print("\n GetFreq for ");
    Serial.print(rawData_sz[jj]);
    Serial.println(" ::  ");
    GetFreq(rawData[jj], rawData_len[jj]);
    Serial.print("GetFreq with _ORIGINAL_ CODE for  ");
    Serial.print(rawData_sz[jj]);
    Serial.print(" ::  ");
    Original(rawData[jj], rawData_len[jj]);
  }
}

void GetFreq(unsigned char rawData[], uint16_t lenP)
{
  int16_t diffs[2000];
  int ii, kk;
  long sum, sum_old;
  int thresh = 0;
  float freq_per = 0;
  byte pd_state = 0;
  float hld_freq_per = -1;
  int pd_3_ii;
  int len; // Used to test shorter samples
  if (lenP > 250)
    len = 250;
  else
    len = lenP;
  for (kk = 0; kk < lenP; kk++)
    diffs[kk] = rawData[kk] - 128; // x32 device workks best with int16
  sum = 0;
  pd_state = 0;
  int period = 0;
  while (len <= lenP)
  {
    uint32_t timr = micros();
    uint32_t timr2 = micros();
    for (ii = 0; ii < len; ii++)
    {
      // Autocorrelation
      sum_old = sum;
      sum = 0;
      for (kk = 0; kk < len - ii; kk++)
        sum += (diffs[kk] * diffs[kk + ii]);
      // Peak Detect State Machine
      if (pd_state == 2 && (sum - sum_old) <= 0)
      {
        timr2 = micros() - timr2;
        pd_3_ii = 0;
        period = ii;
        pd_state = 3; // Once this is set it won't change - so leave
        if (0)
        {
          Serial.print(ii);
          Serial.print("=ii _ kk=");
          Serial.print(kk);
          Serial.print(":: ");
        }
      }
      if (pd_state == 3 && (sum - sum_old) <= 0)
      {
        if (pd_3_ii < 5)
        {
          if (0)
          {
            Serial.print(ii);
            Serial.print("_#3_");
            Serial.print(kk);
            Serial.print(",  ");
          }
        }
        // else break;  // Stopping here finishes faster as this is the answer if "enough" samples!
        pd_3_ii++;
      }
      if (pd_state == 1 && (sum > thresh) && (sum - sum_old) > 0)
        pd_state = 2;
      if (!ii)
      {
        thresh = sum * 0.5;
        pd_state = 1;
      }
    }
    timr = micros() - timr;
    // Frequency identified in kHz
    if (period)
      freq_per = sample_freq / period;
    else
      freq_per = 0;
    Serial.print("-------- Frequency Detected = ");
    Serial.print(freq_per);
    Serial.println("    <<<<<<<<<<<<<<<<<<<<<<<<");
    Serial.print("\tFULL us= ");
    Serial.print(timr);
    Serial.print("\tDONE us= ");
    Serial.print(timr2);
    Serial.print("\tsample len= ");
    Serial.println(len);
    if (len == lenP)
      break;
    if (hld_freq_per == freq_per && 0 != freq_per)
      len = lenP;
    else
      len *= 1.3;
    hld_freq_per = freq_per;
  }
}

void Original(unsigned char rawData[], uint16_t len)
{
  int i, k;
  long sum, sum_old;
  int thresh = 0;
  float freq_per = 0;
  byte pd_state = 0;
  sum = 0;
  pd_state = 0;
  int period = 0;
  uint32_t timr = micros();
  uint32_t timr2 = micros();
  for (i = 0; i < len; i++)
  {
    // Autocorrelation
    sum_old = sum;
    sum = 0;
    for (k = 0; k < len - i; k++)
      sum += (rawData[k] - 128) * (rawData[k + i] - 128) /256;
    /*  // RX8 [h=43] @1Key1 @0Key1
    Serial.print("C");
    Serial.write((rawData[i]-128)>>8);
    Serial.write((rawData[i]-128)&0xff); */
    /*  // RX8 [h=43] @1Key1 @0Key1
    Serial.print("C");
    Serial.write(sum>>8);
    Serial.write(sum&0xff); */
    // Peak Detect State Machine
    if (pd_state == 2 && (sum - sum_old) <= 0)
    {
      timr2 = micros() - timr2;
      period = i;
      pd_state = 3;
    }
    if (pd_state == 1 && (sum > thresh) && (sum - sum_old) > 0)
      pd_state = 2;
    if (!i)
    {
      thresh = sum * 0.5;
      pd_state = 1;
    }
  }
  timr = micros() - timr;
  // Frequency identified in kHz
  freq_per = sample_freq/period;
  Serial.print("\n-------- Frequency Detected = ");
  Serial.print(freq_per);
  Serial.println("    <<<<<<<<<<<<<<<<<<<<<<<<");
  Serial.print("\tFULL us= ");
  Serial.print(timr);
  Serial.print("\tDONE us= ");
  Serial.print(timr2);
  Serial.print("\tsample len= ");
  Serial.println(len);
}
