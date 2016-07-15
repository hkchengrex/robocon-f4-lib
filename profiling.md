# Profiling Note

## General
- Ticks frequency: 84000000 (half of main frequency)
- Ticks prescaler: 83
- Ticks period: 2
- Result: 1 ticks = 2 us

- Optimization level: *O1*
- Not optimized for time

***

## Floating Point calculator (FPU)

Using the following code:

	s32 starting_ticks = get_full_ticks();
	
	volatile float result[200] = {0.0f};
	for (u16 i=0;i<200;i++){
		result[i] = (float)rand() / (float)RAND_MAX;
	}
	
	for (u16 i=1;i<199;i++){
		result[i] = ((result[i]*65.37f*result[i-1] - result[i-2])+14.589f)/562.3458f;
	}

	for (u16 i=198;i>1;i--){
		result[i] = result[i]/result[i+1]/result[i-1];
	}
	
	s32 end_ticks = get_full_ticks();

Then find end_ticks - starting_ticks.
### With single point precision hardware *disabled*
- ticks diff = *808*
### With single point precision hardware *enabled*
- ticks diff = *101*

No new random seeds are generated.
Conclusion: FPU helps.

***

## Comparing square root funciton in different libraries (with FPU)

Using the following code:
	
	#define SAMPLES 100
	
	volatile s32 result[SAMPLES] = {0};
	s32 original[SAMPLES] = {0};
	
	for (u16 i=0;i<SAMPLES;i++){
		float32_t tmp = (float32_t)rand() /(float32_t)RAND_MAX *1000000.0f;
		original[i] = (s32)roundf(tmp);
	}
	
	s32 starting_ticks = get_full_ticks();
	
	for (u16 i=0;i<SAMPLES;i++){
		//result[i] = Sqrt(original[i]);
		//result[i] = s32_sqrt(original[i]);
		result[i] = s32_sqrt2(original[i]);
	}
	
	s32 end_ticks = get_full_ticks();
	
	float total_error = 0;
	for (u16 i=0;i<SAMPLES;i++){
		total_error += (result[i] - sqrt(original[i])*1024.0f)/1024.0f*100.0f/original[i];
	}

sqrt() function of math.h is considered to be the most accurate result.
All functions are declared inline.

### With approx_math Sqrt()
- Scaled by 1024
- Ticks used = 30
- Average error = -0.053578%

### With quick_math s32_sqrt()
- A warp for __sqrtf() and roundf(), scaled by 1024
- Ticks used = 49
- Average error = 0.000124%

### With quick_math s32_sqrt2()
- A warp for sqrtf() and roundf(), scaled by 1024
- Ticks used = 60
- Average error = 0.000124%

Discussion: Sqrt() in approx_math is the quickest, but with a slightly larger error. The error in sqrtf() can be reduced by increasing the scaling, but it is not the case for Sqrt() in approx_math.

Conclusion: Sqrt() in qpprox_math will be adopted. In the case of floating point, use __sqrtf().

***

## Comparing sine/cosine funciton in different libraries (with FPU)