# AutoCorr_Freq_detect
An Arduino example of Autocorrelation for the detection of signal frequency

Accurate Frequency Detection is important for many projects such as Guitar/Piano Tuners, Vibration Analyzers, Heartrate Monitors, MEMs Sensor Analysis and Laboratory Instruments.
There have been many fine examples of projects that try to solve this problem but they all use Time Domain techniques; analyzing the signal for features such as : Zero-Crossings, Peak Detection, Slope Detection etc..
 

A Piano playing Middle-C (C4) looks very different from a Synthesizer Playing Middle-C (C4). Any good Time Domain algorithm will work well with the Piano waveform. But the Synthesizer waveform will not be identifiable that way because its very strong harmonic content makes the fundamental frequency undiscernable . It looks impossible to Identify the Frequency of this signal.

It is possible.

Using Autocorrelation it was measured to be 259.91Hz ... only 0.09Hz away from an Exact Middle C Frequency of 260Hz.

See more here: http://www.akellyirl.com/reliable-frequency-detection-using-dsp-techniques/

Looked at code to optimize detection time and edited to run across all four sample files with new and Original code.  These results from a Teensy 3.6 at 24 MHz compiled for smallest size to be closer to 8 bit AVR's - with enough FLASH/RAM to run this:
Sketch uses 25,396 bytes
Global variables use 6,944 bytes

Major optimization differences for this n-Squared process:
- precalculate the subtraction of 128 to center the value on zero
- remove the /256 operation that changes the magnitude for calculations but not the end result
- when detected the state was set to 3 and no longer changed but continued to run the rest of the nested loop
- adjusted length of sample values used in the test to 250 where it worked as below then increased to 325 to confirm same result, then ran on full set of sample values provided

 GetFreq for AltoSaxVib_C4 ::  
-------- Frequency Detected = 256.40 
-	FULL us= 13423	DONE us= 7660	sample len= 250
-------- Frequency Detected = 256.40
-	FULL us= 22538	DONE us= 10385	sample len= 325
-------- Frequency Detected = 256.40 
-	FULL us= 216564	DONE us= 35491	sample len= 1014

GetFreq with _ORIGINAL_ CODE for  AltoSaxVib_C4 ::  
-------- Frequency Detected = 256.40
-	FULL us= 348506	DONE us= 57276	sample len= 1014

 GetFreq for C4 ::  
-------- Frequency Detected = 259.41
-	FULL us= 13416	DONE us= 7580	sample len= 250
-------- Frequency Detected = 259.41
-	FULL us= 22542	DONE us= 10287	sample len= 325
-------- Frequency Detected = 259.41
-	FULL us= 198276	DONE us= 33518	sample len= 970

GetFreq with _ORIGINAL_ CODE for  C4 ::  
-------- Frequency Detected = 259.41
-	FULL us= 340545	DONE us= 57608	sample len= 970

 GetFreq for Guitar_C5 ::  
-------- Frequency Detected = 512.79
-	FULL us= 13410	DONE us= 4272	sample len= 250
-------- Frequency Detected = 512.79
-	FULL us= 22534	DONE us= 5653	sample len= 325
-------- Frequency Detected = 512.79
-	FULL us= 216988	DONE us= 18374	sample len= 1015

GetFreq with _ORIGINAL_ CODE for  Guitar_C5 ::  
-------- Frequency Detected = 512.79
-	FULL us= 346898	DONE us= 29429	sample len= 1015

 GetFreq for Piano_C4 ::  
-------- Frequency Detected = 259.41
-	FULL us= 13422	DONE us= 7586	sample len= 250
-------- Frequency Detected = 259.41
-	FULL us= 22541	DONE us= 10283	sample len= 325
-------- Frequency Detected = 259.41
-	FULL us= 216565	DONE us= 35101	sample len= 1014

GetFreq with _ORIGINAL_ CODE for  Piano_C4 ::  
-------- Frequency Detected = 259.41
-	FULL us= 378525	DONE us= 61532	sample len= 1014

And to show the full speed Teensy 3.6 difference on the optimized reduced samples:

 GetFreq for AltoSaxVib_C4 ::  
	FULL us= 1068	DONE us= 610	sample len= 250

 GetFreq for C4 ::  
	FULL us= 1069	DONE us= 605	sample len= 250

 GetFreq for Guitar_C5 ::  
	FULL us= 1069	DONE us= 342	sample len= 250

 GetFreq for Piano_C4 ::  
	FULL us= 1069	DONE us= 606	sample len= 250
