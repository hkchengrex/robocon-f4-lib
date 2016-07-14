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
	
	volatile s32 result[100] = {0};
	s32 math_h_result[100];
	s32 original[100] = {0};
	for (u16 i=0;i<100;i++){
		float tmp = (float)rand() /(float)RAND_MAX *1000000.0f;
		original[i] = (s32)roundf(tmp);
		result[i] = (s32)roundf(tmp);
		math_h_result[i] = (s32)roundf(sqrtf(tmp));
	}
	
	s32 starting_ticks = get_full_ticks();

	for (u16 i=0;i<100;i++){
		result[i] = Sqrt(original[i])/1000;
		//result[i] = u32_sqrt(original[i]);
		//result[i] = (s32)roundf(sqrtf(original[i]));
	}
	
	s32 end_ticks = get_full_ticks();
	
	s32 total_error = 0;
	for (u16 i=0;i<100;i++){
		total_error += result[i] - math_h_result[i];
	}

sqrtf() function of math.h is considered to be the most accurate result.

### With approx_math Sqrt()
- Ticks used = 33
- Total error = -31528

### With quick_math u32_sqrt()
- Ticks used = 102
- Total error = -58

### With math.h sqrtf() and roundf()
- Ticks used = 52
- Total error = 0

### With arm's _sqrtf() function
- Ticks used = 41
- Total error = 0

Then I got curious and turned off the FPU, it crashes every time even I tried to reduce its workload....

Conclusion: Just go roundf(_sqrtf)... If you *really* need to go fast, use approx_math.
Note that /1000 is used for apprx_math function and some accuracy is lost but it's not much. _sqrtf would still be much better.
And u32_sqrt in quick math is shit. *Cry in code*
